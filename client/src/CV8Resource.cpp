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
#include "cpp-sdk/events/CResourceStartEvent.h"
#include "cpp-sdk/events/CResourceStopEvent.h"
#include "cpp-sdk/events/CResourceErrorEvent.h"
#include "cpp-sdk/SDK.h"

#include "CV8ScriptRuntime.h"
#include "CV8Resource.h"
#include "V8Helpers.h"
#include "V8Module.h"

#include "workers/CWorker.h"

#include "JSBindings.h"

extern void StaticRequire(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    IImportHandler* handler = nullptr;
    bool isWorker = *(bool*)isolate->GetData(isolate->GetNumberOfDataSlots() - 1);
    if(isWorker)
    {
        CWorker* worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));
        handler = worker;
    }
    else
        handler = static_cast<CV8ResourceImpl*>(resource);

    v8::MaybeLocal<v8::Value> _exports = handler->Require(name);

    if(!_exports.IsEmpty()) info.GetReturnValue().Set(_exports.ToLocalChecked());
    else
        V8Helpers::Throw(isolate, "No such module " + name);
}

void StartFile(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, path);
    v8::MaybeLocal<v8::Module> maybeMod = static_cast<CV8ResourceImpl*>(resource)->ResolveFile(path, v8::Local<v8::Module>(), resource->GetResource());
    V8_CHECK(!maybeMod.IsEmpty(), "Failed to start file");
    auto mod = maybeMod.ToLocalChecked();
    static_cast<CV8ResourceImpl*>(resource)->InstantiateModule(mod);
    alt::MValueDict exports = V8Helpers::V8ToMValue(mod->GetModuleNamespace()).As<alt::IMValueDict>();
    resource->GetResource()->SetExports(exports);
}

void CV8ResourceImpl::ProcessDynamicImports()
{
    if(dynamicImports.empty()) return;
    for(auto& importFn : dynamicImports)
    {
        importFn();
    }
    dynamicImports.clear();
}

extern std::string bootstrap_code =
#include "bootstrap.js.gen"
  ;

