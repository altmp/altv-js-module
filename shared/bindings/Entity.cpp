
#include "../V8Helpers.h"
#include "../helpers/BindHelpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"
#include "cpp-sdk/objects/IPlayer.h"

using namespace alt;

static void HasStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

    V8_RETURN_BOOLEAN(ent->HasStreamSyncedMetaData(key));
}

static void GetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

    V8_RETURN_MVALUE(ent->GetStreamSyncedMetaData(key));
}

static void GetStreamSyncedMetaDataKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

    const std::vector<std::string> list = ent->GetStreamSyncedMetaDataKeys();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

#ifdef ALT_SERVER_API

static void ModelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);
    V8_RETURN_UINT(ent->GetModel());
}

static void ModelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    if(val->IsNumber())
    {
        V8_TO_INTEGER(val, model);
        player->SetModel(model);
    }
    else
    {
        V8_TO_STRING(val, model);
        player->SetModel(alt::ICore::Instance().Hash(model));
    }
}

static void SetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

    if (info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, key);
        V8_ARG_TO_MVALUE(2, value);

        ent->SetStreamSyncedMetaData(key, value);
    }
    else if (info.Length() == 1 && info[0]->IsObject())
    {
        auto dict = V8Helpers::CppValue<v8::Local<v8::Value>>(info[0].As<v8::Object>());
        std::unordered_map<std::string, MValue> values;

        if (dict.has_value())
        {
            for (auto& [key, value] : dict.value())
                values[key] = V8Helpers::V8ToMValue(value);
        }

        ent->SetMultipleStreamSyncedMetaData(values);
    }
}

static void DeleteStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);

    ent->DeleteStreamSyncedMetaData(key);
}

static void SetNetOwner(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);
    V8_GET_THIS_BASE_OBJECT(_this, IEntity);
    V8_ARG_TO_BASE_OBJECT(1, player, IPlayer, "Player");
    V8_ARG_TO_BOOLEAN_OPT(2, disableMigration, false);

    _this->SetNetworkOwner(player, disableMigration);
}

static void ResetNetOwner(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(0, 1);
    V8_GET_THIS_BASE_OBJECT(_this, IEntity);
    V8_ARG_TO_BOOLEAN_OPT(1, disableMigration, false);

    _this->SetNetworkOwner(nullptr, disableMigration);
}

static void AttachTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(7);
    V8_GET_THIS_BASE_OBJECT(_this, IEntity);

    V8_ARG_TO_BASE_OBJECT(1, entity, IEntity, "Entity");

    V8_ARG_TO_VECTOR3(4, pos);
    V8_ARG_TO_VECTOR3(5, rot);
    V8_ARG_TO_BOOLEAN(6, collision);
    V8_ARG_TO_BOOLEAN(7, noFixedRot);

    if(info[1]->IsString() && info[2]->IsString())
    {
        V8_ARG_TO_STRING(2, otherBone);
        V8_ARG_TO_STRING(3, ownBone);

        _this->AttachToEntity(entity, otherBone, ownBone, pos, rot, collision, noFixedRot);
    }
    else if(info[1]->IsNumber() && info[2]->IsNumber())
    {
        V8_ARG_TO_UINT(2, otherBone);
        V8_ARG_TO_UINT(3, ownBone);

        _this->AttachToEntity(entity, otherBone, ownBone, pos, rot, collision, noFixedRot);
    }
    else
    {
        V8Helpers::Throw(isolate, "Invalid bone arguments");
    }
}

#endif  // ALT_SERVER_API

#ifdef ALT_CLIENT_API

static void PositionGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);
    V8_RETURN_VECTOR3(ent->GetPosition());
}

static void PositionSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, alt::IEntity);

    V8_TO_VECTOR3(val, vector);

    const auto netOwner = _this->GetNetworkOwner();
    if(!netOwner || netOwner != alt::ICore::Instance().GetLocalPlayer())
    {
        V8Helpers::Throw(isolate, "Position can only be modified by the network owner of the entity");
        return;
    }
    _this->SetPosition(vector);
}

static void RotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IEntity);
    V8_RETURN_VECTOR3(ent->GetRotation());
}

static void RotationSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, alt::IEntity);

    V8_TO_VECTOR3(val, vector);

    const auto netOwner = _this->GetNetworkOwner();
    if(!netOwner || netOwner != alt::ICore::Instance().GetLocalPlayer())
    {
        V8Helpers::Throw(isolate, "Rotation can only be modified by the network owner of the entity");
        return;
    }
    _this->SetRotation(vector);
}

static void IsSpawnedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(entity, alt::IEntity);

    V8_RETURN_BOOLEAN(entity->GetScriptID() != 0);
}

