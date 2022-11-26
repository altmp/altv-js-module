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

    for(int i = 2; i < info.Length(); ++i) mvArgs.Push(V8Helpers::V8ToMValue(info[i], false));

    if(info[0]->IsNull())
    {
        // Deprecation added: 06/08/2022 (version 13)
        V8_DEPRECATE("emitClient with null", "emitAllClients");
        ICore::Instance().TriggerClientEventForAll(eventName, mvArgs);
        return;
    }

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        Array<IPlayer*> targets;
        targets.Reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            IPlayer* player;
            v8::Local<v8::Value> ply;

            bool toLocalSuccess = arr->Get(ctx, i).ToLocal(&ply);
            V8_CHECK_NORETN(toLocalSuccess, "Invalid player in emitClient players array");
            if(!toLocalSuccess) continue;
            V8Entity* v8Player = V8Entity::Get(ply);

            bool isPlayerType = v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER;
            V8_CHECK_NORETN(isPlayerType, "player inside array expected");
            if(!isPlayerType) continue;
            targets.Push(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()));
        }

        ICore::Instance().TriggerClientEvent(targets, eventName, mvArgs);
    }
    else
    {
        // if first argument is not null and not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or null expected");

        ICore::Instance().TriggerClientEvent(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()), eventName, mvArgs);
    }
}

static void EmitAllClients(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, eventName);

    MValueArgs args;

    for(int i = 1; i < info.Length(); ++i) args.Push(V8Helpers::V8ToMValue(info[i], false));

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
        V8_CHECK(!result.IsEmpty(), "Failed to serialize value");
        mvArgs.Push(result);
    }

    if(info[0]->IsNull())
    {
        // Deprecation added: 06/08/2022 (version 13)
        V8_DEPRECATE("emitClientRaw with null", "emitAllClientsRaw");
        // if first argument is null this event gets send to every player
        ICore::Instance().TriggerClientEventForAll(eventName, mvArgs);
        return;
    }

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        Array<IPlayer*> targets;
        targets.Reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            IPlayer* player;
            v8::Local<v8::Value> ply;

            bool toLocalSuccess = arr->Get(ctx, i).ToLocal(&ply);
            V8_CHECK_NORETN(toLocalSuccess, "Invalid player in emitClient players array");
            if(!toLocalSuccess) continue;
            V8Entity* v8Player = V8Entity::Get(ply);

            bool isPlayerType = v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER;
            V8_CHECK_NORETN(isPlayerType, "player inside array expected");
            if(!isPlayerType) continue;
            targets.Push(dynamic_cast<alt::IPlayer*>(v8Player->GetHandle()));
        }

        ICore::Instance().TriggerClientEvent(targets, eventName, mvArgs);
    }
    else
    {
        // if first argument is not null and not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or null expected");

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
        V8_CHECK(!result.IsEmpty(), "Failed to serialize value");
        args.Push(result);
    }

    ICore::Instance().TriggerClientEventForAll(eventName, args);
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

static void GetNetTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    uint32_t netTime = alt::ICore::Instance().GetNetTime();

    V8_RETURN_UINT(netTime);
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

extern V8Class v8Player, v8Vehicle, v8Blip, v8AreaBlip, v8RadiusBlip, v8PointBlip, v8Checkpoint, v8VoiceChannel, v8Colshape, v8ColshapeCylinder, v8ColshapeSphere, v8ColshapeCircle,
  v8ColshapeCuboid, v8ColshapeRectangle, v8ColshapePolygon;

extern V8Module sharedModule;

extern V8Module v8Alt("alt",
                      &sharedModule,
                      { v8Player,
                        v8Vehicle,
                        v8Blip,
                        v8AreaBlip,
                        v8RadiusBlip,
                        v8PointBlip,
                        v8Checkpoint,
                        v8RadiusBlip,
                        v8VoiceChannel,
                        v8Colshape,
                        v8ColshapeCylinder,
                        v8ColshapeSphere,
                        v8ColshapeCircle,
                        v8ColshapeCuboid,
                        v8ColshapeRectangle,
                        v8ColshapePolygon },
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

                          V8Helpers::RegisterFunc(exports, "setSyncedMeta", &SetSyncedMeta);
                          V8Helpers::RegisterFunc(exports, "deleteSyncedMeta", &DeleteSyncedMeta);

                          V8Helpers::RegisterFunc(exports, "getNetTime", &GetNetTime);

                          V8Helpers::RegisterFunc(exports, "setPassword", &SetPassword);

                          V8Helpers::RegisterFunc(exports, "hashServerPassword", &HashServerPassword);

                          V8Helpers::RegisterFunc(exports, "stopServer", &StopServer);

                          V8Helpers::RegisterFunc(exports, "getVehicleModelInfoByHash", &GetVehicleModelByHash);
                          V8Helpers::RegisterFunc(exports, "getPedModelInfoByHash", &GetPedModelByHash);

                          V8Helpers::RegisterFunc(exports, "getServerConfig", &GetServerConfig);

                          V8Helpers::RegisterFunc(exports, "toggleWorldProfiler", &SetWorldProfiler);

                          V8_OBJECT_SET_STRING(exports, "rootDir", alt::ICore::Instance().GetRootDirectory());
                          V8_OBJECT_SET_INT(exports, "defaultDimension", alt::DEFAULT_DIMENSION);
                          V8_OBJECT_SET_INT(exports, "globalDimension", alt::GLOBAL_DIMENSION);
                      });
