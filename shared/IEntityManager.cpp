#include "IEntityManager.h"

using namespace alt;

extern V8Class v8Vector3, v8Vector2, v8RGBA, v8BaseObject;

static v8::Isolate* isolate = nullptr;

static inline v8::Local<v8::Context> GetContext()
{
    return isolate->GetEnteredOrMicrotaskContext();
}

IEntityManager::IEntityManager(v8::Isolate* _isolate)
{
    isolate = _isolate;
}

IEntityManager::~IEntityManager()
{
    for(auto& [obj, ent] : entities)
    {
        delete ent;
    }
    entities.clear();
}

void IEntityManager::BindEntity(v8::Local<v8::Object> val, alt::Ref<alt::IBaseObject> handle)
{
    V8Entity* ent = new V8Entity(GetContext(), V8Entity::GetClass(handle), val, handle);
    entities.insert({ handle.Get(), ent });
}

v8::Local<v8::Value> IEntityManager::GetBaseObjectOrNull(alt::IBaseObject* handle)
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

v8::Local<v8::Value> IEntityManager::CreateVector3(alt::Vector3f vec)
{
    std::vector<v8::Local<v8::Value>> args{ V8::JSValue(vec[0]), V8::JSValue(vec[1]), V8::JSValue(vec[2]) };

    return v8Vector3.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> IEntityManager::CreateVector2(alt::Vector2f vec)
{
    std::vector<v8::Local<v8::Value>> args{ V8::JSValue(vec[0]), V8::JSValue(vec[1]) };

    return v8Vector2.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> IEntityManager::CreateRGBA(alt::RGBA rgba)
{
    std::vector<v8::Local<v8::Value>> args{ V8::JSValue(rgba.r), V8::JSValue(rgba.g), V8::JSValue(rgba.b), V8::JSValue(rgba.a) };

    return V8::New(isolate, GetContext(), rgbaClass.Get(isolate), args);
}

bool IEntityManager::IsVector3(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), vector3Class.Get(isolate)).To(&result);
    return result;
}

bool IEntityManager::IsVector2(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), vector2Class.Get(isolate)).To(&result);
    return result;
}

bool IEntityManager::IsRGBA(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), rgbaClass.Get(isolate)).To(&result);
    return result;
}

bool IEntityManager::IsBaseObject(v8::Local<v8::Value> val)
{
    bool result = false;
    val->InstanceOf(GetContext(), baseObjectClass.Get(isolate)).To(&result);
    return result;
}

void IEntityManager::OnCreateBaseObject(alt::Ref<alt::IBaseObject> handle)
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

void IEntityManager::OnRemoveBaseObject(alt::Ref<alt::IBaseObject> handle)
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

void IEntityManager::NotifyPoolUpdate(alt::IBaseObject* ent)
{
    switch(ent->GetType())
    {
        case alt::IBaseObject::Type::PLAYER: playerPoolDirty = true; break;
        case alt::IBaseObject::Type::VEHICLE: vehiclePoolDirty = true; break;
    }
}

v8::Local<v8::Array> IEntityManager::GetAllPlayers()
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

v8::Local<v8::Array> IEntityManager::GetAllBlips()
{
    Array<Ref<IBlip>> all = ICore::Instance().GetBlips();
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

    for(uint32_t i = 0; i < all.GetSize(); ++i) jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

    return jsAll;
}

v8::Local<v8::Array> IEntityManager::GetAllVehicles()
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
