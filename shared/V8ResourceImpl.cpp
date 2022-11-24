
#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"
#include "cpp-sdk/events/CPlayerBeforeConnectEvent.h"

#include "V8ResourceImpl.h"

#ifdef ALT_SERVER_API
    #include "CNodeResourceImpl.h"
    #include "CNodeScriptRuntime.h"
    #include "node.h"
#endif

#ifdef ALT_CLIENT_API
    #include "CV8ScriptRuntime.h"
#endif

using namespace alt;

extern V8Class v8Vector3, v8Vector2, v8RGBA, v8BaseObject;
bool V8ResourceImpl::Start()
{
    vector3Class.Reset(isolate, v8Vector3.JSValue(isolate, GetContext()));
    vector2Class.Reset(isolate, v8Vector2.JSValue(isolate, GetContext()));
    rgbaClass.Reset(isolate, v8RGBA.JSValue(isolate, GetContext()));
    baseObjectClass.Reset(isolate, v8BaseObject.JSValue(isolate, GetContext()));

    return true;
}

bool V8ResourceImpl::Stop()
{
    {
        for(auto& handler : localHandlers)
        {
            alt::CEvent::Type type = V8Helpers::EventHandler::GetTypeForEventName(handler.first);
            if(type != alt::CEvent::Type::NONE) IRuntimeEventHandler::Instance().EventHandlerRemoved(type);
        }
    }

    for(auto pair : timers)
    {
        delete pair.second;
    }
    timers.clear();
    for(auto ent : entities)
    {
        delete ent.second;
    }
    entities.clear();
    oldTimers.clear();
    resourceObjects.clear();
    nextTickCallbacks.clear();
    benchmarkTimers.clear();
    resourceObjects.clear();

    localHandlers.clear();
    remoteHandlers.clear();
    localGenericHandlers.clear();
    remoteGenericHandlers.clear();

    players.Reset();
    vehicles.Reset();
    vector3Class.Reset();
    vector2Class.Reset();
    rgbaClass.Reset();
    baseObjectClass.Reset();

    context.Reset();

    return true;
}

void V8ResourceImpl::OnTick()
{
    for(auto& nextTickCb : nextTickCallbacks) nextTickCb();
    nextTickCallbacks.clear();

    for(auto& id : oldTimers) timers.erase(id);

    oldTimers.clear();

    for(auto& p : timers)
    {
        if(std::find(oldTimers.begin(), oldTimers.end(), p.first) != oldTimers.end()) continue;
        int64_t time = GetTime();
        if(!p.second->Update(time)) RemoveTimer(p.first);

        if(GetTime() - time > 50)
        {
            auto& location = p.second->GetLocation();

            if(location.GetLineNumber() != 0)
            {
                Log::Warning << "Timer at " << resource->GetName() << ":" << location.GetFileName() << ":" << location.GetLineNumber() << " was too long " << GetTime() - time << "ms"
                             << Log::Endl;
            }
            else
            {
                Log::Warning << "Timer at " << resource->GetName() << ":" << location.GetFileName() << " was too long " << GetTime() - time << "ms" << Log::Endl;
            }
        }
    }

    for(auto it = localHandlers.begin(); it != localHandlers.end();)
    {
        if(it->second.removed) it = localHandlers.erase(it);
        else
            ++it;
    }

    for(auto it = remoteHandlers.begin(); it != remoteHandlers.end();)
    {
        if(it->second.removed) it = remoteHandlers.erase(it);
        else
            ++it;
    }

    for(auto it = localGenericHandlers.begin(); it != localGenericHandlers.end();)
    {
        if(it->removed) it = localGenericHandlers.erase(it);
        else
            ++it;
    }

    for(auto it = remoteGenericHandlers.begin(); it != remoteGenericHandlers.end();)
    {
        if(it->removed) it = remoteGenericHandlers.erase(it);
        else
            ++it;
    }
}

void V8ResourceImpl::BindEntity(v8::Local<v8::Object> val, alt::IBaseObject* handle)
{
    V8Class* entityClass = V8Entity::GetClass(handle);
    if(!entityClass)
    {
        Log::Error << "Failed to bind entity: Type " << (int)handle->GetType() << " has no class" << Log::Endl;
        return;
    }
    V8Entity* ent = new V8Entity(GetContext(), entityClass, val, handle);
    entities.insert({ handle, ent });
}

