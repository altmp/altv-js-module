#include "cpp-sdk/objects/IEntity.h"
#include "cpp-sdk/objects/IPlayer.h"

#include "cpp-sdk/events/CEvent.h"
#include "cpp-sdk/events/CClientScriptEvent.h"
#include "cpp-sdk/events/CServerScriptEvent.h"
#include "cpp-sdk/events/CWebViewEvent.h"
#include "cpp-sdk/events/CWebSocketClientEvent.h"
#include "cpp-sdk/events/CKeyboardEvent.h"
#include "cpp-sdk/events/CConnectionComplete.h"
#include "cpp-sdk/events/CDisconnectEvent.h"
#include "cpp-sdk/events/CGameEntityCreateEvent.h"
#include "cpp-sdk/events/CGameEntityDestroyEvent.h"
#include "cpp-sdk/events/CSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CStreamSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CGlobalSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CRemoveEntityEvent.h"
#include "cpp-sdk/events/CResourceStartEvent.h"
#include "cpp-sdk/events/CResourceStopEvent.h"
#include "cpp-sdk/events/CResourceErrorEvent.h"
#include "cpp-sdk/SDK.h"

#include "CV8ScriptRuntime.h"
#include "CV8Resource.h"
#include "V8Helpers.h"
#include "V8Module.h"

#include "workers/CWorker.h"

extern void StaticRequire(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    v8::Isolate* isolate = info.GetIsolate();

    V8_CHECK(info.Length() == 1, "1 arg expected");
    V8_CHECK(info[0]->IsString(), "moduleName must be a string");

    V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredOrMicrotaskContext());
    V8_CHECK(resource, "invalid resource");

    std::string name{ *v8::String::Utf8Value{ isolate, info[0] } };

    v8::MaybeLocal<v8::Value> _exports = static_cast<CV8ResourceImpl*>(resource)->Require(name);

    if(!_exports.IsEmpty()) info.GetReturnValue().Set(_exports.ToLocalChecked());
    else
        V8Helpers::Throw(isolate, "No such module " + name);
}

void CV8ResourceImpl::ProcessDynamicImports()
{
    for(auto importFn : dynamicImports)
    {
        importFn();
    }
    dynamicImports.clear();
}

