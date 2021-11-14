
#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

#include "V8ResourceImpl.h"

#ifdef ALT_SERVER_API
    #include "CNodeResourceImpl.h"
    #include "node.h"
#endif

using namespace alt;

V8ResourceImpl::~V8ResourceImpl()
{
    for(auto& [obj, ent] : entities)
    {
        delete ent;
    }

    entities.clear();
}

extern V8Class v8Vector3, v8Vector2, v8RGBA, v8BaseObject;
bool V8ResourceImpl::Start()
{
    vector3Class.Reset(isolate, v8Vector3.JSValue(isolate, GetContext()));
    vector2Class.Reset(isolate, v8Vector2.JSValue(isolate, GetContext()));
    rgbaClass.Reset(isolate, v8RGBA.JSValue(isolate, GetContext()));
    baseObjectClass.Reset(isolate, v8BaseObject.JSValue(isolate, GetContext()));

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
        int64_t time = GetTime();

        if(!p.second->Update(time)) RemoveTimer(p.first);

        if(GetTime() - time > 10)
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

    for(auto it = localGenericHandlers.begin(); it != localGenericHandlers.end(); it++)
    {
        if(it->removed) localGenericHandlers.erase(it);
    }

    for(auto it = remoteGenericHandlers.begin(); it != remoteGenericHandlers.end(); it++)
    {
        if(it->removed) remoteGenericHandlers.erase(it);
    }

    promiseRejections.ProcessQueue(this);
}

void V8ResourceImpl::BindEntity(v8::Local<v8::Object> val, alt::Ref<alt::IBaseObject> handle)
{
    V8Entity* ent = new V8Entity(GetContext(), V8Entity::GetClass(handle), val, handle);
    entities.insert({ handle.Get(), ent });
}

v8::Local<v8::Value> V8ResourceImpl::GetBaseObjectOrNull(alt::IBaseObject* handle)
{
    if(handle == nullptr)
    {
        return v8::Null(isolate);
    }
    else
    {
        return GetOrCreateEntity(handle)->GetJSVal(isolate);
    }
}