v8::Local<v8::Value> V8ResourceImpl::GetBaseObjectOrNull(alt::IBaseObject* handle)
{
    if(handle == nullptr) return v8::Null(isolate);
    else
        return GetOrCreateEntity(handle)->GetJSVal(isolate);
}

v8::Local<v8::Value> V8ResourceImpl::CreateVector3(alt::Vector3f vec)
{
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(vec[0]), V8Helpers::JSValue(vec[1]), V8Helpers::JSValue(vec[2]) };

    return v8Vector3.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> V8ResourceImpl::CreateVector2(alt::Vector2f vec)
{
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(vec[0]), V8Helpers::JSValue(vec[1]) };

    return v8Vector2.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> V8ResourceImpl::CreateRGBA(alt::RGBA rgba)
{
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(rgba.r), V8Helpers::JSValue(rgba.g), V8Helpers::JSValue(rgba.b), V8Helpers::JSValue(rgba.a) };

    return V8Helpers::New(isolate, GetContext(), rgbaClass.Get(isolate), args);
}

bool V8ResourceImpl::IsVector3(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), vector3Class.Get(isolate)).To(&result);
    return result;
}

bool V8ResourceImpl::IsVector2(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), vector2Class.Get(isolate)).To(&result);
    return result;
}

bool V8ResourceImpl::IsRGBA(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), rgbaClass.Get(isolate)).To(&result);
    return result;
}

bool V8ResourceImpl::IsBaseObject(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), baseObjectClass.Get(isolate)).To(&result);
    return result;
}

void V8ResourceImpl::OnCreateBaseObject(alt::IBaseObject* handle)
{
    NotifyPoolUpdate(handle);
}

void V8ResourceImpl::OnRemoveBaseObject(alt::IBaseObject* handle)
{
    NotifyPoolUpdate(handle);

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Context::Scope scope(GetContext());

    V8Entity* ent = GetEntity(handle);
    if(!ent) return;

    auto entityType = handle->GetType();
    if (entityType == alt::IBaseObject::Type::PLAYER
        || entityType == alt::IBaseObject::Type::LOCAL_PLAYER
        || entityType == alt::IBaseObject::Type::VEHICLE
    ) {
        std::vector<V8Helpers::EventCallback*> handlers = GetLocalHandlers("removeEntity");
        std::vector<v8::Local<v8::Value>> args{ ent->GetJSVal(isolate) };
        InvokeEventHandlers(nullptr, handlers, args);
    }

    entities.erase(handle);
    ent->GetJSVal(isolate)->SetInternalField(0, v8::External::New(isolate, nullptr));
    delete ent;
}

void V8ResourceImpl::NotifyPoolUpdate(alt::IBaseObject* ent)
{
    switch(ent->GetType())
    {
        case alt::IBaseObject::Type::PLAYER: playerPoolDirty = true; break;
        case alt::IBaseObject::Type::VEHICLE: vehiclePoolDirty = true; break;
        case alt::IBaseObject::Type::OBJECT: objectPoolDirty = true; break;
    }
}

