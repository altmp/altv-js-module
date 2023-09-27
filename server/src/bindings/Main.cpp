#include "stdafx.h"

#include "V8Module.h"
#include "CNodeResourceImpl.h"

using namespace alt;

static void OnClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->SubscribeGenericRemote(callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, eventName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->SubscribeRemote(eventName, callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
    }
}

static void OnceClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->SubscribeGenericRemote(callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource), true);
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, eventName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->SubscribeRemote(eventName, callback, V8Helpers::SourceLocation::GetCurrent(isolate, resource), true);
    }
}

static void OffClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->UnsubscribeGenericRemote(callback);
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, evName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->UnsubscribeRemote(evName, callback);
    }
}

static void EmitClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(2);

    V8_ARG_TO_STRING(2, eventName);

    MValueArgs mvArgs;

    for(int i = 2; i < info.Length(); ++i) mvArgs.emplace_back(V8Helpers::V8ToMValue(info[i], false));

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        std::vector<IPlayer*> targets;
        targets.reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            IPlayer* player;
            v8::Local<v8::Value> ply;

            bool toLocalSuccess = arr->Get(ctx, i).ToLocal(&ply);
            V8_CHECK_NORETN(toLocalSuccess, "Invalid player in emitClient players array");
            if(!toLocalSuccess) continue;
            V8Entity* v8Player = V8Entity::Get(ply);

            bool isPlayerType = v8Player && v8Player->GetHandle() && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER;
            V8_CHECK_NORETN(isPlayerType, "player inside array expected");
            if(!isPlayerType) continue;
            targets.push_back(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()));
        }

        ICore::Instance().TriggerClientEvent(targets, eventName, mvArgs);
    }
    else
    {
        // if first argument not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle() && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or player array expected");

        ICore::Instance().TriggerClientEvent(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()), eventName, mvArgs);
    }
}

static void EmitClientUnreliable(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(2);

    V8_ARG_TO_STRING(2, eventName);

    MValueArgs mvArgs;

    for(int i = 2; i < info.Length(); ++i) mvArgs.emplace_back(V8Helpers::V8ToMValue(info[i], false));

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        std::vector<IPlayer*> targets;
        targets.reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            IPlayer* player;
            v8::Local<v8::Value> ply;

            bool toLocalSuccess = arr->Get(ctx, i).ToLocal(&ply);
            V8_CHECK_NORETN(toLocalSuccess, "Invalid player in emitClientUnreliable players array");
            if(!toLocalSuccess) continue;
            V8Entity* v8Player = V8Entity::Get(ply);

            bool isPlayerType = v8Player && v8Player->GetHandle() && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER;
            V8_CHECK_NORETN(isPlayerType, "player inside array expected");
            if(!isPlayerType) continue;
            targets.push_back(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()));
        }

        ICore::Instance().TriggerClientEventUnreliable(targets, eventName, mvArgs);
    }
    else
    {
        // if first argument is not null and not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle() && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or player array expected");

        ICore::Instance().TriggerClientEventUnreliable(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()), eventName, mvArgs);
    }
}

static void EmitAllClients(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, eventName);

    MValueArgs args;

    for(int i = 1; i < info.Length(); ++i) args.emplace_back(V8Helpers::V8ToMValue(info[i], false));

    ICore::Instance().TriggerClientEventForAll(eventName, args);
}

