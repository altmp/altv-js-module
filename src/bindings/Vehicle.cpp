
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"

#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

using namespace alt;

static void HandlingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    std::vector<v8::Local<v8::Value>> args{
        info.This()};

    extern V8Class v8Handling;
    V8_RETURN(v8Handling.New(ctx, args));
}

static void SpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_NUMBER(vehicle->GetWheelSpeed());
}

static void GearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetCurrentGear());
}

static void RPMGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_NUMBER(vehicle->GetCurrentRPM());
}

static void WheelsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetWheelsCount());
}

static void SpeedVectorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
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
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsDestroyed());
}

static void ModKitsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetModKitsCount());
}

static void ModKitGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetModKit());
}

static void IsPrimaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsPrimaryColorRGB());
}

static void PrimaryColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetPrimaryColor());
}

static void PrimaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateRGBA(vehicle->GetPrimaryColorRGB()));
}

static void IsSecondaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsSecondaryColorRGB());
}

static void SecondaryColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetSecondaryColor());
}

static void SecondaryColorRGBGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN(resource->CreateRGBA(vehicle->GetSecondaryColorRGB()));
}

static void PearlColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetPearlColor());
}

static void WheelColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetWheelColor());
}

static void InteriorColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetInteriorColor());
}

static void DashboardColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetDashboardColor());
}

static void IsTireSmokeColorCustomGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
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
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetWheelType());
}

static void WheelVariationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetWheelVariation());
}

static void RearWheelVariationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetRearWheelVariation());
}

static void IsCustomTiresGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->GetCustomTires());
}

static void SpecialDarknessGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetSpecialDarkness());
}

static void NumberplateIndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetNumberplateIndex());
}

static void NumberplateTextGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_STRING(vehicle->GetNumberplateText().CStr());
}

static void WindowTintGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetWindowTint());
}

static void DirtLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetDirtLevel());
}

static void IsNeonActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
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

    v8::Local<v8::Object> neonActive = v8::Object::New(isolate);

    bool left, right, front, back;
    vehicle->GetNeonActive(&left, &right, &front, &back);

    neonActive->Set(ctx, v8::String::NewFromUtf8(isolate, "left").ToLocalChecked(), v8::Boolean::New(isolate, left));
    neonActive->Set(ctx, v8::String::NewFromUtf8(isolate, "right").ToLocalChecked(), v8::Boolean::New(isolate, right));
    neonActive->Set(ctx, v8::String::NewFromUtf8(isolate, "front").ToLocalChecked(), v8::Boolean::New(isolate, front));
    neonActive->Set(ctx, v8::String::NewFromUtf8(isolate, "back").ToLocalChecked(), v8::Boolean::New(isolate, back));

    V8_RETURN(neonActive);
}

static void LiveryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetLivery());
}

static void RoofLiveryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetRoofLivery());
}

static void EngineOnGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsEngineOn());
}

static void HandbrakeActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsHandbrakeActive());
}

static void HeadlightColorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetHeadlightColor());
}

static void RadioStationIndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetRadioStationIndex());
}

static void IsSirenActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsSirenActive());
}

static void LockStateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetLockState());
}

static void IsDaylightOnGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsDaylightOn());
}

static void IsNightlightOnGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsNightlightOn());
}

static void RoofStateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetRoofState());
}

static void IsFlamethrowerActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsFlamethrowerActive());
}

static void LightsMultiplierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_NUMBER(vehicle->GetLightsMultiplier());
}

static void EngineHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetEngineHealth());
}

static void PetrolTankHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetPetrolTankHealth());
}

static void RepairsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetRepairsCount());
}

static void BodyHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetBodyHealth());
}

static void BodyAdditionalHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_INTEGER(vehicle->GetBodyAdditionalHealth());
}

static void HasArmoredWindowsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->HasArmoredWindows());
}

static void IsManualEngineControlGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsManualEngineControl());
}

static void IsHandlingModifiedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(vehicle, alt::IVehicle);

    V8_RETURN_BOOLEAN(vehicle->IsHandlingModified());
}

// static void GravityGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
//     V8_GET_ISOLATE_CONTEXT();

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
//     V8_GET_ISOLATE_CONTEXT();
//     V8_CHECK(val->IsNumber(), "val must be a number");

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
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

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Array> arr = v8::Array::New(isolate);

    uint16_t i = 0;
    for (auto vehicle : alt::ICore::Instance().GetVehicles())
    {
        if (vehicle)
            arr->Set(ctx, i++, resource->GetOrCreateEntity(vehicle.Get(), "Vehicle")->GetJSVal(isolate));
    };

    V8_RETURN(arr);
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid).As<alt::IVehicle>());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, id);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByID(id).As<alt::IVehicle>());
}

