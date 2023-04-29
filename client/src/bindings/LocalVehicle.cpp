#include "V8Helpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

#include "cpp-sdk/script-objects/ILocalVehicle.h"
using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(6);

    uint32_t modelHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, modelName);
        modelHash = alt::ICore::Instance().Hash(modelName);
    }
    else
    {
        V8_ARG_TO_UINT(1, _modelHash);
        modelHash = _modelHash;
    }
    V8_ARG_TO_INT32(2, dimension);
    V8_ARG_TO_VECTOR3(3, pos);
    V8_ARG_TO_VECTOR3(4, rot);
    V8_ARG_TO_BOOLEAN(5, useStreaming);
    V8_ARG_TO_UINT(6, streamingDistance);

    auto vehicle = alt::ICore::Instance().CreateLocalVehicle(modelHash, dimension, pos, rot, useStreaming, streamingDistance, resource->GetResource());
    V8_BIND_BASE_OBJECT(vehicle, "Failed to create localVehicle");
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::LOCAL_VEHICLE, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::LOCAL_VEHICLE)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);

    alt::IWorldObject* entity = alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid);

    if(entity && (entity->GetType() == alt::IWorldObject::Type::LOCAL_VEHICLE))
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

extern V8Class v8WorldObject;
extern V8Class v8LocalVehicle("LocalVehicle",
                              v8WorldObject,
                              Constructor,
                              [](v8::Local<v8::FunctionTemplate> tpl)
                              {
                                  v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                  V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                                  V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetID>(isolate, tpl, "id");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetModel>(isolate, tpl, "model");
                                  V8Helpers::SetAccessor<ILocalVehicle, Rotation, &ILocalVehicle::GetRotation, &ILocalVehicle::SetRotation>(isolate, tpl, "rot");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsVisible, &ILocalVehicle::SetVisible>(isolate, tpl, "visible");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetScriptID>(isolate, tpl, "scriptID");

                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetRemoteID>(isolate, tpl, "remoteId");
                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsRemote>(isolate, tpl, "isRemote");
                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsStreamedIn>(isolate, tpl, "isStreamedIn");
                              });
