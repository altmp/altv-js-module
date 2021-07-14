
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

using namespace alt;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

    auto vehicle = info.This();
    V8_OBJECT_GET_INTEGER(vehicle, "id", id);
	V8_OBJECT_GET_NUMBER(vehicle, "model", model);

	std::ostringstream ss;
	ss << "Vehicle{ id: " << std::to_string(id) << ", model: " << std::to_string((uint64_t)model) << " }";

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

static void SpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_NUMBER(vehicle->GetWheelSpeed());
}

static void GearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetCurrentGear());
}

static void GearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, gear);
    vehicle->SetCurrentGear(gear);
}

static void RPMGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_NUMBER(vehicle->GetCurrentRPM());
}

static void WheelsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetWheelsCount());
}

static void SpeedVectorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateVector3(vehicle->GetSpeedVector()));
}

static void DriverGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BASE_OBJECT(vehicle->GetDriver());
}

static void IsDestroyedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsDestroyed());
}

static void ModKitsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetModKitsCount());
}

static void ModKitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetModKit());
}

static void IsPrimaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsPrimaryColorRGB());
}

static void PrimaryColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetPrimaryColor());
}

static void PrimaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateRGBA(vehicle->GetPrimaryColorRGB()));
}

static void IsSecondaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsSecondaryColorRGB());
}

static void SecondaryColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetSecondaryColor());
}

static void SecondaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateRGBA(vehicle->GetSecondaryColorRGB()));
}

static void PearlColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetPearlColor());
}

static void WheelColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetWheelColor());
}

static void InteriorColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetInteriorColor());
}

static void DashboardColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetDashboardColor());
}

static void IsTireSmokeColorCustomGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsTireSmokeColorCustom());
}

static void TireSmokeColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateRGBA(vehicle->GetTireSmokeColor()));
}

static void WheelTypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetWheelType());
}

static void WheelVariationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetWheelVariation());
}

static void RearWheelVariationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetRearWheelVariation());
}

static void IsCustomTiresGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->GetCustomTires());
}

static void SpecialDarknessGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetSpecialDarkness());
}

static void NumberplateIndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetNumberplateIndex());
}

static void NumberplateTextGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_STRING(vehicle->GetNumberplateText().CStr());
}

static void WindowTintGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetWindowTint());
}

static void DirtLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetDirtLevel());
}

static void IsNeonActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsNeonActive());
}

static void NeonColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateRGBA(vehicle->GetNeonColor()));
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

static void LiveryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetLivery());
}

static void RoofLiveryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetRoofLivery());
}

static void EngineOnGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsEngineOn());
}

static void HandbrakeActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsHandbrakeActive());
}

static void HeadlightColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetHeadlightColor());
}

static void RadioStationIndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetRadioStationIndex());
}

static void IsSirenActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsSirenActive());
}

static void LockStateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetLockState());
}

static void IsDaylightOnGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsDaylightOn());
}

static void IsNightlightOnGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsNightlightOn());
}

static void RoofStateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetRoofState());
}

static void IsFlamethrowerActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsFlamethrowerActive());
}

static void LightsMultiplierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_NUMBER(vehicle->GetLightsMultiplier());
}

static void EngineHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetEngineHealth());
}

static void PetrolTankHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetPetrolTankHealth());
}

static void RepairsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetRepairsCount());
}

static void BodyHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetBodyHealth());
}

static void BodyAdditionalHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetBodyAdditionalHealth());
}

static void HasArmoredWindowsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->HasArmoredWindows());
}

static void IsManualEngineControlGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsManualEngineControl());
}

static void IsHandlingModifiedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsHandlingModified());
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

// static void GravityGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
//     V8_GET_ISOLATE();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     auto vehicle = _this->GetHandle().As<alt::IVehicle>()->GetGameVehicle();
//     V8_CHECK(vehicle, "Could not retrieve game vehicle");

//     Log::Debug("GRAVITY", vehicle->gravity);
//     auto ret = v8::Number::New(isolate, vehicle->gravity);
//     Log::Debug("RET GRAVITY", ret->NumberValue(ctx).ToChecked());
//     info.GetReturnValue().Set(ret);
// }