static void GetSyncInfo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(entity, alt::IEntity);

    const auto syncInfo = entity->GetSyncInfo();

    v8::Local<v8::Object> obj = v8::Object::New(isolate);
    obj->Set(ctx, V8Helpers::JSValue("active"), V8Helpers::JSValue(syncInfo.active));
    obj->Set(ctx, V8Helpers::JSValue("receivedTick"), V8Helpers::JSValue(syncInfo.receivedTick));
    obj->Set(ctx, V8Helpers::JSValue("fullyReceivedTick"), V8Helpers::JSValue(syncInfo.fullyReceivedTick));
    obj->Set(ctx, V8Helpers::JSValue("sendTick"), V8Helpers::JSValue(syncInfo.sendTick));
    obj->Set(ctx, V8Helpers::JSValue("ackedSendTick"), V8Helpers::JSValue(syncInfo.ackedSendTick));
    obj->Set(ctx, V8Helpers::JSValue("propertyCount"), V8Helpers::JSValue(syncInfo.propertyCount));
    obj->Set(ctx, V8Helpers::JSValue("componentCount"), V8Helpers::JSValue(syncInfo.componentCount));
    obj->Set(ctx, V8Helpers::JSValue("propertiesUpdateTick"), V8Helpers::JSValue(syncInfo.propertiesUpdateTick));
    obj->Set(ctx, V8Helpers::JSValue("componentPropertyIndex"), V8Helpers::JSValue(syncInfo.componentPropertyIndex));
    obj->SetIntegrityLevel(ctx, v8::IntegrityLevel::kFrozen);

    V8_RETURN(obj);
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetWorldObjectByScriptID(scriptGuid));
}

#endif  // ALT_CLIENT_API

static void StaticAllGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Array> arr = v8::Array::New(isolate);

    uint16_t i = 0;
    for(auto entity : alt::ICore::Instance().GetEntities())
    {
        if(entity) arr->Set(ctx, i++, resource->GetOrCreateEntity(entity, "Entity")->GetJSVal(isolate));
    };

    V8_RETURN(arr);
}

extern V8Class v8WorldObject;
extern V8Class v8Entity("Entity",
                        v8WorldObject,
                        [](v8::Local<v8::FunctionTemplate> tpl)
                        {
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", StaticAllGetter);

                            V8Helpers::SetAccessor<IEntity, IPlayer*, &IEntity::GetNetworkOwner>(isolate, tpl, "netOwner");

                            V8Helpers::SetMethod(isolate, tpl, "hasStreamSyncedMeta", HasStreamSyncedMeta);
                            V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMeta", GetStreamSyncedMeta);
                            V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMetaKeys", GetStreamSyncedMetaDataKeys);
                            V8Helpers::SetAccessor<IEntity, bool, &IEntity::IsFrozen, &IEntity::SetFrozen>(isolate, tpl, "frozen");

#ifdef ALT_SERVER_API
                            V8Helpers::SetAccessor<IEntity, Rotation, &IEntity::GetRotation, &IEntity::SetRotation>(isolate, tpl, "rot");
                            V8Helpers::SetAccessor(isolate, tpl, "model", &ModelGetter, &ModelSetter);
                            V8Helpers::SetAccessor<IEntity, bool, &IEntity::GetVisible, &IEntity::SetVisible>(isolate, tpl, "visible");
                            V8Helpers::SetAccessor<IEntity, bool, &IEntity::GetStreamed, &IEntity::SetStreamed>(isolate, tpl, "streamed");

                            V8Helpers::SetMethod(isolate, tpl, "setStreamSyncedMeta", SetStreamSyncedMeta);
                            V8Helpers::SetMethod(isolate, tpl, "deleteStreamSyncedMeta", DeleteStreamSyncedMeta);

                            V8Helpers::SetMethod(isolate, tpl, "setNetOwner", SetNetOwner);
                            V8Helpers::SetMethod(isolate, tpl, "resetNetOwner", ResetNetOwner);

                            V8Helpers::SetMethod(isolate, tpl, "attachTo", AttachTo);
                            V8Helpers::SetMethod<IEntity, &IEntity::Detach>(isolate, tpl, "detach");

                            V8Helpers::SetAccessor<IEntity, bool, &IEntity::HasCollision, &IEntity::SetCollision>(isolate, tpl, "collision");

                            V8Helpers::SetAccessor<IEntity, uint32_t, &IEntity::GetStreamingDistance, &IEntity::SetStreamingDistance>(isolate, tpl, "streamingDistance");


                            V8Helpers::SetAccessor<IEntity, uint32_t, &IEntity::GetTimestamp>(isolate, tpl, "timestamp");
#endif  // ALT_SERVER_API

#ifdef ALT_CLIENT_API
                            V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

                            V8Helpers::SetAccessor(isolate, tpl, "pos", &PositionGetter, &PositionSetter);
                            V8Helpers::SetAccessor(isolate, tpl, "rot", &RotationGetter, &RotationSetter);
                            V8Helpers::SetAccessor<IEntity, uint32_t, &IEntity::GetModel>(isolate, tpl, "model");
                            V8Helpers::SetAccessor<IEntity, uint32_t, &IEntity::GetScriptID>(isolate, tpl, "scriptID");
                            V8Helpers::SetAccessor<IEntity, bool, &IEntity::GetVisible>(isolate, tpl, "visible");

                            V8Helpers::SetAccessor(isolate, tpl, "isSpawned", &IsSpawnedGetter);
                            V8Helpers::SetMethod(isolate, tpl, "getSyncInfo", &GetSyncInfo);
#endif  // ALT_CLIENT_API
                        });
