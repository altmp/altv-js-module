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

static void ToggleExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, extraID);
    V8_ARG_TO_BOOLEAN(2, toggle);
    vehicle->ToggleExtra(extraID, toggle);
}

static void GetWheelCamber(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelCamber(wheel));
}

static void SetWheelCamber(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelCamber(wheel, value);
}

static void GetWheelTrackWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelTrackWidth(wheel));
}

static void SetWheelTrackWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelTrackWidth(wheel, value);
}

static void GetWheelHeight(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelHeight(wheel));
}

static void SetWheelHeight(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelHeight(wheel, value);
}

static void GetWheelTyreRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelTyreRadius(wheel));
}

static void SetWheelTyreRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelTyreRadius(wheel, value);
}

static void GetWheelRimRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelRimRadius(wheel));
}

static void SetWheelRimRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelRimRadius(wheel, value);
}

static void GetWheelTyreWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelTyreWidth(wheel));
}

static void SetWheelTyreWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelTyreWidth(wheel, value);
}

static void GetWheelSurfaceMaterial(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::ILocalVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_UINT(vehicle->GetWheelSurfaceMaterial(wheel));
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

                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetModel>(isolate, tpl, "model");
                                  V8Helpers::SetAccessor<ILocalVehicle, Rotation, &ILocalVehicle::GetRotation, &ILocalVehicle::SetRotation>(isolate, tpl, "rot");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsVisible, &ILocalVehicle::SetVisible>(isolate, tpl, "visible");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetScriptID>(isolate, tpl, "scriptID");

                                  V8Helpers::SetAccessor<ILocalVehicle, uint32_t, &ILocalVehicle::GetRemoteID>(isolate, tpl, "remoteId");
                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsRemote>(isolate, tpl, "isRemote");
                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsStreamedIn>(isolate, tpl, "isStreamedIn");

                                  // Common getters
                                  V8Helpers::SetAccessor<ILocalVehicle, float, &ILocalVehicle::GetWheelSpeed>(isolate, tpl, "speed");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint16_t, &ILocalVehicle::GetCurrentGear, &ILocalVehicle::SetCurrentGear>(isolate, tpl, "gear");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint16_t, &ILocalVehicle::GetMaxGear>(isolate, tpl, "maxGear");
                                  V8Helpers::SetAccessor<ILocalVehicle, float, &ILocalVehicle::GetCurrentRPM, &ILocalVehicle::SetCurrentRPM>(isolate, tpl, "rpm");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint8_t, &ILocalVehicle::GetWheelsCount>(isolate, tpl, "wheelsCount");
                                  V8Helpers::SetAccessor<ILocalVehicle, alt::Vector3f, &ILocalVehicle::GetSpeedVector>(isolate, tpl, "speedVector");

                                  V8Helpers::SetAccessor<ILocalVehicle, bool, &ILocalVehicle::IsEngineOn>(isolate, tpl, "engineOn");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint8_t, &ILocalVehicle::GetLockState>(isolate, tpl, "lockState");
                                  V8Helpers::SetAccessor<ILocalVehicle, int32_t, &ILocalVehicle::GetPetrolTankHealth>(isolate, tpl, "petrolTankHealth");

                                  V8Helpers::SetMethod(isolate, tpl, "toggleExtra", ToggleExtra);
                                  V8Helpers::SetAccessor<ILocalVehicle, uint8_t, &ILocalVehicle::GetLightsIndicator, &ILocalVehicle::SetLightsIndicator>(isolate, tpl, "indicatorLights");
                                  V8Helpers::SetAccessor<ILocalVehicle, uint8_t, &ILocalVehicle::GetSeatCount>(isolate, tpl, "seatCount");

                                  V8Helpers::SetMethod(isolate, tpl, "getWheelCamber", GetWheelCamber);
                                  V8Helpers::SetMethod(isolate, tpl, "setWheelCamber", SetWheelCamber);
                                  V8Helpers::SetMethod(isolate, tpl, "getWheelTrackWidth", GetWheelTrackWidth);
                                  V8Helpers::SetMethod(isolate, tpl, "setWheelTrackWidth", SetWheelTrackWidth);
                                  V8Helpers::SetMethod(isolate, tpl, "getWheelHeight", GetWheelHeight);
                                  V8Helpers::SetMethod(isolate, tpl, "setWheelHeight", SetWheelHeight);
                                  V8Helpers::SetMethod(isolate, tpl, "getWheelTyreRadius", GetWheelTyreRadius);
                                  V8Helpers::SetMethod(isolate, tpl, "setWheelTyreRadius", SetWheelTyreRadius);
                                  V8Helpers::SetMethod(isolate, tpl, "getWheelRimRadius", GetWheelRimRadius);
                                  V8Helpers::SetMethod(isolate, tpl, "setWheelRimRadius", SetWheelRimRadius);
                                  V8Helpers::SetMethod(isolate, tpl, "getWheelTyreWidth", GetWheelTyreWidth);
                                  V8Helpers::SetMethod(isolate, tpl, "setWheelTyreWidth", SetWheelTyreWidth);
                                  V8Helpers::SetMethod(isolate, tpl, "getWheelSurfaceMaterial", GetWheelSurfaceMaterial);

                                  V8Helpers::SetAccessor<ILocalVehicle, float, &ILocalVehicle::GetEngineTemperature, &ILocalVehicle::SetEngineTemperature>(isolate, tpl, "engineTemperature");
                                  V8Helpers::SetAccessor<ILocalVehicle, float, &ILocalVehicle::GetFuelLevel, &ILocalVehicle::SetFuelLevel>(isolate, tpl, "fuelLevel");
                                  V8Helpers::SetAccessor<ILocalVehicle, float, &ILocalVehicle::GetOilLevel, &ILocalVehicle::SetOilLevel>(isolate, tpl, "oilLevel");
                              });
