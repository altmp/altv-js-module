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

CNodeResourceImpl::CNodeResourceImpl(CNodeScriptRuntime* _runtime, alt::IResource* resource) : V8ResourceImpl(nullptr, resource), runtime(_runtime)
{
    uvLoop = new uv_loop_t;
    uv_loop_init(uvLoop);

    auto allocator = node::CreateArrayBufferAllocator();
    isolate = node::NewIsolate(allocator, uvLoop, runtime->GetPlatform());
    nodeData = node::CreateIsolateData(isolate, uvLoop, runtime->GetPlatform(), allocator);

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Context> _context = node::NewContext(isolate);
    _context->SetAlignedPointerInEmbedderData(1, resource);
    context.Reset(isolate, _context);
}

bool CNodeResourceImpl::Start()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    auto _context = GetContext();
    v8::Context::Scope scope(_context);

    _context->Global()->Set(_context, V8Helpers::JSValue("__resourceLoaded"), v8::Function::New(_context, &ResourceLoaded).ToLocalChecked());
    _context->Global()->Set(_context, V8Helpers::JSValue("__internal_bindings_code"), V8Helpers::JSValue(JSBindings::GetBindingsCode()));

    V8Class::LoadAll(isolate);
    V8ResourceImpl::Start();
    V8ResourceImpl::SetupScriptGlobals();

    node::ThreadId threadId = node::AllocateEnvironmentThreadId();
    auto flags = static_cast<node::EnvironmentFlags::Flags>(node::EnvironmentFlags::kNoFlags);
    auto inspector = node::GetInspectorParentHandle(runtime->GetParentEnv(), threadId, resource->GetName().c_str());

    std::vector<std::string> args{ resource->GetName() };
    std::vector<std::string> execArgs{ };

    env = node::CreateEnvironment(nodeData, _context, args, execArgs, flags, threadId, std::move(inspector));        
    node::LoadEnvironment(env, bootstrap_code);

    // Not sure it's needed anymore
    asyncResource.Reset(isolate, v8::Object::New(isolate));
    asyncContext = node::EmitAsyncInit(isolate, asyncResource.Get(isolate), "CNodeResourceImpl");

    while(!envStarted && !startError)
    {
        runtime->OnTick();
        OnTick();
    }

    Log::Debug << "Started" << Log::Endl;
    DispatchStartEvent(startError);

    return !startError;
}

bool CNodeResourceImpl::Stop()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    Log::Debug << "Before stop" << Log::Endl;

    {
        v8::Context::Scope scope(GetContext());
        DispatchStopEvent();

        node::EmitAsyncDestroy(isolate, asyncContext);
        asyncResource.Reset();
    }

    Log::Debug << "After stop" << Log::Endl;

    node::EmitProcessBeforeExit(env);
    node::EmitProcessExit(env);

    V8ResourceImpl::Stop();

    node::Stop(env);

    node::FreeEnvironment(env);
    node::FreeIsolateData(nodeData);

    envStarted = false;

    uv_loop_close(uvLoop);
    delete uvLoop;

    vehiclePassengers.clear();

    rpcHandlers.clear();
    remoteRPCHandlers.clear();
    awaitableRPCHandlers.clear();

    return true;
}