extern V8Module altModule;
bool CV8ResourceImpl::Start()
{
    if(resource->GetMain().empty()) return false;

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
    V8ResourceImpl::SetupScriptGlobals();

    v8::Context::Scope context_scope(ctx);

    std::string path = resource->GetMain();
    Log::Info << "[V8] Starting script " << path << Log::Endl;

    bool result = V8Helpers::TryCatch(
      [&]()
      {
          v8::MaybeLocal<v8::Module> maybeModule;
          v8::ScriptOrigin scriptOrigin(isolate, V8Helpers::JSValue("<bootstrapper>"), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());
          v8::ScriptCompiler::Source source{ V8Helpers::JSValue(bootstrap_code), scriptOrigin };
          maybeModule = v8::ScriptCompiler::CompileModule(isolate, &source);

          if(maybeModule.IsEmpty()) return false;

          v8::Local<v8::Module> curModule = maybeModule.ToLocalChecked();

          auto exports = altModule.GetExports(isolate, ctx);
          // Overwrite global console object
          auto console = ctx->Global()->Get(ctx, V8Helpers::JSValue("console")).ToLocalChecked().As<v8::Object>();
          if(!console.IsEmpty())
          {
              console->Set(ctx, V8Helpers::JSValue("log"), exports->Get(ctx, V8Helpers::JSValue("log")).ToLocalChecked());
              console->Set(ctx, V8Helpers::JSValue("warn"), exports->Get(ctx, V8Helpers::JSValue("logWarning")).ToLocalChecked());
              console->Set(ctx, V8Helpers::JSValue("error"), exports->Get(ctx, V8Helpers::JSValue("logError")).ToLocalChecked());
              console->Set(ctx, V8Helpers::JSValue("time"), exports->Get(ctx, V8Helpers::JSValue("time")).ToLocalChecked());
              console->Set(ctx, V8Helpers::JSValue("timeEnd"), exports->Get(ctx, V8Helpers::JSValue("timeEnd")).ToLocalChecked());
          }

          // Add global timer funcs
          ctx->Global()->Set(ctx, V8Helpers::JSValue("setInterval"), exports->Get(ctx, V8Helpers::JSValue("setInterval")).ToLocalChecked());
          ctx->Global()->Set(ctx, V8Helpers::JSValue("setTimeout"), exports->Get(ctx, V8Helpers::JSValue("setTimeout")).ToLocalChecked());
          ctx->Global()->Set(ctx, V8Helpers::JSValue("clearInterval"), exports->Get(ctx, V8Helpers::JSValue("clearInterval")).ToLocalChecked());
          ctx->Global()->Set(ctx, V8Helpers::JSValue("clearTimeout"), exports->Get(ctx, V8Helpers::JSValue("clearTimeout")).ToLocalChecked());

          ctx->Global()->Set(ctx, V8Helpers::JSValue("__internal_get_exports"), v8::Function::New(ctx, &StaticRequire).ToLocalChecked());
          ctx->Global()->Set(ctx, V8Helpers::JSValue("__internal_bindings_code"), V8Helpers::JSValue(JSBindings::GetBindingsCode()));
          ctx->Global()->Set(ctx, V8Helpers::JSValue("__internal_main_path"), V8Helpers::JSValue(path));
          ctx->Global()->Set(ctx, V8Helpers::JSValue("__internal_start_file"), v8::Function::New(ctx, &StartFile).ToLocalChecked());

          bool result = InstantiateModule(curModule);
          if(!result) return false;

          Log::Info << "[V8] Started script " << path << Log::Endl;
          return true;
      });

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
    if(!context.IsEmpty())
    {
        auto nscope = resource->PushNativesScope();

        v8::Locker locker(isolate);
        v8::Isolate::Scope isolateScope(isolate);
        v8::HandleScope handleScope(isolate);
        auto ctx = GetContext();
        v8::Context::Scope scope(ctx);

        auto resources = static_cast<CV8ScriptRuntime*>(resource->GetRuntime())->GetResources();
        auto name = this->resource->GetName();
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

    std::vector<alt::IBaseObject*> objects(ownedObjects.size());

    for(auto handle : ownedObjects) objects.push_back(handle);

    ownedObjects.clear();

    // ?????
    // for (auto handle : objects)
    // {
    // 	CGame::Instance().DestroyBaseObject(handle);
    // }

    // runtime->GetInspector()->contextDestroyed(context.Get(isolate));

    for(auto worker : workers)
    {
        worker->Destroy();
    }
    workers.clear();

    webViewHandlers.clear();
    webSocketClientHandlers.clear();
    audioHandlers.clear();
    rmlHandlers.clear();
    localStorage.Reset();
    syntheticModuleExports.clear();
    promiseRejections.ClearQueue();
    dynamicImports.clear();
    modules.clear();
    requiresMap.clear();

    isPreloading = true;

    V8ResourceImpl::Stop();

    return true;
}

void CV8ResourceImpl::OnEvent(const alt::CEvent* e)
{
    auto nscope = resource->PushNativesScope();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());

    V8Helpers::EventHandler* handler = V8Helpers::EventHandler::Get(e);
    if(!handler) return;

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
                eventName = static_cast<const alt::CClientScriptEvent*>(e)->GetName().c_str();
            }
            else if(evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(false));
                eventName = static_cast<const alt::CServerScriptEvent*>(e)->GetName().c_str();
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
            ProcessDynamicImports();
            runtime.resourcesLoaded = true;
        }
        else if(e->GetType() == alt::CEvent::Type::DISCONNECT_EVENT)
        {
            CV8ScriptRuntime& runtime = CV8ScriptRuntime::Instance();
            runtime.resourcesLoaded = false;
        }
    }

    return;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetWebViewHandlers(alt::IWebView* view, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = webViewHandlers.find(view);

    if(it != webViewHandlers.end())
    {
        auto range = it->second.equal_range(name);

        for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);
    }

    return handlers;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetWebSocketClientHandlers(alt::IWebSocketClient* webSocket, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = webSocketClientHandlers.find(webSocket);

    if(it != webSocketClientHandlers.end())
    {
        auto range = it->second.equal_range(name);

        for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);
    }

    return handlers;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetAudioHandlers(alt::IAudio* audio, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = audioHandlers.find(audio);

    if(it != audioHandlers.end())
    {
        auto range = it->second.equal_range(name);

        for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);
    }

    return handlers;
}

