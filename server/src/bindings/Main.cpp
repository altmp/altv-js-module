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

        resource->SubscribeGenericRemote(callback, V8Helpers::SourceLocation::GetCurrent(isolate));
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, eventName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->SubscribeRemote(eventName.ToString(), callback, V8Helpers::SourceLocation::GetCurrent(isolate));
    }
}

static void OnceClient(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(info.Length() == 1)
    {
        V8_ARG_TO_FUNCTION(1, callback);

        resource->SubscribeGenericRemote(callback, V8Helpers::SourceLocation::GetCurrent(isolate), true);
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_STRING(1, eventName);
        V8_ARG_TO_FUNCTION(2, callback);

        resource->SubscribeRemote(eventName.ToString(), callback, V8Helpers::SourceLocation::GetCurrent(isolate), true);
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

        resource->UnsubscribeRemote(evName.ToString(), callback);
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
        // if first argument is null this event gets send to every player
        ICore::Instance().TriggerClientEventForAll(eventName.ToString(), mvArgs);
        return;
    }

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        Array<Ref<IPlayer>> targets;
        targets.Reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            Ref<IPlayer> player;
            v8::Local<v8::Value> ply;
            V8_CHECK(arr->Get(ctx, i).ToLocal(&ply), "Invalid player in emitClient players array");
            V8Entity* v8Player = V8Entity::Get(ply);

            V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player inside array expected");
            targets.Push(v8Player->GetHandle().As<IPlayer>());
        }

        ICore::Instance().TriggerClientEvent(targets, eventName.ToString(), mvArgs);
    }
    else
    {
        // if first argument is not null and not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or null expected");

        ICore::Instance().TriggerClientEvent(v8Player->GetHandle().As<IPlayer>(), eventName.ToString(), mvArgs);
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
        alt::MValueByteArray result = V8Helpers::V8ToRawBytes(info[i]);
        V8_CHECK(!result.IsEmpty(), "Failed to serialize value");
        mvArgs.Push(result);
    }

    if(info[0]->IsNull())
    {
        // if first argument is null this event gets send to every player
        ICore::Instance().TriggerClientEventForAll(eventName.ToString(), mvArgs);
        return;
    }

    if(info[0]->IsArray())
    {
        // if first argument is an array of players this event will be sent to every player in array
        v8::Local<v8::Array> arr = info[0].As<v8::Array>();
        Array<Ref<IPlayer>> targets;
        targets.Reserve(arr->Length());

        for(int i = 0; i < arr->Length(); ++i)
        {
            Ref<IPlayer> player;
            v8::Local<v8::Value> ply;
            V8_CHECK(arr->Get(ctx, i).ToLocal(&ply), "Invalid player in emitClient players array");
            V8Entity* v8Player = V8Entity::Get(ply);

            V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player inside array expected");
            targets.Push(v8Player->GetHandle().As<IPlayer>());
        }

        ICore::Instance().TriggerClientEvent(targets, eventName.ToString(), mvArgs);
    }
    else
    {
        // if first argument is not null and not an array this event gets sent to the specific player
        V8Entity* v8Player = V8Entity::Get(info[0]);
        V8_CHECK(v8Player && v8Player->GetHandle()->GetType() == alt::IBaseObject::Type::PLAYER, "player or null expected");

        ICore::Instance().TriggerClientEvent(v8Player->GetHandle().As<IPlayer>(), eventName.ToString(), mvArgs);
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
        alt::MValueByteArray result = V8Helpers::V8ToRawBytes(info[i]);
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

static void GetResourceMain(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);

    V8_CHECK(resource, "Resource does not exist");

    V8_RETURN_STRING(resource->GetMain().CStr());
}

static void GetResourcePath(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);

    V8_CHECK(resource, "Resource does not exist");

    V8_RETURN_STRING(resource->GetPath().CStr());
}

static void HashServerPassword(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, password);

    V8_RETURN_UINT64(alt::ICore::Instance().HashServerPassword(password));
}

extern V8Class v8Player, v8Vehicle, v8Blip, v8AreaBlip, v8RadiusBlip, v8PointBlip, v8Checkpoint, v8VoiceChannel, v8Colshape, v8ColshapeCylinder, v8ColshapeSphere, v8ColshapeCircle,
  v8ColshapeCuboid, v8ColshapeRectangle;

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
                        v8ColshapeRectangle },
                      [](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports) {
                          v8::Isolate* isolate = ctx->GetIsolate();

                          V8Helpers::RegisterFunc(exports, "getResourceMain", &GetResourceMain);
                          V8Helpers::RegisterFunc(exports, "getResourcePath", &GetResourcePath);

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

                          V8_OBJECT_SET_STRING(exports, "rootDir", alt::ICore::Instance().GetRootDirectory());
                          V8_OBJECT_SET_INT(exports, "defaultDimension", alt::DEFAULT_DIMENSION);
                          V8_OBJECT_SET_INT(exports, "globalDimension", alt::GLOBAL_DIMENSION);
                      });