v8::Local<v8::Array> V8ResourceImpl::GetAllPlayers()
{
    if(playerPoolDirty)
    {
        playerPoolDirty = false;

        Array<IPlayer*> all = ICore::Instance().GetPlayers();
        v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

        for(uint32_t i = 0; i < all.GetSize(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

        players.Reset(isolate, jsAll);
        jsAll->SetIntegrityLevel(GetContext(), v8::IntegrityLevel::kFrozen);
        return jsAll;
    }

    return players.Get(isolate);
}

v8::Local<v8::Array> V8ResourceImpl::GetAllVehicles()
{
    if(vehiclePoolDirty)
    {
        vehiclePoolDirty = false;

        Array<IVehicle*> all = ICore::Instance().GetVehicles();
        v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

        for(uint32_t i = 0; i < all.GetSize(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

        vehicles.Reset(isolate, jsAll);
        jsAll->SetIntegrityLevel(GetContext(), v8::IntegrityLevel::kFrozen);
        return jsAll;
    }
    return vehicles.Get(isolate);
}

v8::Local<v8::Array> V8ResourceImpl::GetAllBlips()
{
    std::vector<IBlip*> all = ICore::Instance().GetBlips();
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.size());

    for(uint32_t i = 0; i < all.size(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

    jsAll->SetIntegrityLevel(GetContext(), v8::IntegrityLevel::kFrozen);
    return jsAll;
}

#ifdef ALT_CLIENT_API
v8::Local<v8::Array> V8ResourceImpl::GetAllObjects()
{
    if(objectPoolDirty)
    {
        objectPoolDirty = false;

        std::vector<IObject*> all = ICore::Instance().GetObjects();
        v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.size());

        for(uint32_t i = 0; i < all.size(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

        objects.Reset(isolate, jsAll);
        jsAll->SetIntegrityLevel(GetContext(), v8::IntegrityLevel::kFrozen);
        return jsAll;
    }
    return objects.Get(isolate);
}
#endif

std::vector<V8Helpers::EventCallback*> V8ResourceImpl::GetLocalHandlers(const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto range = localHandlers.equal_range(name);

    for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);

    return handlers;
}

std::vector<V8Helpers::EventCallback*> V8ResourceImpl::GetRemoteHandlers(const std::string& name)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    auto range = remoteHandlers.equal_range(name);

    for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);

    return handlers;
}

std::vector<V8Helpers::EventCallback*> V8ResourceImpl::GetGenericHandlers(bool local)
{
    std::vector<V8Helpers::EventCallback*> handlers;
    if(local)
        for(auto& it : localGenericHandlers) handlers.push_back(&it);
    else
        for(auto& it : remoteGenericHandlers) handlers.push_back(&it);
    return handlers;
}

extern V8Class v8Resource;
v8::Local<v8::Object> V8ResourceImpl::GetOrCreateResourceObject(alt::IResource* resource)
{
    // If already created return instance
    if(resourceObjects.count(resource) != 0) return resourceObjects.at(resource).Get(isolate);
    // Create instance
    v8::Local<v8::Object> obj = v8Resource.CreateInstance(GetContext());
    obj->SetInternalField(0, v8::External::New(isolate, resource));
    resourceObjects.insert({ resource, V8Helpers::CPersistent<v8::Object>(isolate, obj) });
    return obj;
}

void V8ResourceImpl::DeleteResourceObject(alt::IResource* resource)
{
    if(resourceObjects.count(resource) == 0) return;
    v8::Local<v8::Object> obj = resourceObjects.at(resource).Get(isolate);
    obj->SetInternalField(0, v8::External::New(isolate, nullptr));
    resourceObjects.erase(resource);
}

void V8ResourceImpl::InvokeEventHandlers(const alt::CEvent* ev, const std::vector<V8Helpers::EventCallback*>& handlers, std::vector<v8::Local<v8::Value>>& args, bool waitForPromiseResolve)
{
    for(auto handler : handlers)
    {
        if(handler->removed) continue;
        int64_t time = GetTime();

        V8Helpers::TryCatch(
          [&]
          {
              v8::MaybeLocal<v8::Value> retn = V8Helpers::CallFunctionWithTimeout(handler->fn.Get(isolate), GetContext(), args);
              if(retn.IsEmpty()) return false;

              v8::Local<v8::Value> returnValue = retn.ToLocalChecked();
              if(ev && returnValue->IsFalse()) ev->Cancel();
              else if(ev && ev->GetType() == alt::CEvent::Type::PLAYER_BEFORE_CONNECT && returnValue->IsString())
                  static_cast<alt::CPlayerBeforeConnectEvent*>(const_cast<alt::CEvent*>(ev))->Cancel(*v8::String::Utf8Value(isolate, returnValue));
              // todo: add this once a generic Cancel() with string as arg has been added to the sdk
              // else if(ev && returnValue->IsString())
              //    ev->Cancel(*v8::String::Utf8Value(isolate, returnValue));

              // Wait until the returned promise has been resolved, by continously forcing the event loop to run,
              // until the promise is resolved.
              if(waitForPromiseResolve && returnValue->IsPromise())
              {
                  v8::Local<v8::Promise> promise = returnValue.As<v8::Promise>();
                  while(true)
                  {
                      v8::Promise::PromiseState state = promise->State();
                      if(state == v8::Promise::PromiseState::kPending)
                      {
#ifdef ALT_CLIENT_API
                          CV8ScriptRuntime::Instance().OnTick();
#endif
#ifdef ALT_SERVER_API
                          CNodeScriptRuntime::Instance().OnTick();
#endif
                          // Run event loop
                          OnTick();
                      }
                      else if(state == v8::Promise::PromiseState::kFulfilled)
                      {
                          v8::Local<v8::Value> value = promise->Result();
                          if(value->IsFalse()) ev->Cancel();
                          break;
                      }
                      else if(state == v8::Promise::PromiseState::kRejected)
                      {
                          // todo: we should probably do something with the rejection here
                          break;
                      }
                  }
              }

              return true;
          });

        if(GetTime() - time > 50 && !waitForPromiseResolve)
        {
            if(handler->location.GetLineNumber() != 0)
                Log::Warning << "Event handler at " << resource->GetName() << ":" << handler->location.GetFileName() << ":" << handler->location.GetLineNumber() << " was too long "
                             << (GetTime() - time) << "ms" << Log::Endl;
            else
                Log::Warning << "Event handler at " << resource->GetName() << ":" << handler->location.GetFileName() << " was too long " << (GetTime() - time) << "ms" << Log::Endl;
        }

        if(handler->once) handler->removed = true;
    }
}

// Internal script globals
static void SetLogFunction(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    v8::Local<v8::Value> function = info[0];
    if(!function->IsFunction()) return;
    resource->SetLogFunction(function.As<v8::Function>());
}

// Types:
// 0 = normal
// 1 = warning
// 2 = error
static void PrintLog(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN_MIN(1);

    V8_ARG_TO_INT32(1, type);
    std::stringstream stream;
    for(size_t i = 1; i < info.Length(); i++)
    {
        std::string arg = *v8::String::Utf8Value(isolate, info[i]);
        stream << arg;
        if(i != info.Length() - 1) stream << " ";
    }
    switch(type)
    {
        case 0:
        {
            alt::ICore::Instance().LogColored(stream.str());
            break;
        }
        case 1:
        {
            alt::ICore::Instance().LogWarning(stream.str());
            break;
        }
        case 2:
        {
            alt::ICore::Instance().LogError(stream.str());
            break;
        }
    }
}

void V8ResourceImpl::SetupScriptGlobals()
{
    v8::Local<v8::Context> ctx = context.Get(isolate);

    ctx->Global()->Set(ctx, V8Helpers::JSValue("__setLogFunction"), v8::Function::New(ctx, &::SetLogFunction).ToLocalChecked());
    ctx->Global()->Set(ctx, V8Helpers::JSValue("__printLog"), v8::Function::New(ctx, &::PrintLog).ToLocalChecked());
}

alt::MValue V8ResourceImpl::FunctionImpl::Call(alt::MValueArgs args) const
{
    if(!resource->GetResource()->IsStarted())
    {
        Log::Error << "Tried to call function on resource " << resource->GetResource()->GetName() << " while the resource is stopped" << Log::Endl;
        return alt::ICore::Instance().CreateMValueNone();
    }
    v8::Isolate* isolate = resource->GetIsolate();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Context> ctx = resource->GetContext();
    v8::Context::Scope scope(ctx);

#ifdef ALT_SERVER_API
    CNodeResourceImpl* nodeRes = static_cast<CNodeResourceImpl*>(resource);
    V8_CHECK_RETN(nodeRes->IsEnvStarted(), "Tried to call exported function of invalid resource", alt::ICore::Instance().CreateMValueNone());
    node::CallbackScope callbackScope(isolate, nodeRes->GetAsyncResource(), nodeRes->GetAsyncContext());
#endif  // ALT_SERVER_API

    std::vector<v8::Local<v8::Value>> v8Args;
    V8Helpers::MValueArgsToV8(args, v8Args);

    alt::MValue res;
    V8Helpers::TryCatch(
      [&]
      {
          v8::MaybeLocal<v8::Value> _res = V8Helpers::CallFunctionWithTimeout(function.Get(isolate), resource->GetContext(), v8Args);

          if(_res.IsEmpty()) return false;

          res = V8Helpers::V8ToMValue(_res.ToLocalChecked());
          return true;
      });

    if(res.IsEmpty()) res = alt::ICore::Instance().CreateMValueNone();

    return res;
}