extern V8Module altModule;
bool CV8ResourceImpl::Start()
{
    if(resource->GetMain().IsEmpty()) return false;

    resource->EnableNatives();
    auto nscope = resource->PushNativesScope();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolate_scope(isolate);
    v8::HandleScope handle_scope(isolate);

    microtaskQueue = v8::MicrotaskQueue::New(isolate, v8::MicrotasksPolicy::kExplicit);
    v8::Local<v8::Context> ctx =
      v8::Context::New(isolate, nullptr, v8::MaybeLocal<v8::ObjectTemplate>(), v8::MaybeLocal<v8::Value>(), v8::DeserializeInternalFieldsCallback(), microtaskQueue.get());

    context.Reset(isolate, ctx);
    ctx->SetAlignedPointerInEmbedderData(1, resource);

    V8ResourceImpl::Start();

    v8::Context::Scope context_scope(ctx);

    // Log::Debug(V8ResourceImpl::GetResource(ctx));
    // Log::Debug(V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext()));

    /*runtime->GetInspector()->contextCreated({
            ctx,
            1,
            v8_inspector::StringView{ (uint8_t*)name.CStr(), name.GetSize() }
    });*/

    std::string path = resource->GetMain().ToString();

    alt::IPackage* pkg = resource->GetPackage();
    alt::IPackage::File* file = pkg->OpenFile(path);

    alt::String src{ pkg->GetFileSize(file) };

    pkg->ReadFile(file, src.GetData(), src.GetSize());
    pkg->CloseFile(file);

    Log::Info << "[V8] Starting script " << path << Log::Endl;

    v8::Local<v8::String> sourceCode = V8Helpers::JSValue(src);

    v8::ScriptOrigin scriptOrigin(isolate, V8Helpers::JSValue(path.c_str()), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());

    bool result = V8Helpers::TryCatch([&]() {
        v8::ScriptCompiler::Source source{ sourceCode, scriptOrigin };
        v8::MaybeLocal<v8::Module> maybeModule = v8::ScriptCompiler::CompileModule(isolate, &source);

        if(maybeModule.IsEmpty()) return false;

        v8::Local<v8::Module> curModule = maybeModule.ToLocalChecked();

        modules.emplace(path, v8::UniquePersistent<v8::Module>{ isolate, curModule });

        auto exports = altModule.GetExports(isolate, ctx);
        // Overwrite global console object
        auto console = ctx->Global()->Get(ctx, V8Helpers::JSValue("console")).ToLocalChecked().As<v8::Object>();
        if(!console.IsEmpty())
        {
            console->Set(ctx, V8Helpers::JSValue("log"), exports->Get(ctx, V8Helpers::JSValue("log")).ToLocalChecked());
            console->Set(ctx, V8Helpers::JSValue("warn"), exports->Get(ctx, V8Helpers::JSValue("logWarning")).ToLocalChecked());
            console->Set(ctx, V8Helpers::JSValue("error"), exports->Get(ctx, V8Helpers::JSValue("logError")).ToLocalChecked());
        }

        // Add global timer funcs
        ctx->Global()->Set(ctx, V8Helpers::JSValue("setInterval"), exports->Get(ctx, V8Helpers::JSValue("setInterval")).ToLocalChecked());
        ctx->Global()->Set(ctx, V8Helpers::JSValue("setTimeout"), exports->Get(ctx, V8Helpers::JSValue("setTimeout")).ToLocalChecked());
        ctx->Global()->Set(ctx, V8Helpers::JSValue("clearInterval"), exports->Get(ctx, V8Helpers::JSValue("clearInterval")).ToLocalChecked());
        ctx->Global()->Set(ctx, V8Helpers::JSValue("clearTimeout"), exports->Get(ctx, V8Helpers::JSValue("clearTimeout")).ToLocalChecked());

        ctx->Global()->Set(ctx, V8Helpers::JSValue("__internal_get_exports"), v8::Function::New(ctx, &StaticRequire).ToLocalChecked());

        bool res = curModule->InstantiateModule(ctx, CV8ScriptRuntime::ResolveModule).IsJust();

        if(!res) return false;

        v8::MaybeLocal<v8::Value> v = curModule->Evaluate(ctx);

        if(v.IsEmpty()) return false;

        if(curModule->GetStatus() == v8::Module::Status::kErrored)
        {
            v8::Local<v8::Promise> promise = v.ToLocalChecked().As<v8::Promise>();
            bool hasHandler = promise->HasHandler();
            if(!hasHandler)
            {
                promise->MarkAsHandled();
                isolate->ThrowException(promise->Result());
                return false;
            }
        }

        alt::MValue _exports = V8Helpers::V8ToMValue(curModule->GetModuleNamespace());
        resource->SetExports(_exports.As<alt::IMValueDict>());

        if(curModule->IsGraphAsync()) Log::Warning << "[V8] Top level await is an experimental feature, use it at your own risk" << Log::Endl;

        Log::Info << "[V8] Started script " << path << Log::Endl;
        return true;
    });

    if(!result)
    {
        modules.erase(path);
    }

    DispatchStartEvent(!result);

    // if all resources are already loaded
    if(CV8ScriptRuntime::Instance().resourcesLoaded)
    {
        ProcessDynamicImports();
    }

    return result;
}

bool CV8ResourceImpl::Stop()
{
    std::vector<alt::Ref<alt::IBaseObject>> objects(ownedObjects.size());

    for(auto handle : ownedObjects) objects.push_back(handle);

    ownedObjects.clear();

    // ?????
    // for (auto handle : objects)
    // {
    // 	CGame::Instance().DestroyBaseObject(handle);
    // }

    // runtime->GetInspector()->contextDestroyed(context.Get(isolate));

    for(auto pair : timers)
    {
        delete pair.second;
    }
    timers.clear();
    oldTimers.clear();

    for(auto worker : workers)
    {
        worker->Destroy();
    }
    workers.clear();

    if(!context.IsEmpty())
    {
        auto nscope = resource->PushNativesScope();

        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);
        auto ctx = GetContext();
        v8::Context::Scope scope(ctx);

        auto resources = static_cast<CV8ScriptRuntime*>(resource->GetRuntime())->GetResources();
        auto name = this->resource->GetName().ToString();
        for(auto res : resources)
        {
            if(res == this) continue;
            auto it = res->modules.find(name);
            if(it != res->modules.end())
            {
                res->modules.erase(it);
            }
            auto found = res->requiresMap.find(name);
            if(found != res->requiresMap.end())
            {
                res->requiresMap.erase(found);
            }
        }

        DispatchStopEvent();
    }

    return true;
}

