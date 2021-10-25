#pragma once

#include "v8.h"
#include "V8Helpers.h"
#include "V8Entity.h"

class IEntityManager
{
protected:
    std::unordered_map<alt::IBaseObject*, V8Entity*> entities;

    V8::CPersistent<v8::Function> vector3Class;
    V8::CPersistent<v8::Function> vector2Class;
    V8::CPersistent<v8::Function> rgbaClass;
    V8::CPersistent<v8::Function> baseObjectClass;

    bool playerPoolDirty = true;
    v8::UniquePersistent<v8::Array> players;

    bool vehiclePoolDirty = true;
    v8::UniquePersistent<v8::Array> vehicles;

public:
    IEntityManager(v8::Isolate* isolate);
    ~IEntityManager();

    V8Entity* GetEntity(alt::IBaseObject* handle)
    {
        auto it = entities.find(handle);

        if(it == entities.end()) return nullptr;

        return it->second;
    }

    V8Entity* CreateEntity(alt::IBaseObject* handle)
    {
        auto context = v8::Isolate::GetCurrent()->GetEnteredOrMicrotaskContext();
        V8Class* _class = V8Entity::GetClass(handle);

        V8Entity* ent = new V8Entity(context, _class, _class->CreateInstance(context), handle);
        entities.insert({ handle, ent });
        return ent;
    }

    void BindEntity(v8::Local<v8::Object> val, alt::Ref<alt::IBaseObject> handle);

    V8Entity* GetOrCreateEntity(alt::IBaseObject* handle, const char* className = "")
    {
        if(!handle) Log::Error << __FUNCTION__ << " received invalid handle please contact developers if you see this" << Log::Endl;

        V8Entity* ent = GetEntity(handle);

        if(!ent) ent = CreateEntity(handle);

        return ent;
    }

    v8::Local<v8::Value> GetBaseObjectOrNull(alt::IBaseObject* handle);

    template<class T>
    v8::Local<v8::Value> GetBaseObjectOrNull(const alt::Ref<T>& handle)
    {
        return GetBaseObjectOrNull(handle.Get());
    }

    v8::Local<v8::Value> CreateVector3(alt::Vector3f vec);
    v8::Local<v8::Value> CreateVector2(alt::Vector2f vec);
    v8::Local<v8::Value> CreateRGBA(alt::RGBA rgba);

    bool IsVector3(v8::Local<v8::Value> val);
    bool IsVector2(v8::Local<v8::Value> val);
    bool IsRGBA(v8::Local<v8::Value> val);
    bool IsBaseObject(v8::Local<v8::Value> val);

    void OnCreateBaseObject(alt::Ref<alt::IBaseObject> handle);
    void OnRemoveBaseObject(alt::Ref<alt::IBaseObject> handle);

    void NotifyPoolUpdate(alt::IBaseObject* ent);

    v8::Local<v8::Array> GetAllPlayers();
    v8::Local<v8::Array> GetAllVehicles();
    v8::Local<v8::Array> GetAllBlips();
};
