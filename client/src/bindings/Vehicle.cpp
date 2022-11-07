
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

using namespace alt;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    std::ostringstream ss;
    ss << "Vehicle{ id: " << std::to_string(vehicle->GetID()) << ", model: " << std::to_string((uint64_t)vehicle->GetModel()) << " }";

    V8_RETURN_STRING(ss.str());
}

static void HandlingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_NEW_ARGS(args);
    V8_ADD_ARG(args, info.This());

    extern V8Class v8Handling;
    V8_RETURN(v8Handling.New(ctx, args));
}

static void NeonGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    bool left, right, front, back;
    vehicle->GetNeonActive(&left, &right, &front, &back);

    V8_NEW_OBJECT(neonActive);
    V8_OBJECT_SET_BOOLEAN(neonActive, "left", left);
    V8_OBJECT_SET_BOOLEAN(neonActive, "right", right);
    V8_OBJECT_SET_BOOLEAN(neonActive, "front", front);
    V8_OBJECT_SET_BOOLEAN(neonActive, "back", back);

    V8_RETURN(neonActive);
}

static void ToggleExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, extraID);
    V8_ARG_TO_BOOLEAN(2, toggle);
    vehicle->ToggleExtra(extraID, toggle);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllVehicles()->Clone());
}

static void StreamedInGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    auto streamedIn = CV8ScriptRuntime::Instance().GetStreamedInVehicles();
    auto arr = v8::Array::New(isolate, streamedIn.size());
    int i = 0;
    for(auto kv : streamedIn)
    {
        arr->Set(ctx, i, resource->GetOrCreateEntity(kv.second, "Vehicle")->GetJSVal(isolate));
        i++;
    }

    V8_RETURN(arr);
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid));
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IEntity* entity = alt::ICore::Instance().GetEntityByID(id);

    if(entity && entity->GetType() == alt::IEntity::Type::VEHICLE)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void IndicatorLightsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetLightsIndicator());
}

static void IndicatorLightsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, indicatorLights);
    vehicle->SetLightsIndicator(indicatorLights);
}

static void GetWheelCamber(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelCamber(wheel));
}

static void SetWheelCamber(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelCamber(wheel, value);
}

static void GetWheelTrackWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelTrackWidth(wheel));
}

static void SetWheelTrackWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelTrackWidth(wheel, value);
}

static void GetWheelHeight(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelHeight(wheel));
}

static void SetWheelHeight(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelHeight(wheel, value);
}

static void GetWheelTyreRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelTyreRadius(wheel));
}

static void SetWheelTyreRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelTyreRadius(wheel, value);
}

static void GetWheelRimRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelRimRadius(wheel));
}

static void SetWheelRimRadius(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelRimRadius(wheel, value);
}

static void GetWheelTyreWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_NUMBER(vehicle->GetWheelTyreWidth(wheel));
}

static void SetWheelTyreWidth(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, wheel);
    V8_ARG_TO_NUMBER(2, value);
    vehicle->SetWheelTyreWidth(wheel, value);
}

static void GetWheelSurfaceMaterial(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, wheel);
    V8_RETURN_UINT(vehicle->GetWheelSurfaceMaterial(wheel));
}