bool CV8ResourceImpl::OnEvent(const alt::CEvent* e)
{
    auto nscope = resource->PushNativesScope();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());

    V8Helpers::EventHandler* handler = V8Helpers::EventHandler::Get(e);
    if(!handler) return true;

    // Generic event handler
    {
        auto evType = e->GetType();
        if(evType == alt::CEvent::Type::CLIENT_SCRIPT_EVENT || evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
        {
            std::vector<V8Helpers::EventCallback*> callbacks;
            const char* eventName;

            if(evType == alt::CEvent::Type::CLIENT_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(true));
                eventName = static_cast<const alt::CClientScriptEvent*>(e)->GetName().CStr();
            }
            else if(evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(false));
                eventName = static_cast<const alt::CServerScriptEvent*>(e)->GetName().CStr();
            }

            if(callbacks.size() != 0)
            {
                auto evArgs = handler->GetArgs(this, e);
                evArgs.insert(evArgs.begin(), V8Helpers::JSValue(eventName));

                InvokeEventHandlers(e, callbacks, evArgs);
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> callbacks = handler->GetCallbacks(this, e);
    if(callbacks.size() > 0)
    {
        std::vector<v8::Local<v8::Value>> args = handler->GetArgs(this, e);

        InvokeEventHandlers(e, callbacks, args);
    }

    // Dynamic imports
    {
        if(e->GetType() == alt::CEvent::Type::CONNECTION_COMPLETE)
        {
            CV8ScriptRuntime& runtime = CV8ScriptRuntime::Instance();
            if(!runtime.resourcesLoaded)
            {
                runtime.resourcesLoaded = true;
                ProcessDynamicImports();
            }
        }
        else if(e->GetType() == alt::CEvent::Type::DISCONNECT_EVENT)
        {
            CV8ScriptRuntime::Instance().resourcesLoaded = false;
        }
    }

    {
        if(e->GetType() == alt::CEvent::Type::WEB_VIEW_EVENT)
        {
            auto ev = static_cast<const alt::CWebViewEvent*>(e);
            if(ev->GetName() == "load") HandleWebViewEventQueue(ev->GetTarget());
        }
    }

    return true;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetWebViewHandlers(alt::Ref<alt::IWebView> view, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = webViewHandlers.find(view.Get());

    if(it != webViewHandlers.end())
    {
        auto range = it->second.equal_range(name);

        for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);
    }

    return handlers;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetWebSocketClientHandlers(alt::Ref<alt::IWebSocketClient> webSocket, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = webSocketClientHandlers.find(webSocket.Get());

    if(it != webSocketClientHandlers.end())
    {
        auto range = it->second.equal_range(name);

        for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);
    }

    return handlers;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetAudioHandlers(alt::Ref<alt::IAudio> audio, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = audioHandlers.find(audio.Get());

    if(it != audioHandlers.end())
    {
        auto range = it->second.equal_range(name);

        for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);
    }

    return handlers;
}

void CV8ResourceImpl::OnTick()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Context::Scope scope(GetContext());

    microtaskQueue->PerformCheckpoint(isolate);

    int64_t time = GetTime();

    V8ResourceImpl::OnTick();

    if(GetTime() - time > 10)
    {
        Log::Warning << "Resource " << resource->GetName() << " tick was too long " << GetTime() - time << " ms" << Log::Endl;
    }

    for(auto& view : webViewHandlers)
    {
        for(auto it = view.second.begin(); it != view.second.end();)
        {
            if(it->second.removed) it = view.second.erase(it);
            else
                ++it;
        }
    }

    for(auto& webSocket : webSocketClientHandlers)
    {
        for(auto it = webSocket.second.begin(); it != webSocket.second.end();)
        {
            if(it->second.removed) it = webSocket.second.erase(it);
            else
                ++it;
        }
    }

    for(auto worker : workers)
    {
        worker->HandleMainEventQueue();
    }

    promiseRejections.ProcessQueue(this);
}

void CV8ResourceImpl::OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage& data)
{
    promiseRejections.RejectedWithNoHandler(this, data);
}

void CV8ResourceImpl::OnPromiseHandlerAdded(v8::PromiseRejectMessage& data)
{
    promiseRejections.HandlerAdded(this, data);
}

void CV8ResourceImpl::AddWorker(CWorker* worker)
{
    workers.insert(worker);
}

void CV8ResourceImpl::RemoveWorker(CWorker* worker)
{
    workers.erase(worker);
}

void CV8ResourceImpl::HandleWebViewEventQueue(const alt::Ref<alt::IWebView> view)
{
    auto& eventQueuesMap = this->GetWebviewsEventQueue();
    if(!eventQueuesMap.count(view)) return;

    auto& eventQueue = eventQueuesMap.at(view);
    if(eventQueue.empty()) return;

    for(auto& [evName, mvArgs] : eventQueue)
    {
        view->Trigger(evName, mvArgs);
    }

    eventQueuesMap.erase(view);
}
