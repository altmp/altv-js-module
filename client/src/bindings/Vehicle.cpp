
#include "V8Helpers.h"
#include "V8BindHelpers.h"
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

    V8_RETURN_STRING(ss.str().c_str());
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
        arr->Set(ctx, i, resource->GetOrCreateEntity(kv.second.Get(), "Vehicle")->GetJSVal(isolate));
        i++;
    }

    V8_RETURN(arr);
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid).As<alt::IVehicle>());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, id);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByID(id).As<alt::IVehicle>());
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

extern V8Class v8Entity;
extern V8Class v8Vehicle("Vehicle", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8::SetMethod(isolate, tpl, "toString", ToString);

    V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

    V8::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8::SetStaticAccessor(isolate, tpl, "streamedIn", &StreamedInGetter);

    // Common getters
    V8::SetAccessor<IVehicle, float, &IVehicle::GetWheelSpeed>(isolate, tpl, "speed");
    V8::SetAccessor<IVehicle, uint16_t, &IVehicle::GetCurrentGear, &IVehicle::SetCurrentGear>(isolate, tpl, "gear");
    V8::SetAccessor<IVehicle, uint16_t, &IVehicle::GetMaxGear>(isolate, tpl, "maxGear");
    V8::SetAccessor<IVehicle, float, &IVehicle::GetCurrentRPM>(isolate, tpl, "rpm");
    V8::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWheelsCount>(isolate, tpl, "wheelsCount");
    V8::SetAccessor<IVehicle, alt::Vector3f, &IVehicle::GetSpeedVector>(isolate, tpl, "speedVector");
    V8::SetAccessor(isolate, tpl, "handling", &HandlingGetter);
    V8::SetMethod(isolate, tpl, "toggleExtra", ToggleExtra);
    V8::SetAccessor<IVehicle, uint8_t, &IVehicle::GetLightsIndicator, &IVehicle::SetLightsIndicator>(isolate, tpl, "indicatorLights");
    V8::SetAccessor<IVehicle, Vector3f, &IVehicle::GetVelocity>(isolate, tpl, "velocity");

    /*GETTERS BELOW ARE UNIMPLEMENTED
    V8::SetAccessor(isolate, tpl, "isDestroyed", &IsDestroyedGetter);
    V8::SetAccessor(isolate, tpl, "driver", &DriverGetter);

    // Appearance getters
    V8::SetAccessor(isolate, tpl, "modKitsCount", &ModKitsCountGetter);
    V8::SetAccessor(isolate, tpl, "modKit", &ModKitGetter);
    V8::SetAccessor(isolate, tpl, "hasCustomPrimaryColor", &IsPrimaryColorRGBGetter);
    V8::SetAccessor(isolate, tpl, "primaryColor", &PrimaryColorGetter);
    V8::SetAccessor(isolate, tpl, "customPrimaryColor", &PrimaryColorRGBGetter);
    //V8::SetAccessor(isolate, tpl, "hasCustomSecondaryColor", &IsSecondaryColorRGBGetter);
    V8::SetAccessor(isolate, tpl, "secondaryColor", &SecondaryColorGetter);
    V8::SetAccessor(isolate, tpl, "customSecondaryColor", &SecondaryColorRGBGetter);
    V8::SetAccessor(isolate, tpl, "pearlColor", &PearlColorGetter);
    V8::SetAccessor(isolate, tpl, "wheelColor", &WheelColorGetter);
    V8::SetAccessor(isolate, tpl, "interiorColor", &InteriorColorGetter);
    V8::SetAccessor(isolate, tpl, "dashboardColor", &DashboardColorGetter);
    //V8::SetAccessor(isolate, tpl, "hasCustomTireSmokeColor", &IsTireSmokeColorCustomGetter);
    V8::SetAccessor(isolate, tpl, "tireSmokeColor", &TireSmokeColorGetter);
    V8::SetAccessor(isolate, tpl, "wheelType", &WheelTypeGetter);
    V8::SetAccessor(isolate, tpl, "frontWheels", &WheelVariationGetter);
    V8::SetAccessor(isolate, tpl, "rearWheels", &RearWheelVariationGetter);
    V8::SetAccessor(isolate, tpl, "customTires", &IsCustomTiresGetter);
    V8::SetAccessor(isolate, tpl, "darkness", &SpecialDarknessGetter);
    V8::SetAccessor(isolate, tpl, "numberPlateIndex", &NumberplateIndexGetter);
    V8::SetAccessor(isolate, tpl, "numberPlateText", &NumberplateTextGetter);
    V8::SetAccessor(isolate, tpl, "windowTint", &WindowTintGetter);
    V8::SetAccessor(isolate, tpl, "dirtLevel", &DirtLevelGetter);
    //V8::SetAccessor(isolate, tpl, "neonActive", &IsNeonActiveGetter);
    V8::SetAccessor(isolate, tpl, "neon", &NeonGetter);
    V8::SetAccessor(isolate, tpl, "neonColor", &NeonColorGetter);*/
});