static void EmitClientRaw(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(2);

    V8_ARG_TO_STRING(2, eventName);

    MValueArgs mvArgs;

    for(int i = 2; i < info.Length(); ++i)
    {
        v8::TryCatch tryCatch(isolate);
        alt::MValueByteArray result = V8Helpers::V8ToRawBytes(info[i]);
        if(tryCatch.HasCaught())
        {
            tryCatch.ReThrow();
            return;
        }
        V8_CHECK(result, "Failed to serialize value");
        mvArgs.emplace_back(result);
    }

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        std::vector<IPlayer*> targets;
        targets.reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            IPlayer* player;
            v8::Local<v8::Value> ply;

            bool toLocalSuccess = arr->Get(ctx, i).ToLocal(&ply);
            V8_CHECK_NORETN(toLocalSuccess, "Invalid player in emitClient players array");
            if(!toLocalSuccess) continue;
            V8Entity* v8Player = V8Entity::Get(ply);

            bool isPlayerType = v8Player && v8Player->GetHandle() && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER;
            V8_CHECK_NORETN(isPlayerType, "player inside array expected");
            if(!isPlayerType) continue;
            targets.push_back(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()));
        }

        ICore::Instance().TriggerClientEvent(targets, eventName, mvArgs);
    }
    else
    {
        // if first argument is not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle() && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or player array expected");

        ICore::Instance().TriggerClientEvent(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()), eventName, mvArgs);
    }
}

static void EmitAllClientsRaw(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, eventName);

    MValueArgs args;

    for(int i = 1; i < info.Length(); ++i)
    {
        v8::TryCatch tryCatch(isolate);
        alt::MValueByteArray result = V8Helpers::V8ToRawBytes(info[i]);
        if(tryCatch.HasCaught())
        {
            tryCatch.ReThrow();
            return;
        }
        V8_CHECK(result, "Failed to serialize value");
        args.emplace_back(result);
    }

    ICore::Instance().TriggerClientEventForAll(eventName, args);
}

static void EmitAllClientsUnreliable(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, eventName);

    MValueArgs args;

    for(int i = 1; i < info.Length(); ++i) args.emplace_back(V8Helpers::V8ToMValue(info[i], false));

    ICore::Instance().TriggerClientEventUnreliableForAll(eventName, args);
}

static void SetSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, value);

    alt::ICore::Instance().SetSyncedMetaData(key, value);
}

static void DeleteSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, key);

    alt::ICore::Instance().DeleteSyncedMetaData(key);
}

static void SetPassword(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, password);

    alt::ICore::Instance().SetPassword(password);
}

static void StartResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    alt::ICore::Instance().StartResource(name);
}

static void StopResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    alt::ICore::Instance().StopResource(name);
}

static void RestartResource(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    alt::ICore::Instance().RestartResource(name);
}

static void HashServerPassword(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, password);

    V8_RETURN_UINT64(alt::ICore::Instance().HashServerPassword(password));
}

static void StopServer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    alt::ICore::Instance().StopServer();
}