void CNodeResourceImpl::Started(v8::Local<v8::Value> _exports)
{
    if(!_exports->IsNullOrUndefined())
    {
        alt::MValueDict exports = std::dynamic_pointer_cast<alt::IMValueDict>(V8Helpers::V8ToMValue(_exports));
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

    HandleVehiclePassengerSeatEvents(e);

    if (e->GetType() == alt::CEvent::Type::SCRIPT_RPC_EVENT)
    {
        HandleClientRpcEvent((alt::CScriptRPCEvent*)e);
    }
    else if (e->GetType() == alt::CEvent::Type::SCRIPT_RPC_ANSWER_EVENT)
    {
        auto ev = static_cast<const alt::CScriptRPCAnswerEvent*>(e);
        HandleClientRpcAnswerEvent(ev);
    }
    else if (e->GetType() == alt::CEvent::Type::DISCONNECT_EVENT)
    {
        auto ev = static_cast<const alt::CPlayerDisconnectEvent*>(e);
        auto player = ev->GetTarget();

        remoteRPCHandlers.erase(player);

        for (auto it = awaitableRPCHandlers.rbegin(); it != awaitableRPCHandlers.rend(); ++it)
        {
            if (player == it->Player)
                awaitableRPCHandlers.erase(std::next(it).base());
        }
    }

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

void CNodeResourceImpl::HandleVehiclePassengerSeatEvents(const alt::CEvent* ev)
{
    auto evType = ev->GetType();

    if (evType == alt::CEvent::Type::PLAYER_ENTER_VEHICLE)
    {
        auto event = static_cast<const alt::CPlayerEnterVehicleEvent*>(ev);
        auto vehicle = event->GetTarget();
        auto player = event->GetPlayer();
        auto seat = event->GetSeat();

        if (!vehiclePassengers.contains(vehicle))
            vehiclePassengers[vehicle] = {};

        vehiclePassengers[vehicle][seat] = player;
    }
    else if (evType == alt::CEvent::Type::PLAYER_LEAVE_VEHICLE)
    {
        auto event = static_cast<const alt::CPlayerLeaveVehicleEvent*>(ev);
        auto vehicle = event->GetTarget();
        auto player = event->GetPlayer();

        if (vehiclePassengers.contains(vehicle))
        {
            for ( auto it = vehiclePassengers[vehicle].begin(); it != vehiclePassengers[vehicle].end(); )
            {
                if (it->second == player)
                    it = vehiclePassengers[vehicle].erase(it);
                else
                    ++it;
            }

            if (vehiclePassengers[vehicle].empty())
                vehiclePassengers.erase(vehicle);
        }
    }
    else if (evType == alt::CEvent::Type::PLAYER_CHANGE_VEHICLE_SEAT)
    {
        auto event = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(ev);
        auto vehicle = event->GetTarget();

        if (!vehiclePassengers.contains(vehicle))
            vehiclePassengers[vehicle] = {};

        vehiclePassengers[vehicle].erase(event->GetOldSeat());
        vehiclePassengers[vehicle][event->GetNewSeat()] = event->GetPlayer();
    }
}

void CNodeResourceImpl::HandleClientRpcEvent(alt::CScriptRPCEvent* ev)
{
    auto handler = rpcHandlers.find(ev->GetName());

    if (handler == rpcHandlers.end())
        return;

    auto context = GetContext();
    auto isolate = GetIsolate();

    std::vector<v8::Local<v8::Value>> args;
    args.push_back(GetBaseObjectOrNull(ev->GetTarget()));
    V8Helpers::MValueArgsToV8(ev->GetArgs(), args);

    auto result = V8Helpers::CallFunctionWithTimeout(handler->second.Get(isolate), context, args);

    v8::Local<v8::Value> returnValue;
    if (!result.IsEmpty())
        returnValue = result.ToLocalChecked();
    else
        returnValue = v8::Undefined(isolate);

    if (returnValue->IsPromise())
    {
        ev->WillAnswer();
        awaitableRPCHandlers.push_back({ ev->GetTarget(), ev->GetAnswerID(), v8::Global<v8::Promise>(isolate, returnValue.As<v8::Promise>()) });
        return;
    }

    // Retrieve returned error message when an error was returned
    if (returnValue->IsNativeError())
    {
        v8::Local<v8::Value> exception = returnValue.As<v8::Value>();

        v8::String::Utf8Value messageValue(isolate, exception->ToString(isolate->GetCurrentContext()).ToLocalChecked());
        std::string errorMessage = *messageValue;

        // Strip exception prefix
        if (size_t colonPos = errorMessage.find(':'); colonPos != std::string::npos)
            errorMessage = errorMessage.substr(colonPos + 2);

        ev->AnswerWithError(errorMessage);
        return;
    }

    ev->Answer(V8Helpers::V8ToMValue(returnValue));
}

void CNodeResourceImpl::HandleClientRpcAnswerEvent(const alt::CScriptRPCAnswerEvent* ev)
{
    auto context = GetContext();
    auto isolate = GetIsolate();

    if (auto resource = Get(isolate->GetEnteredOrMicrotaskContext()); !resource->GetResource()->IsStarted())
        return;

    auto player = ev->GetTarget();
    auto answerId = ev->GetAnswerID();

    if (!remoteRPCHandlers.contains(player))
        return;

    for (auto it = remoteRPCHandlers[player].rbegin(); it != remoteRPCHandlers[player].rend(); ++it)
    {
        if (it->AnswerId != answerId)
            continue;

        if (auto promise = it->PromiseResolver.Get(isolate); promise->IsPromise())
        {
            if (auto errorMessage = ev->GetAnswerError(); !errorMessage.empty())
                promise->Reject(context, V8Helpers::JSValue(errorMessage));
            else
                promise->Resolve(context, V8Helpers::MValueToV8(ev->GetAnswer()));
        }

        remoteRPCHandlers[player].erase(std::next(it).base());
    }

    if (remoteRPCHandlers[player].empty())
        remoteRPCHandlers.erase(player);
}

void CNodeResourceImpl::OnTick()
{
    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Context::Scope scope(GetContext());
    node::CallbackScope callbackScope(isolate, asyncResource.Get(isolate), asyncContext);

    runtime->GetPlatform()->DrainTasks(isolate);
    uv_run(uvLoop, UV_RUN_NOWAIT);
    V8ResourceImpl::OnTick();
}

bool CNodeResourceImpl::MakeClient(alt::IResource::CreationInfo* info, std::vector<std::string>)
{
    if(resource->GetClientType() == "jsb") info->type = "js";
    return true;
}