extern V8Class v8Entity;
extern V8Class v8Vehicle("Vehicle", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

    V8::SetStaticAccessor(isolate, tpl, "all", &AllGetter);

    // Common getters
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "speed").ToLocalChecked(), &SpeedGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "gear").ToLocalChecked(), &GearGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "rpm").ToLocalChecked(), &RPMGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "wheelsCount").ToLocalChecked(), &WheelsCountGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "speedVector").ToLocalChecked(), &SpeedVectorGetter);
    // proto->SetAccessor(v8::String::NewFromUtf8(isolate, "gravity", &GravityGetter).ToLocalChecked(), &GravitySetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handling").ToLocalChecked(), &HandlingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isDestroyed").ToLocalChecked(), &IsDestroyedGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "driver").ToLocalChecked(), &DriverGetter);

    // Appearance getters
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "modKitsCount").ToLocalChecked(), &ModKitsCountGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "modKit").ToLocalChecked(), &ModKitGetter);
    //proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hasCustomPrimaryColor").ToLocalChecked(), &IsPrimaryColorRGBGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "primaryColor").ToLocalChecked(), &PrimaryColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "customPrimaryColor").ToLocalChecked(), &PrimaryColorRGBGetter);
    //proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hasCustomSecondaryColor").ToLocalChecked(), &IsSecondaryColorRGBGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "secondaryColor").ToLocalChecked(), &SecondaryColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "customSecondaryColor").ToLocalChecked(), &SecondaryColorRGBGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "pearlColor").ToLocalChecked(), &PearlColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "wheelColor").ToLocalChecked(), &WheelColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "interiorColor").ToLocalChecked(), &InteriorColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "dashboardColor").ToLocalChecked(), &DashboardColorGetter);
    //proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hasCustomTireSmokeColor").ToLocalChecked(), &IsTireSmokeColorCustomGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tireSmokeColor").ToLocalChecked(), &TireSmokeColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "wheelType").ToLocalChecked(), &WheelTypeGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "frontWheels").ToLocalChecked(), &WheelVariationGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "rearWheels").ToLocalChecked(), &RearWheelVariationGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "customTires").ToLocalChecked(), &IsCustomTiresGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "darkness").ToLocalChecked(), &SpecialDarknessGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "numberPlateIndex").ToLocalChecked(), &NumberplateIndexGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "numberPlateText").ToLocalChecked(), &NumberplateTextGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "windowTint").ToLocalChecked(), &WindowTintGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "dirtLevel").ToLocalChecked(), &DirtLevelGetter);
    //proto->SetAccessor(v8::String::NewFromUtf8(isolate, "neonActive").ToLocalChecked(), &IsNeonActiveGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "neon").ToLocalChecked(), &NeonGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "neonColor").ToLocalChecked(), &NeonColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "livery").ToLocalChecked(), &LiveryGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "roofLivery").ToLocalChecked(), &RoofLiveryGetter);

    // Gamestate getters
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "engineOn").ToLocalChecked(), &EngineOnGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handbrakeActive").ToLocalChecked(), &HandbrakeActiveGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "headlightColor").ToLocalChecked(), &HeadlightColorGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "activeRadioStation").ToLocalChecked(), &RadioStationIndexGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "sirenActive").ToLocalChecked(), &IsSirenActiveGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lockState").ToLocalChecked(), &LockStateGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "daylightOn").ToLocalChecked(), &IsDaylightOnGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "nightlightOn").ToLocalChecked(), &IsNightlightOnGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "roofState").ToLocalChecked(), &RoofStateGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "flamethrowerActive").ToLocalChecked(), &IsFlamethrowerActiveGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lightsMultiplier").ToLocalChecked(), &LightsMultiplierGetter);

    // Health getters
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "engineHealth").ToLocalChecked(), &EngineHealthGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "petrolTankHealth").ToLocalChecked(), &PetrolTankHealthGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "repairsCount").ToLocalChecked(), &RepairsCountGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "bodyHealth").ToLocalChecked(), &BodyHealthGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "bodyAdditionalHealth").ToLocalChecked(), &BodyAdditionalHealthGetter);

    // Damage getters
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hasArmoredWindows").ToLocalChecked(), &HasArmoredWindowsGetter);

    // Script getters
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "manualEngineControl").ToLocalChecked(), &IsManualEngineControlGetter);
    //proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handlingModified").ToLocalChecked(), &IsHandlingModifiedGetter);
});