extern V8Class v8Entity;
extern V8Class v8Vehicle("Vehicle",
                         v8Entity,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                             v8::Isolate* isolate = v8::Isolate::GetCurrent();

                             V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

                             V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                             V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

                             V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                             V8Helpers::SetStaticAccessor(isolate, tpl, "streamedIn", &StreamedInGetter);

                             // Common getters
                             V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetWheelSpeed>(isolate, tpl, "speed");
                             V8Helpers::SetAccessor<IVehicle, uint16_t, &IVehicle::GetCurrentGear, &IVehicle::SetCurrentGear>(isolate, tpl, "gear");
                             V8Helpers::SetAccessor<IVehicle, uint16_t, &IVehicle::GetMaxGear>(isolate, tpl, "maxGear");
                             V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetCurrentRPM>(isolate, tpl, "rpm");
                             V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWheelsCount>(isolate, tpl, "wheelsCount");
                             V8Helpers::SetAccessor<IVehicle, alt::Vector3f, &IVehicle::GetSpeedVector>(isolate, tpl, "speedVector");
                             V8Helpers::SetAccessor(isolate, tpl, "handling", &HandlingGetter);
                             V8Helpers::SetMethod(isolate, tpl, "toggleExtra", ToggleExtra);
                             V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetLightsIndicator, &IVehicle::SetLightsIndicator>(isolate, tpl, "indicatorLights");
                             V8Helpers::SetAccessor<IVehicle, Vector3f, &IVehicle::GetVelocity>(isolate, tpl, "velocity");
                             V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetSeatCount>(isolate, tpl, "seatCount");
                             V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetLockState>(isolate, tpl, "lockState");
                             V8Helpers::SetAccessor<IVehicle, int32_t, &IVehicle::GetPetrolTankHealth>(isolate, tpl, "petrolTankHealth");

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

                             V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetEngineTemperature, &IVehicle::SetEngineTemperature>(isolate, tpl, "engineTemperature");
                             V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetFuelLevel, &IVehicle::SetFuelLevel>(isolate, tpl, "fuelLevel");
                             V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetOilLevel, &IVehicle::SetOilLevel>(isolate, tpl, "oilLevel");
                             V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsEngineOn>(isolate, tpl, "engineOn");

                             V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetEngineLightState, &IVehicle::SetEngineLightState>(isolate, tpl, "engineLight");
                             V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetAbsLightState, &IVehicle::SetAbsLightState>(isolate, tpl, "absLight");
                             V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetPetrolLightState, &IVehicle::SetPetrolLightState>(isolate, tpl, "petrolLight");
                             V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetOilLightState, &IVehicle::SetOilLightState>(isolate, tpl, "oilLight");
                             V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetBatteryLightState, &IVehicle::SetBatteryLightState>(isolate, tpl, "batteryLight");
                             V8Helpers::SetMethod<IVehicle, &IVehicle::ResetDashboardLights>(isolate, tpl, "resetDashboardLights");

                             V8Helpers::SetMethod(isolate, tpl, "getWheelSurfaceMaterial", GetWheelSurfaceMaterial);

                             /*GETTERS BELOW ARE UNIMPLEMENTED
                             V8Helpers::SetAccessor(isolate, tpl, "isDestroyed", &IsDestroyedGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "driver", &DriverGetter);

                             // Appearance getters
                             V8Helpers::SetAccessor(isolate, tpl, "modKitsCount", &ModKitsCountGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "modKit", &ModKitGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "hasCustomPrimaryColor", &IsPrimaryColorRGBGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "primaryColor", &PrimaryColorGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "customPrimaryColor", &PrimaryColorRGBGetter);
                             //V8Helpers::SetAccessor(isolate, tpl, "hasCustomSecondaryColor", &IsSecondaryColorRGBGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "secondaryColor", &SecondaryColorGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "customSecondaryColor", &SecondaryColorRGBGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "pearlColor", &PearlColorGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "wheelColor", &WheelColorGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "interiorColor", &InteriorColorGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "dashboardColor", &DashboardColorGetter);
                             //V8Helpers::SetAccessor(isolate, tpl, "hasCustomTireSmokeColor", &IsTireSmokeColorCustomGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "tireSmokeColor", &TireSmokeColorGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "wheelType", &WheelTypeGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "frontWheels", &WheelVariationGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "rearWheels", &RearWheelVariationGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "customTires", &IsCustomTiresGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "darkness", &SpecialDarknessGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "numberPlateIndex", &NumberplateIndexGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "numberPlateText", &NumberplateTextGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "windowTint", &WindowTintGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "dirtLevel", &DirtLevelGetter);
                             //V8Helpers::SetAccessor(isolate, tpl, "neonActive", &IsNeonActiveGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "neon", &NeonGetter);
                             V8Helpers::SetAccessor(isolate, tpl, "neonColor", &NeonColorGetter);*/
                         });