static void HasExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, extra);
    uint32_t hash = info.Data().As<v8::Uint32>()->Value();

    const alt::VehicleModelInfo& modelInfo = alt::ICore::Instance().GetVehicleModelByHash(hash);
    V8_RETURN_BOOLEAN(modelInfo.DoesExtraExist(extra));
}
static void HasDefaultExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, extra);
    uint32_t hash = info.Data().As<v8::Uint32>()->Value();

    const alt::VehicleModelInfo& modelInfo = alt::ICore::Instance().GetVehicleModelByHash(hash);
    V8_RETURN_BOOLEAN(modelInfo.DoesExtraDefault(extra));
}
static void GetVehicleModelByHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, hash);

    const alt::VehicleModelInfo& modelInfo = alt::ICore::Instance().GetVehicleModelByHash(hash);
    V8_NEW_OBJECT(infoObj);

    infoObj->Set(ctx, V8Helpers::JSValue("title"), V8Helpers::JSValue(modelInfo.title));
    infoObj->Set(ctx, V8Helpers::JSValue("type"), V8Helpers::JSValue((int)modelInfo.modelType));
    infoObj->Set(ctx, V8Helpers::JSValue("wheelsCount"), V8Helpers::JSValue(modelInfo.wheelsCount));
    infoObj->Set(ctx, V8Helpers::JSValue("hasArmoredWindows"), V8Helpers::JSValue(modelInfo.hasArmoredWindows));
    infoObj->Set(ctx, V8Helpers::JSValue("primaryColor"), V8Helpers::JSValue(modelInfo.primaryColor));
    infoObj->Set(ctx, V8Helpers::JSValue("secondaryColor"), V8Helpers::JSValue(modelInfo.secondaryColor));
    infoObj->Set(ctx, V8Helpers::JSValue("pearlColor"), V8Helpers::JSValue(modelInfo.pearlColor));
    infoObj->Set(ctx, V8Helpers::JSValue("wheelsColor"), V8Helpers::JSValue(modelInfo.wheelsColor));
    infoObj->Set(ctx, V8Helpers::JSValue("interiorColor"), V8Helpers::JSValue(modelInfo.interiorColor));
    infoObj->Set(ctx, V8Helpers::JSValue("dashboardColor"), V8Helpers::JSValue(modelInfo.dashboardColor));
    infoObj->Set(ctx, V8Helpers::JSValue("hasAutoAttachTrailer"), V8Helpers::JSValue(modelInfo.hasAutoAttachTrailer));

    size_t modkitsSize = std::size(modelInfo.modkits);
    v8::Local<v8::Array> modkitsArr = v8::Array::New(isolate, modkitsSize);
    for(size_t i = 0; i < modkitsSize; i++)
    {
        modkitsArr->Set(ctx, i, V8Helpers::JSValue(modelInfo.modkits[i] != 0xFFFF));
    }
    infoObj->Set(ctx, V8Helpers::JSValue("availableModkits"), modkitsArr);

    infoObj->Set(ctx, V8Helpers::JSValue("hasExtra"), v8::Function::New(ctx, &HasExtra, v8::Uint32::NewFromUnsigned(isolate, hash)).ToLocalChecked());
    infoObj->Set(ctx, V8Helpers::JSValue("hasDefaultExtra"), v8::Function::New(ctx, &HasDefaultExtra, v8::Uint32::NewFromUnsigned(isolate, hash)).ToLocalChecked());

    size_t boneSize = std::size(modelInfo.bones);
    v8::Local<v8::Array> boneArr = v8::Array::New(isolate, boneSize);
    for(size_t i = 0; i < boneSize; i++)
    {
        V8_NEW_OBJECT(boneObj);
        boneObj->Set(ctx, V8Helpers::JSValue("id"), V8Helpers::JSValue(modelInfo.bones[i].id));
        boneObj->Set(ctx, V8Helpers::JSValue("index"), V8Helpers::JSValue(modelInfo.bones[i].index));
        boneObj->Set(ctx, V8Helpers::JSValue("name"), V8Helpers::JSValue(modelInfo.bones[i].name));
        boneArr->Set(ctx, i, boneObj);
    }
    infoObj->Set(ctx, V8Helpers::JSValue("bones"), boneArr);

    infoObj->Set(ctx, V8Helpers::JSValue("canAttachCars"), V8Helpers::JSValue(modelInfo.canAttachCars));
    infoObj->Set(ctx, V8Helpers::JSValue("handlingNameHash"), V8Helpers::JSValue(modelInfo.handlingNameHash));

    V8_RETURN(infoObj);
}

static void GetServerConfig(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    auto config = alt::ICore::Instance().GetServerConfig();
    v8::Local<v8::Value> val = V8Helpers::ConfigNodeToV8(config);
    V8_CHECK(!val.IsEmpty(), "Failed to convert server config to V8 value");
    V8_RETURN(val);
}