std::vector<V8Helpers::EventCallback*> CV8ResourceImpl::GetRmlHandlers(alt::IRmlElement* element, const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto it = rmlHandlers.find(element);

    if(it != rmlHandlers.end())
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

    for(auto& audio : audioHandlers)
    {
        for(auto it = audio.second.begin(); it != audio.second.end();)
        {
            if(it->second.removed) it = audio.second.erase(it);
            else
                ++it;
        }
    }

    for(auto& rml : rmlHandlers)
    {
        for(auto it = rml.second.begin(); it != rml.second.end();)
        {
            if(it->second.removed) it = rml.second.erase(it);
            else
                ++it;
        }
    }

    for(auto worker : workers)
    {
        worker->GetMainEventHandler().Process();
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

// *** Oh god is this horrible, but V8 is holding me hostage by not allowing
// *** the embedder to use a lambda function, because we are apparently still
// *** living in 2012 so we have to do this HORRIBLE way of geting the actual
// *** export for synthetic modules, instead of just using a lambda
// TODO: pray that v8 some day switches to using lambdas
v8::MaybeLocal<v8::Value> EvaluateSyntheticModule(v8::Local<v8::Context> context, v8::Local<v8::Module> syntheticModule)
{
    CV8ResourceImpl* resource = static_cast<CV8ResourceImpl*>(V8ResourceImpl::Get(context));
    v8::MaybeLocal<v8::Value> maybeModuleExport = resource->GetSyntheticModuleExport(syntheticModule);
    v8::Local<v8::Value> moduleExport;
    if(!maybeModuleExport.ToLocal(&moduleExport)) return v8::MaybeLocal<v8::Value>();
    syntheticModule->SetSyntheticModuleExport(v8::Isolate::GetCurrent(), V8Helpers::JSValue("default"), moduleExport);
    v8::Local<v8::Promise::Resolver> resolver = v8::Promise::Resolver::New(context).ToLocalChecked();
    resolver->Resolve(context, v8::Undefined(context->GetIsolate()));
    return resolver->GetPromise();
}
v8::Local<v8::Module> CV8ResourceImpl::CreateSyntheticModule(const std::string& name, v8::Local<v8::Value> exportValue)
{
    std::vector<v8::Local<v8::String>> exports = { V8Helpers::JSValue("default") };
    v8::Local<v8::Module> syntheticModule = v8::Module::CreateSyntheticModule(isolate, V8Helpers::JSValue(name), exports, &EvaluateSyntheticModule);
    syntheticModuleExports.insert({ syntheticModule->GetIdentityHash(), V8Helpers::CPersistent<v8::Value>(isolate, exportValue) });
    return syntheticModule;
}
v8::MaybeLocal<v8::Value> CV8ResourceImpl::GetSyntheticModuleExport(v8::Local<v8::Module> syntheticModule)
{
    if(!syntheticModule->IsSyntheticModule()) return v8::MaybeLocal<v8::Value>();
    auto result = syntheticModuleExports.find(syntheticModule->GetIdentityHash());
    if(result != syntheticModuleExports.end()) return result->second.Get(isolate);
    return v8::MaybeLocal<v8::Value>();
}

bool CV8ResourceImpl::InstantiateModule(v8::Local<v8::Module> mod, bool preload)
{
    bool res = mod->InstantiateModule(GetContext(), CV8ScriptRuntime::ResolveModule).IsJust();

    if(!res) return false;

    v8::MaybeLocal<v8::Value> v = mod->Evaluate(GetContext());
    if(v.IsEmpty()) return false;

    if(preload) isPreloading = false;

    v8::Local<v8::Promise> modulePromise = v.ToLocalChecked().As<v8::Promise>();
    int64_t start = GetTime();
    while(modulePromise->State() == v8::Promise::kPending)
    {
        if(GetTime() > start + 5000)
        {
            Log::Error << "[V8] Resource start timed out (broken top-level await statements?)" << Log::Endl;
            return false;
        }
        OnTick();
    }

    if(mod->GetStatus() == v8::Module::Status::kErrored)
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
    return true;
}