v8::Local<v8::Value> V8ResourceImpl::CreateVector3(alt::Vector3f vec)
{
    std::vector<v8::Local<v8::Value>> args{ V8::JSValue(vec[0]), V8::JSValue(vec[1]), V8::JSValue(vec[2]) };

    return v8Vector3.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> V8ResourceImpl::CreateVector2(alt::Vector2f vec)
{
    std::vector<v8::Local<v8::Value>> args{ V8::JSValue(vec[0]), V8::JSValue(vec[1]) };

    return v8Vector2.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> V8ResourceImpl::CreateRGBA(alt::RGBA rgba)
{
    std::vector<v8::Local<v8::Value>> args{ V8::JSValue(rgba.r), V8::JSValue(rgba.g), V8::JSValue(rgba.b), V8::JSValue(rgba.a) };

    return V8::New(isolate, GetContext(), rgbaClass.Get(isolate), args);
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

void V8ResourceImpl::OnCreateBaseObject(alt::Ref<alt::IBaseObject> handle)
{
    Log::Debug << "OnCreateBaseObject " << handle.Get() << " " << (entities.find(handle.Get()) != entities.end()) << Log::Endl;

    /*if (entities.find(handle.Get()) == entities.end())
    {
            v8::Locker locker(isolate);
            v8::Isolate::Scope isolateScope(isolate);
            v8::HandleScope handleScope(isolate);

            v8::Context::Scope scope(GetContext());
            CreateEntity(handle.Get());
    }*/

    NotifyPoolUpdate(handle.Get());
}

void V8ResourceImpl::OnRemoveBaseObject(alt::Ref<alt::IBaseObject> handle)
{
    NotifyPoolUpdate(handle.Get());

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);
    v8::Context::Scope scope(GetContext());

    V8Entity* ent = GetEntity(handle.Get());

    if(!ent) return;

    entities.erase(handle.Get());

    // TODO: ent->SetWeak();
    ent->GetJSVal(isolate)->SetInternalField(0, v8::External::New(isolate, nullptr));
    delete ent;
}

void V8ResourceImpl::NotifyPoolUpdate(alt::IBaseObject* ent)
{
    switch(ent->GetType())
    {
        case alt::IBaseObject::Type::PLAYER: playerPoolDirty = true; break;
        case alt::IBaseObject::Type::VEHICLE: vehiclePoolDirty = true; break;
    }
}

v8::Local<v8::Array> V8ResourceImpl::GetAllPlayers()
{
    if(playerPoolDirty)
    {
        playerPoolDirty = false;

        Array<Ref<IPlayer>> all = ICore::Instance().GetPlayers();
        v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

        for(uint32_t i = 0; i < all.GetSize(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

        players.Reset(isolate, jsAll);
        return jsAll;
    }

    return players.Get(isolate);
}

v8::Local<v8::Array> V8ResourceImpl::GetAllBlips()
{
    Array<Ref<IBlip>> all = ICore::Instance().GetBlips();
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

    for(uint32_t i = 0; i < all.GetSize(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

    return jsAll;
}

v8::Local<v8::Array> V8ResourceImpl::GetAllVehicles()
{
    if(vehiclePoolDirty)
    {
        vehiclePoolDirty = false;

        Array<Ref<IVehicle>> all = ICore::Instance().GetVehicles();
        v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

        for(uint32_t i = 0; i < all.GetSize(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

        vehicles.Reset(isolate, jsAll);
        return jsAll;
    }

    return vehicles.Get(isolate);
}

std::vector<V8::EventCallback*> V8ResourceImpl::GetLocalHandlers(const std::string& name)
{
    std::vector<V8::EventCallback*> handlers;
    auto range = localHandlers.equal_range(name);

    for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);

    return handlers;
}

std::vector<V8::EventCallback*> V8ResourceImpl::GetRemoteHandlers(const std::string& name)
{
    std::vector<V8::EventCallback*> handlers;
    auto range = remoteHandlers.equal_range(name);

    for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);

    return handlers;
}

std::vector<V8::EventCallback*> V8ResourceImpl::GetGenericHandlers(bool local)
{
    std::vector<V8::EventCallback*> handlers;
    if(local)
    {
        for(auto& it : localGenericHandlers)
        {
            handlers.push_back(&it);
        }
    }
    else
    {
        for(auto& it : remoteGenericHandlers)
        {
            handlers.push_back(&it);
        }
    }
    return handlers;
}

void V8ResourceImpl::InvokeEventHandlers(const alt::CEvent* ev, const std::vector<V8::EventCallback*>& handlers, std::vector<v8::Local<v8::Value>>& args)
{
    for(auto handler : handlers)
    {
        int64_t time = GetTime();

        if(handler->removed) continue;

        V8Helpers::TryCatch([&] {
            v8::MaybeLocal<v8::Value> retn = handler->fn.Get(isolate)->Call(GetContext(), v8::Undefined(isolate), args.size(), args.data());

            if(retn.IsEmpty()) return false;

            if(ev && retn.ToLocalChecked()->IsFalse()) ev->Cancel();

            return true;
        });

        if(GetTime() - time > 5)
        {
            if(handler->location.GetLineNumber() != 0)
            {
                Log::Warning << "Event handler at " << resource->GetName() << ":" << handler->location.GetFileName() << ":" << handler->location.GetLineNumber() << " was too long "
                             << (GetTime() - time) << "ms" << Log::Endl;
            }
            else
            {
                Log::Warning << "Event handler at " << resource->GetName() << ":" << handler->location.GetFileName() << " was too long " << (GetTime() - time) << "ms" << Log::Endl;
            }
        }

        if(handler->once) handler->removed = true;
    }
}

alt::MValue V8ResourceImpl::FunctionImpl::Call(alt::MValueArgs args) const
{
    v8::Isolate* isolate = resource->GetIsolate();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Context> ctx = resource->GetContext();
    v8::Context::Scope scope(ctx);

#ifdef ALT_SERVER_API
    CNodeResourceImpl* nodeRes = static_cast<CNodeResourceImpl*>(resource);
    if(!nodeRes->IsEnvStarted())
    {
        V8Helpers::Throw(isolate, "Tried to call exported function of invalid resource");
        return alt::ICore::Instance().CreateMValueNone();
    }
    node::CallbackScope callbackScope(isolate, nodeRes->GetAsyncResource(), nodeRes->GetAsyncContext());
#endif  // ALT_SERVER_API

    std::vector<v8::Local<v8::Value>> v8Args;
    V8Helpers::MValueArgsToV8(args, v8Args);

    alt::MValue res;
    V8Helpers::TryCatch([&] {
        v8::MaybeLocal<v8::Value> _res = function.Get(isolate)->CallAsFunction(resource->GetContext(), v8::Undefined(isolate), v8Args.size(), v8Args.data());

        if(_res.IsEmpty()) return false;

        res = V8Helpers::V8ToMValue(_res.ToLocalChecked());
        return true;
    });

    if(res.IsEmpty()) res = alt::ICore::Instance().CreateMValueNone();

    return res;
}