static void GetPedModelByHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, hash);

    const alt::PedModelInfo& modelInfo = alt::ICore::Instance().GetPedModelByHash(hash);
    V8_NEW_OBJECT(infoObj);

    infoObj->Set(ctx, V8Helpers::JSValue("hash"), V8Helpers::JSValue(modelInfo.hash));
    infoObj->Set(ctx, V8Helpers::JSValue("name"), V8Helpers::JSValue(modelInfo.name));
    infoObj->Set(ctx, V8Helpers::JSValue("type"), V8Helpers::JSValue(modelInfo.type));
    infoObj->Set(ctx, V8Helpers::JSValue("dlcName"), V8Helpers::JSValue(modelInfo.dlcName));
    infoObj->Set(ctx, V8Helpers::JSValue("defaultUnarmedWeapon"), V8Helpers::JSValue(modelInfo.defaultUnarmedWeapon));
    infoObj->Set(ctx, V8Helpers::JSValue("movementClipSet"), V8Helpers::JSValue(modelInfo.movementClipSet));

    size_t boneSize = std::size(modelInfo.bones);
    v8::Local<v8::Array> boneArr = v8::Array::New(isolate, boneSize);
    for(size_t i = 0; i < boneSize; i++)
    {
        V8_NEW_OBJECT(boneObj);
        boneObj->Set(ctx, V8Helpers::JSValue("id"), V8Helpers::JSValue(modelInfo.bones[i].id));
        boneObj->Set(ctx, V8Helpers::JSValue("index"), V8Helpers::JSValue(modelInfo.bones[i].index));
        boneObj->Set(ctx, V8Helpers::JSValue("name"), V8Helpers::JSValue(modelInfo.bones[i].name));
        boneArr->Set(ctx, i, boneObj);
    }
    infoObj->Set(ctx, V8Helpers::JSValue("bones"), boneArr);

    V8_RETURN(infoObj);
}

static void SetWorldProfiler(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, isActive);

    ICore::Instance().SetWorldProfiler(isActive);
}

static void GetEntitiesInDimension(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT32(1, dimension);
    V8_ARG_TO_UINT(2, allowedTypes);

    auto entities = ICore::Instance().GetEntitiesInDimension(dimension, allowedTypes);
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, entities.size());
    for(uint32_t i = 0; i < entities.size(); ++i) jsAll->Set(resource->GetContext(), i, resource->GetBaseObjectOrNull(entities[i]));

    V8_RETURN(jsAll);
}

static void GetEntitiesInRange(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(4);
    V8_ARG_TO_VECTOR3(1, position);
    V8_ARG_TO_INT32(2, range);
    V8_ARG_TO_INT32(3, dimension);
    V8_ARG_TO_UINT(4, allowedTypes);

    auto entities = ICore::Instance().GetEntitiesInRange(position, range, dimension, allowedTypes);
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, entities.size());
    for(uint32_t i = 0; i < entities.size(); ++i) jsAll->Set(resource->GetContext(), i, resource->GetBaseObjectOrNull(entities[i]));

    V8_RETURN(jsAll);
}

static void GetClosestEntities(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(5);
    V8_ARG_TO_VECTOR3(1, position);
    V8_ARG_TO_INT32(2, range);
    V8_ARG_TO_INT32(3, dimension);
    V8_ARG_TO_INT32(4, limit)
    V8_ARG_TO_UINT(5, allowedTypes);

    auto entities = ICore::Instance().GetClosestEntities(position, range, dimension, limit, allowedTypes);
    v8::Local<v8::Array> jsAll = v8::Array::New(isolate, entities.size());
    for(uint32_t i = 0; i < entities.size(); ++i) jsAll->Set(resource->GetContext(), i, resource->GetBaseObjectOrNull(entities[i]));

    V8_RETURN(jsAll);
}