// static void GravitySetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
//     V8_GET_ISOLATE();
//     V8_CHECK(val->IsNumber(), "val must be a number");

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredOrMicrotaskContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(val->IsNumber(), "val needs to be a nummber (float)");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     auto vehicle = _this->GetHandle().As<alt::IVehicle>()->GetGameVehicle();
//     V8_CHECK(vehicle, "Could not retrieve game vehicle");

//     Log::Debug("GRAVITY", vehicle->gravity);
//     auto newval = val->NumberValue(ctx).ToChecked();
//     Log::Debug("NEW GRAVITY", newval);
//     vehicle->gravity = newval;
// }

static void MaxGearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INT(vehicle->GetMaxGear());
}

static void MaxGearSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_TO_INTEGER(val, maxGear);
    vehicle->SetMaxGear(maxGear);
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

static void IndicatorLightsSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
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
    V8::SetAccessor(isolate, tpl, "speed", &SpeedGetter);
    V8::SetAccessor(isolate, tpl, "gear", &GearGetter, &GearSetter);
    V8::SetAccessor(isolate, tpl, "maxGear", &MaxGearGetter, &MaxGearSetter);
    V8::SetAccessor(isolate, tpl, "rpm", &RPMGetter);
    V8::SetAccessor(isolate, tpl, "wheelsCount", &WheelsCountGetter);
    V8::SetAccessor(isolate, tpl, "speedVector", &SpeedVectorGetter);
    // proto->SetAccessor(v8::String::NewFromUtf8(isolate, "gravity", &GravityGetter).ToLocalChecked(), &GravitySetter);
    V8::SetAccessor(isolate, tpl, "handling", &HandlingGetter);
    V8::SetMethod(isolate, tpl, "toggleExtra", ToggleExtra);
    V8::SetAccessor(isolate, tpl, "indicatorLights", &IndicatorLightsGetter, &IndicatorLightsSetter);

    /* GETTERS BELOW ARE UNIMPLEMENTED
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
    V8::SetAccessor(isolate, tpl, "neonColor", &NeonColorGetter);
    V8::SetAccessor(isolate, tpl, "livery", &LiveryGetter);
    V8::SetAccessor(isolate, tpl, "roofLivery", &RoofLiveryGetter);

    // Gamestate getters
    V8::SetAccessor(isolate, tpl, "engineOn", &EngineOnGetter);
    V8::SetAccessor(isolate, tpl, "handbrakeActive", &HandbrakeActiveGetter);
    V8::SetAccessor(isolate, tpl, "headlightColor", &HeadlightColorGetter);
    V8::SetAccessor(isolate, tpl, "activeRadioStation", &RadioStationIndexGetter);
    V8::SetAccessor(isolate, tpl, "sirenActive", &IsSirenActiveGetter);
    V8::SetAccessor(isolate, tpl, "lockState", &LockStateGetter);
    V8::SetAccessor(isolate, tpl, "daylightOn", &IsDaylightOnGetter);
    V8::SetAccessor(isolate, tpl, "nightlightOn", &IsNightlightOnGetter);
    V8::SetAccessor(isolate, tpl, "roofState", &RoofStateGetter);
    V8::SetAccessor(isolate, tpl, "flamethrowerActive", &IsFlamethrowerActiveGetter);
    V8::SetAccessor(isolate, tpl, "lightsMultiplier", &LightsMultiplierGetter);

    // Health getters
    V8::SetAccessor(isolate, tpl, "engineHealth", &EngineHealthGetter);
    V8::SetAccessor(isolate, tpl, "petrolTankHealth", &PetrolTankHealthGetter);
    V8::SetAccessor(isolate, tpl, "repairsCount", &RepairsCountGetter);
    V8::SetAccessor(isolate, tpl, "bodyHealth", &BodyHealthGetter);
    V8::SetAccessor(isolate, tpl, "bodyAdditionalHealth", &BodyAdditionalHealthGetter);

    // Damage getters
    V8::SetAccessor(isolate, tpl, "hasArmoredWindows", &HasArmoredWindowsGetter);

    // Script getters
    V8::SetAccessor(isolate, tpl, "manualEngineControl", &IsManualEngineControlGetter);
    V8::SetAccessor(isolate, tpl, "handlingModified", &IsHandlingModifiedGetter);
    */
});
