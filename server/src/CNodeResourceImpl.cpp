#include "stdafx.h"

#include "CNodeResourceImpl.h"
#include "CNodeScriptRuntime.h"
#include "V8Module.h"
#include "V8Helpers.h"

#include "JSBindings.h"

static void ResourceLoaded(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, name);

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);
    if(resource && resource->GetType() == "js")
    {
        CNodeResourceImpl* _resource = static_cast<CNodeResourceImpl*>(resource->GetImpl());
        _resource->Started(info[1]);
    }
}

static const char bootstrap_code[] =
#include "bootstrap.js.gen"
  ;

bool CNodeResourceImpl::Start()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::ObjectTemplate> global = v8::ObjectTemplate::New(isolate);

    v8::Local<v8::String> resourceName = V8Helpers::JSValue(resource->GetName());

    v8::Local<v8::Context> _context = node::NewContext(isolate, global);
    v8::Context::Scope scope(_context);

    _context->Global()->Set(_context, V8Helpers::JSValue("__resourceLoaded"), v8::Function::New(_context, &ResourceLoaded).ToLocalChecked());
    _context->Global()->Set(_context, V8Helpers::JSValue("__internal_bindings_code"), V8Helpers::JSValue(JSBindings::GetBindingsCode()));

    _context->SetAlignedPointerInEmbedderData(1, resource);
    context.Reset(isolate, _context);

    V8ResourceImpl::Start();
    V8ResourceImpl::SetupScriptGlobals();

    node::EnvironmentFlags::Flags flags = (node::EnvironmentFlags::Flags)(node::EnvironmentFlags::kOwnsProcessState & node::EnvironmentFlags::kNoCreateInspector);

    uvLoop = new uv_loop_t;
    uv_loop_init(uvLoop);

    nodeData = node::CreateIsolateData(isolate, uvLoop, runtime->GetPlatform());
    std::vector<std::string> argv = { "altv-resource" };
    env = node::CreateEnvironment(nodeData, _context, argv, argv, flags);

    node::IsolateSettings is;
    node::SetIsolateUpForNode(isolate, is);

    node::LoadEnvironment(env, bootstrap_code);

    asyncResource.Reset(isolate, v8::Object::New(isolate));
    asyncContext = node::EmitAsyncInit(isolate, asyncResource.Get(isolate), "CNodeResourceImpl");

    while(!envStarted && !startError)
    {
        runtime->OnTick();
        OnTick();
    }

    DispatchStartEvent(startError);

    return !startError;
}

bool CNodeResourceImpl::Stop()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    {
        v8::Context::Scope scope(GetContext());
        DispatchStopEvent();

        node::EmitAsyncDestroy(isolate, asyncContext);
        asyncResource.Reset();
    }

    node::EmitProcessBeforeExit(env);
    node::EmitProcessExit(env);

    V8ResourceImpl::Stop();

    node::Stop(env);

    node::FreeEnvironment(env);
    node::FreeIsolateData(nodeData);

    envStarted = false;

    uv_loop_close(uvLoop);
    delete uvLoop;

    connectionInfoMap.clear();

    return true;
}

void CNodeResourceImpl::Started(v8::Local<v8::Value> _exports)
{
    if(!_exports->IsNullOrUndefined())
    {
        alt::MValueDict exports = V8Helpers::V8ToMValue(_exports).As<alt::IMValueDict>();
        resource->SetExports(exports);
        envStarted = true;
    }
    else
    {
        startError = true;
    }
}

void CNodeResourceImpl::OnEvent(const alt::CEvent* e)
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());
    // env->PushAsyncCallbackScope();

    V8Helpers::EventHandler* handler = V8Helpers::EventHandler::Get(e);
    if(!handler) return;

    // Generic event handler
    {
        auto evType = e->GetType();
        if(evType == alt::CEvent::Type::CLIENT_SCRIPT_EVENT || evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
        {
            std::vector<V8Helpers::EventCallback*> callbacks;
            const char* eventName;

            if(evType == alt::CEvent::Type::SERVER_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(true));
                eventName = static_cast<const alt::CServerScriptEvent*>(e)->GetName().c_str();
            }
            else if(evType == alt::CEvent::Type::CLIENT_SCRIPT_EVENT)
            {
                callbacks = std::move(GetGenericHandlers(false));
                eventName = static_cast<const alt::CClientScriptEvent*>(e)->GetName().c_str();
            }

            if(callbacks.size() != 0)
            {
                auto evArgs = handler->GetArgs(this, e);
                evArgs.insert(evArgs.begin(), V8Helpers::JSValue(eventName));

                node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);
                InvokeEventHandlers(e, callbacks, evArgs);
            }
        }
    }

    std::vector<V8Helpers::EventCallback*> callbacks = handler->GetCallbacks(this, e);
    if(callbacks.size() > 0)
    {
        std::vector<v8::Local<v8::Value>> args = handler->GetArgs(this, e);

        node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);
        InvokeEventHandlers(e, callbacks, args);
    }

    // env->PopAsyncCallbackScope();
    return;
}

void CNodeResourceImpl::OnTick()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());
    node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);

    uv_run(uvLoop, UV_RUN_NOWAIT);
    V8ResourceImpl::OnTick();
}

bool CNodeResourceImpl::MakeClient(alt::IResource::CreationInfo* info, alt::Array<std::string>)
{
    if(resource->GetClientType() == "jsb") info->type = "js";
    return true;
}