static void GetWeaponModelByHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, hash);

    const alt::WeaponModelInfo& modelInfo = alt::ICore::Instance().GetWeaponModelByHash(hash);
    V8_NEW_OBJECT(infoObj);

    infoObj->Set(ctx, V8Helpers::JSValue("hash"), V8Helpers::JSValue(modelInfo.hash));
    infoObj->Set(ctx, V8Helpers::JSValue("name"), V8Helpers::JSValue(modelInfo.name));
    infoObj->Set(ctx, V8Helpers::JSValue("modelName"), V8Helpers::JSValue(modelInfo.modelName));
    infoObj->Set(ctx, V8Helpers::JSValue("modelHash"), V8Helpers::JSValue(modelInfo.modelHash));
    infoObj->Set(ctx, V8Helpers::JSValue("ammoTypeHash"), V8Helpers::JSValue(modelInfo.ammoTypeHash));
    infoObj->Set(ctx, V8Helpers::JSValue("ammoType"), V8Helpers::JSValue(modelInfo.ammoType));
    infoObj->Set(ctx, V8Helpers::JSValue("ammoModelName"), V8Helpers::JSValue(modelInfo.ammoModelName));
    infoObj->Set(ctx, V8Helpers::JSValue("ammoModelHash"), V8Helpers::JSValue(modelInfo.ammoModelHash));
    infoObj->Set(ctx, V8Helpers::JSValue("defaultMaxAmmoMp"), V8Helpers::JSValue(modelInfo.defaultMaxAmmoMp));
    infoObj->Set(ctx, V8Helpers::JSValue("skillAbove50MaxAmmoMp"), V8Helpers::JSValue(modelInfo.skillAbove50MaxAmmoMp));
    infoObj->Set(ctx, V8Helpers::JSValue("maxSkillMaxAmmoMp"), V8Helpers::JSValue(modelInfo.maxSkillMaxAmmoMp));
    infoObj->Set(ctx, V8Helpers::JSValue("bonusMaxAmmoMp"), V8Helpers::JSValue(modelInfo.bonusMaxAmmoMp));

    V8_RETURN(infoObj);
}

static void GetAmmoHashForWeaponHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    uint32_t hash;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weaponHash);
        hash = weaponHash;
    }
    else
    {
        V8_ARG_TO_STRING(1, weaponHash);
        hash = alt::ICore::Instance().Hash(weaponHash);
    }

    V8_RETURN_UINT(alt::ICore::Instance().GetAmmoHashForWeaponHash(hash));
}

static void SetVoiceExternalPublic(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, host);
    V8_ARG_TO_UINT(2, port);

    alt::ICore::Instance().SetVoiceExternalPublic(host, port);
}

static void SetVoiceExternal(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, host);
    V8_ARG_TO_UINT(2, port);

    alt::ICore::Instance().SetVoiceExternal(host, port);
}

static void GetStreamingDistance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetStreamingDistance());
}

static void SetStreamingDistance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, distance);

    alt::ICore::Instance().SetStreamingDistance(distance);
}

static void GetStreamingTickRate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetStreamingTickRate());
}

static void SetStreamingTickRate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, tickrate);

    alt::ICore::Instance().SetStreamingTickRate(tickrate);
}

static void GetStreamerThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetStreamerThreadCount());
}

static void SetStreamerThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, count);

    alt::ICore::Instance().SetStreamerThreadCount(count);
}

static void GetMaxStreamingPeds(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetMaxStreamingPeds());
}

static void SetMaxStreamingPeds(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, limit);

    alt::ICore::Instance().SetMaxStreamingPeds(limit);
}

static void GetMaxStreamingObjects(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetMaxStreamingObjects());
}

static void SetMaxStreamingObjects(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, limit);

    alt::ICore::Instance().SetMaxStreamingObjects(limit);
}

static void GetMaxStreamingVehicles(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetMaxStreamingVehicles());
}

static void SetMaxStreamingVehicles(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, limit);

    alt::ICore::Instance().SetMaxStreamingVehicles(limit);
}

static void GetMigrationThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetMigrationThreadCount());
}

static void SetMigrationThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, count);

    alt::ICore::Instance().SetMigrationThreadCount(count);
}

static void GetSyncSendThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetSyncSendThreadCount());
}

static void SetSyncSendThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, count);

    alt::ICore::Instance().SetSyncSendThreadCount(count);
}

static void GetSyncReceiveThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetSyncReceiveThreadCount());
}

static void SetSyncReceiveThreadCount(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, count);

    alt::ICore::Instance().SetSyncReceiveThreadCount(count);
}

static void GetMigrationTickRate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetMigrationTickRate());
}

static void SetMigrationTickRate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, tickrate);

    alt::ICore::Instance().SetMigrationTickRate(tickrate);
}

static void GetColShapeTickRate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetColShapeTickRate());
}

static void SetColShapeTickRate(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, tickrate);

    alt::ICore::Instance().SetColShapeTickRate(tickrate);
}

static void GetMigrationDistance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN_UINT(alt::ICore::Instance().GetMigrationDistance());
}

static void SetMigrationDistance(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, tickrate);

    alt::ICore::Instance().SetMigrationDistance(tickrate);
}

extern V8Class v8Player, v8Vehicle, v8Blip, v8AreaBlip, v8RadiusBlip, v8PointBlip, v8Checkpoint, v8VoiceChannel, v8Colshape, v8ColshapeCylinder, v8ColshapeSphere, v8ColshapeCircle,
  v8ColshapeCuboid, v8ColshapeRectangle, v8ColshapePolygon, v8Ped, v8Object, v8VirtualEntity, v8VirtualEntityGroup, v8Marker, v8ConnectionInfo;

extern V8Module sharedModule;

extern V8Module
  v8Alt("alt",
        &sharedModule,
        { v8Player,           v8Vehicle,        v8Blip,           v8AreaBlip,       v8RadiusBlip,        v8PointBlip,       v8Checkpoint, v8RadiusBlip, v8VoiceChannel,       v8Colshape,
          v8ColshapeCylinder, v8ColshapeSphere, v8ColshapeCircle, v8ColshapeCuboid, v8ColshapeRectangle, v8ColshapePolygon, v8Ped,        v8Object,     v8VirtualEntityGroup, v8VirtualEntity,
          v8Marker,           v8ConnectionInfo },
        [](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)
        {
            v8::Isolate* isolate = ctx->GetIsolate();

            V8Helpers::RegisterFunc(exports, "startResource", &StartResource);
            V8Helpers::RegisterFunc(exports, "stopResource", &StopResource);
            V8Helpers::RegisterFunc(exports, "restartResource", &RestartResource);

            V8Helpers::RegisterFunc(exports, "onClient", &OnClient);
            V8Helpers::RegisterFunc(exports, "onceClient", &OnceClient);
            V8Helpers::RegisterFunc(exports, "offClient", &OffClient);
            V8Helpers::RegisterFunc(exports, "emitClient", &EmitClient);
            V8Helpers::RegisterFunc(exports, "emitAllClients", &EmitAllClients);
            V8Helpers::RegisterFunc(exports, "emitClientRaw", &EmitClientRaw);
            V8Helpers::RegisterFunc(exports, "emitAllClientsRaw", &EmitAllClientsRaw);
            V8Helpers::RegisterFunc(exports, "emitClientUnreliable", &EmitClientUnreliable);
            V8Helpers::RegisterFunc(exports, "emitAllClientsUnreliable", &EmitAllClientsUnreliable);

            V8Helpers::RegisterFunc(exports, "setSyncedMeta", &SetSyncedMeta);
            V8Helpers::RegisterFunc(exports, "deleteSyncedMeta", &DeleteSyncedMeta);

            V8Helpers::RegisterFunc(exports, "setPassword", &SetPassword);

            V8Helpers::RegisterFunc(exports, "hashServerPassword", &HashServerPassword);

            V8Helpers::RegisterFunc(exports, "stopServer", &StopServer);

            V8Helpers::RegisterFunc(exports, "getVehicleModelInfoByHash", &GetVehicleModelByHash);
            V8Helpers::RegisterFunc(exports, "getPedModelInfoByHash", &GetPedModelByHash);
            V8Helpers::RegisterFunc(exports, "getWeaponModelInfoByHash", &GetWeaponModelByHash);
            V8Helpers::RegisterFunc(exports, "getAmmoHashForWeaponHash", &GetAmmoHashForWeaponHash);

            V8Helpers::RegisterFunc(exports, "getServerConfig", &GetServerConfig);

            V8Helpers::RegisterFunc(exports, "toggleWorldProfiler", &SetWorldProfiler);

            V8Helpers::RegisterFunc(exports, "getEntitiesInDimension", &GetEntitiesInDimension);
            V8Helpers::RegisterFunc(exports, "getEntitiesInRange", &GetEntitiesInRange);
            V8Helpers::RegisterFunc(exports, "getClosestEntities", &GetClosestEntities);

            V8Helpers::RegisterFunc(exports, "setVoiceExternalPublic", &SetVoiceExternalPublic);
            V8Helpers::RegisterFunc(exports, "setVoiceExternal", &SetVoiceExternal);

            V8Helpers::RegisterFunc(exports, "getMaxStreamingPeds", &GetMaxStreamingPeds);
            V8Helpers::RegisterFunc(exports, "getMaxStreamingObjects", &GetMaxStreamingObjects);
            V8Helpers::RegisterFunc(exports, "getMaxStreamingVehicles", &GetMaxStreamingVehicles);
            V8Helpers::RegisterFunc(exports, "setMaxStreamingPeds", &SetMaxStreamingPeds);
            V8Helpers::RegisterFunc(exports, "setMaxStreamingObjects", &SetMaxStreamingObjects);
            V8Helpers::RegisterFunc(exports, "setMaxStreamingVehicles", &SetMaxStreamingVehicles);

            V8Helpers::RegisterFunc(exports, "getStreamerThreadCount", &GetStreamerThreadCount);
            V8Helpers::RegisterFunc(exports, "setStreamerThreadCount", &SetStreamerThreadCount);

            V8Helpers::RegisterFunc(exports, "getMigrationThreadCount", &GetMigrationThreadCount);
            V8Helpers::RegisterFunc(exports, "setMigrationThreadCount", &SetMigrationThreadCount);

            V8Helpers::RegisterFunc(exports, "getSyncSendThreadCount", &GetSyncSendThreadCount);
            V8Helpers::RegisterFunc(exports, "setSyncSendThreadCount", &SetSyncSendThreadCount);

            V8Helpers::RegisterFunc(exports, "getSyncReceiveThreadCount", &GetSyncReceiveThreadCount);
            V8Helpers::RegisterFunc(exports, "setSyncReceiveThreadCount", &SetSyncReceiveThreadCount);

            V8Helpers::RegisterFunc(exports, "getStreamingTickRate", &GetStreamingTickRate);
            V8Helpers::RegisterFunc(exports, "setStreamingTickRate", &SetStreamingTickRate);

            V8Helpers::RegisterFunc(exports, "getMigrationTickRate", &GetMigrationTickRate);
            V8Helpers::RegisterFunc(exports, "setMigrationTickRate", &SetMigrationTickRate);

            V8Helpers::RegisterFunc(exports, "getColShapeTickRate", &GetColShapeTickRate);
            V8Helpers::RegisterFunc(exports, "setColShapeTickRate", &SetColShapeTickRate);

            V8Helpers::RegisterFunc(exports, "getStreamingDistance", &GetStreamingDistance);
            V8Helpers::RegisterFunc(exports, "setStreamingDistance", &SetStreamingDistance);

            V8Helpers::RegisterFunc(exports, "getMigrationDistance", &GetMigrationDistance);
            V8Helpers::RegisterFunc(exports, "setMigrationDistance", &SetMigrationDistance);

            V8_OBJECT_SET_STRING(exports, "rootDir", alt::ICore::Instance().GetRootDirectory());
        });
