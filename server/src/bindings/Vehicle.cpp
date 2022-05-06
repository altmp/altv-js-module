#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

#include "vehicle/Appearance.h"
#include "vehicle/GameState.h"
#include "vehicle/Health.h"
#include "vehicle/Damage.h"
#include "vehicle/ScriptGameState.h"

using namespace V8Helpers::Vehicle;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(3, 7);

    V8_CHECK(info[0]->IsString() || info[0]->IsNumber(), "string or number expected");

    uint32_t modelHash;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, model);
        modelHash = model;
    }
    else
    {
        V8_ARG_TO_STRING(1, model);
        modelHash = alt::ICore::Instance().Hash(model);
    }

    alt::Position pos;
    alt::Rotation rot;
    if(info.Length() == 3)
    {
        V8_ARG_TO_VECTOR3(2, position);
        V8_ARG_TO_VECTOR3(3, rotation);

        pos = position;
        rot = rotation;
    }
    else
    {
        V8_ARG_TO_NUMBER(2, x);
        V8_ARG_TO_NUMBER(3, y);
        V8_ARG_TO_NUMBER(4, z);
        V8_ARG_TO_NUMBER(5, rx);
        V8_ARG_TO_NUMBER(6, ry);
        V8_ARG_TO_NUMBER(7, rz);

        pos = { x, y, z };
        rot = { rx, ry, rz };
    }

    Ref<IVehicle> veh = alt::ICore::Instance().CreateVehicle(modelHash, pos, rot);

    V8_CHECK(veh, "Failed to create vehicle");

    resource->BindEntity(info.This(), veh.Get());
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllVehicles());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::Ref<alt::IEntity> entity = alt::ICore::Instance().GetEntityByID(id);

    if(entity && entity->GetType() == alt::IEntity::Type::VEHICLE)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void SetTrainEngineId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

    if(info[0]->IsNull())
    {
        alt::Ref<alt::IVehicle> ref;
        _this->SetTrainEngineId(ref);
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, vehicle, IVehicle, "Vehicle");
        _this->SetTrainEngineId(vehicle);
    }
}

static void SetTrainLinkedToBackwardId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

    if(info[0]->IsNull())
    {
        alt::Ref<alt::IVehicle> ref;
        _this->SetTrainLinkedToBackwardId(ref);
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, vehicle, IVehicle, "Vehicle");
        _this->SetTrainLinkedToBackwardId(vehicle);
    };
}

static void SetTrainLinkedToForwardId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

    if(info[0]->IsNull())
    {
        alt::Ref<alt::IVehicle> ref;
        _this->SetTrainLinkedToForwardId(ref);
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, vehicle, IVehicle, "Vehicle");
        _this->SetTrainLinkedToForwardId(vehicle);
    };
}

static void SetSearchLight(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

    V8_ARG_TO_BASE_OBJECT(1, spottedEntity, IEntity, "Entity");
    V8_ARG_TO_BOOLEAN(2, state)

    _this->SetSearchLight(state, spottedEntity);
}

extern V8Class v8Entity;
extern V8Class v8Vehicle("Vehicle", v8Entity, Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

    // Common getter/setters
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsDestroyed>(isolate, tpl, "destroyed");
    V8Helpers::SetAccessor<IVehicle, Ref<IPlayer>, &IVehicle::GetDriver>(isolate, tpl, "driver");
    V8Helpers::SetAccessor<IVehicle, Vector3f, &IVehicle::GetVelocity>(isolate, tpl, "velocity");

    // Appearance getters/setters
    V8Helpers::SetAccessor(isolate, tpl, "modKit", &ModKitGetter, &ModKitSetter);
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetModKitsCount>(isolate, tpl, "modKitsCount");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetPrimaryColor, &IVehicle::SetPrimaryColor>(isolate, tpl, "primaryColor");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetSecondaryColor, &IVehicle::SetSecondaryColor>(isolate, tpl, "secondaryColor");
    V8Helpers::SetAccessor<IVehicle, RGBA, &IVehicle::GetPrimaryColorRGB, &IVehicle::SetPrimaryColorRGB>(isolate, tpl, "customPrimaryColor");
    V8Helpers::SetAccessor<IVehicle, RGBA, &IVehicle::GetSecondaryColorRGB, &IVehicle::SetSecondaryColorRGB>(isolate, tpl, "customSecondaryColor");
    V8Helpers::SetAccessor<IVehicle, RGBA, &IVehicle::GetTireSmokeColor, &IVehicle::SetTireSmokeColor>(isolate, tpl, "tireSmokeColor");
    V8Helpers::SetAccessor<IVehicle, RGBA, &IVehicle::GetNeonColor, &IVehicle::SetNeonColor>(isolate, tpl, "neonColor");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetPearlColor, &IVehicle::SetPearlColor>(isolate, tpl, "pearlColor");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWheelColor, &IVehicle::SetWheelColor>(isolate, tpl, "wheelColor");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetInteriorColor, &IVehicle::SetInteriorColor>(isolate, tpl, "interiorColor");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetDashboardColor, &IVehicle::SetDashboardColor>(isolate, tpl, "dashboardColor");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetCustomTires, &IVehicle::SetCustomTires>(isolate, tpl, "customTires");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetSpecialDarkness, &IVehicle::SetSpecialDarkness>(isolate, tpl, "darkness");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWindowTint, &IVehicle::SetWindowTint>(isolate, tpl, "windowTint");
    V8Helpers::SetAccessor(isolate, tpl, "neon", &NeonActiveGetter, &NeonActiveSetter);
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetDirtLevel, &IVehicle::SetDirtLevel>(isolate, tpl, "dirtLevel");
    V8Helpers::SetAccessor<IVehicle, uint32_t, &IVehicle::GetNumberplateIndex, &IVehicle::SetNumberplateIndex>(isolate, tpl, "numberPlateIndex");
    V8Helpers::SetAccessor<IVehicle, std::string, &IVehicle::GetNumberplateText, const std::string&, &IVehicle::SetNumberplateText>(isolate, tpl, "numberPlateText");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetLivery, &IVehicle::SetLivery>(isolate, tpl, "livery");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetRoofLivery, &IVehicle::SetRoofLivery>(isolate, tpl, "roofLivery");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWheelType>(isolate, tpl, "wheelType");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWheelVariation>(isolate, tpl, "frontWheels");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetRearWheelVariation>(isolate, tpl, "rearWheels");

    // Appearance methods
    V8Helpers::SetMethod(isolate, tpl, "getModsCount", &GetModsCount);
    V8Helpers::SetMethod(isolate, tpl, "setMod", &SetMod);
    V8Helpers::SetMethod(isolate, tpl, "getMod", &GetMod);
    V8Helpers::SetMethod(isolate, tpl, "setExtra", &SetExtra);
    V8Helpers::SetMethod(isolate, tpl, "getExtra", &GetExtra);
    V8Helpers::SetMethod(isolate, tpl, "setWheels", &SetWheels);
    V8Helpers::SetMethod(isolate, tpl, "setRearWheels", &SetRearWheels);
    V8Helpers::SetMethod(isolate, tpl, "getAppearanceDataBase64", &GetAppearanceData);
    V8Helpers::SetMethod(isolate, tpl, "setAppearanceDataBase64", &SetAppearanceData);

    // Gamestate getters/setters
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsEngineOn, &IVehicle::SetEngineOn>(isolate, tpl, "engineOn");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsHandbrakeActive>(isolate, tpl, "handbrakeActive");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetHeadlightColor, &IVehicle::SetHeadlightColor>(isolate, tpl, "headlightColor");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsSirenActive, &IVehicle::SetSirenActive>(isolate, tpl, "sirenActive");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetLockState, &IVehicle::SetLockState>(isolate, tpl, "lockState");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsDaylightOn>(isolate, tpl, "daylightOn");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsNightlightOn>(isolate, tpl, "nightlightOn");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetRoofState, &IVehicle::SetRoofState>(isolate, tpl, "roofState");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsFlamethrowerActive>(isolate, tpl, "flamethrowerActive");
    V8Helpers::SetAccessor<IVehicle, uint32_t, &IVehicle::GetRadioStationIndex, &IVehicle::SetRadioStationIndex>(isolate, tpl, "activeRadioStation");
    V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetLightsMultiplier, &IVehicle::SetLightsMultiplier>(isolate, tpl, "lightsMultiplier");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsDriftMode, &IVehicle::SetDriftMode>(isolate, tpl, "driftModeEnabled");

    // Gamestate methods
    V8Helpers::SetMethod(isolate, tpl, "getDoorState", &GetDoorState);
    V8Helpers::SetMethod(isolate, tpl, "setDoorState", &SetDoorState);
    V8Helpers::SetMethod(isolate, tpl, "isWindowOpened", &GetWindowOpened);
    V8Helpers::SetMethod(isolate, tpl, "setWindowOpened", &SetWindowOpened);
    V8Helpers::SetMethod(isolate, tpl, "getGamestateDataBase64", &GetGamestateData);
    V8Helpers::SetMethod(isolate, tpl, "setGamestateDataBase64", &SetGamestateData);

    // Health getters/setters
    V8Helpers::SetAccessor<IVehicle, int32_t, &IVehicle::GetEngineHealth, &IVehicle::SetEngineHealth>(isolate, tpl, "engineHealth");
    V8Helpers::SetAccessor<IVehicle, int32_t, &IVehicle::GetPetrolTankHealth, &IVehicle::SetPetrolTankHealth>(isolate, tpl, "petrolTankHealth");
    V8Helpers::SetAccessor<IVehicle, uint32_t, &IVehicle::GetBodyHealth, &IVehicle::SetBodyHealth>(isolate, tpl, "bodyHealth");
    V8Helpers::SetAccessor<IVehicle, uint32_t, &IVehicle::GetBodyAdditionalHealth, &IVehicle::SetBodyAdditionalHealth>(isolate, tpl, "bodyAdditionalHealth");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetWheelsCount>(isolate, tpl, "wheelsCount");
    V8Helpers::SetAccessor<IVehicle, uint8_t, &IVehicle::GetRepairsCount>(isolate, tpl, "repairsCount");
    /*proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lastAttacker"), &LastAttackerGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lastAttackedWith"), &LastDamagedWithGetter);*/

    // Health methods
    V8Helpers::SetMethod(isolate, tpl, "setWheelBurst", &SetWheelBurst);
    V8Helpers::SetMethod(isolate, tpl, "isWheelBurst", &GetWheelBurst);
    V8Helpers::SetMethod(isolate, tpl, "setWheelHasTire", &SetWheelHasTire);
    V8Helpers::SetMethod(isolate, tpl, "doesWheelHasTire", &GetWheelHasTire);
    V8Helpers::SetMethod(isolate, tpl, "setWheelDetached", &SetWheelDetached);
    V8Helpers::SetMethod(isolate, tpl, "isWheelDetached", &GetIsWheelDetached);
    V8Helpers::SetMethod(isolate, tpl, "setWheelOnFire", &SetWheelOnFire);
    V8Helpers::SetMethod(isolate, tpl, "isWheelOnFire", &GetIsWheelOnFire);
    V8Helpers::SetMethod(isolate, tpl, "setWheelHealth", &SetWheelHealth);
    V8Helpers::SetMethod(isolate, tpl, "getWheelHealth", &GetWheelHealth);
    V8Helpers::SetMethod(isolate, tpl, "getHealthDataBase64", &GetHealthData);
    V8Helpers::SetMethod(isolate, tpl, "setHealthDataBase64", &SetHealthData);

    // Damage getters/setters
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::HasArmoredWindows>(isolate, tpl, "hasArmoredWindows");

    // Damage methods
    V8Helpers::SetMethod(isolate, tpl, "getPartDamageLevel", &GetPartDamageLevel);
    V8Helpers::SetMethod(isolate, tpl, "setPartDamageLevel", &SetPartDamageLevel);
    V8Helpers::SetMethod(isolate, tpl, "getPartBulletHoles", &GetPartBulletHoles);
    V8Helpers::SetMethod(isolate, tpl, "setPartBulletHoles", &SetPartBulletHoles);
    V8Helpers::SetMethod(isolate, tpl, "isLightDamaged", &IsLightDamaged);
    V8Helpers::SetMethod(isolate, tpl, "setLightDamaged", &SetLightDamaged);
    V8Helpers::SetMethod(isolate, tpl, "isWindowDamaged", &IsWindowDamaged);
    V8Helpers::SetMethod(isolate, tpl, "setWindowDamaged", &SetWindowDamaged);
    V8Helpers::SetMethod(isolate, tpl, "isSpecialLightDamaged", &IsSpecialLightDamaged);
    V8Helpers::SetMethod(isolate, tpl, "setSpecialLightDamaged", &SetSpecialLightDamaged);
    V8Helpers::SetMethod(isolate, tpl, "getBumperDamageLevel", &GetBumperDamageLevel);
    V8Helpers::SetMethod(isolate, tpl, "setBumperDamageLevel", &SetBumperDamageLevel);
    V8Helpers::SetMethod(isolate, tpl, "getArmoredWindowHealth", &GetArmoredWindowHealth);
    V8Helpers::SetMethod(isolate, tpl, "setArmoredWindowHealth", &SetArmoredWindowHealth);
    V8Helpers::SetMethod(isolate, tpl, "getArmoredWindowShootCount", &GetArmoredWindowShootCount);
    V8Helpers::SetMethod(isolate, tpl, "setArmoredWindowShootCount", &SetArmoredWindowShootCount);
    V8Helpers::SetMethod(isolate, tpl, "getDamageStatusBase64", &GetDamageStatus);
    V8Helpers::SetMethod(isolate, tpl, "setDamageStatusBase64", &SetDamageStatus);
    V8Helpers::SetMethod<IVehicle, &IVehicle::SetFixed>(isolate, tpl, "repair");
    V8Helpers::SetMethod(isolate, tpl, "setWheelFixed", &SetWheelFixed);

    // Script getters/setters
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsManualEngineControl, &IVehicle::SetManualEngineControl>(isolate, tpl, "manualEngineControl");

    // Script methods
    V8Helpers::SetMethod(isolate, tpl, "getScriptDataBase64", &GetScriptData);
    V8Helpers::SetMethod(isolate, tpl, "setScriptDataBase64", &SetScriptData);

    V8Helpers::SetAccessor<IVehicle, Ref<IVehicle>, &IVehicle::GetAttached>(isolate, tpl, "attached");
    V8Helpers::SetAccessor<IVehicle, Ref<IVehicle>, &IVehicle::GetAttachedTo>(isolate, tpl, "attachedTo");

    // Train getter/setter
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsTrainMissionTrain, &IVehicle::SetTrainMissionTrain>(isolate, tpl, "isMissionTrain");
    V8Helpers::SetAccessor<IVehicle, int8_t, &IVehicle::GetTrainTrackId, &IVehicle::SetTrainTrackId>(isolate, tpl, "trainTrackId");
    V8Helpers::SetAccessor<IVehicle, Ref<IVehicle>, &IVehicle::GetTrainEngineId>(isolate, tpl, "trainEngineId");
    V8Helpers::SetMethod(isolate, tpl, "setTrainEngineId", &SetTrainEngineId);
    V8Helpers::SetAccessor<IVehicle, int8_t, &IVehicle::GetTrainConfigIndex, &IVehicle::SetTrainConfigIndex>(isolate, tpl, "trainConfigIndex");
    V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetTrainDistanceFromEngine, &IVehicle::SetTrainDistanceFromEngine>(isolate, tpl, "trainDistanceFromEngine");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsTrainEngine, &IVehicle::SetTrainIsEngine>(isolate, tpl, "isTrainEngine");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsTrainCaboose, &IVehicle::SetTrainIsCaboose>(isolate, tpl, "isTrainCaboose");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetTrainDirection, &IVehicle::SetTrainDirection>(isolate, tpl, "trainDirection");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::HasTrainPassengerCarriages, &IVehicle::SetTrainHasPassengerCarriages>(isolate, tpl, "trainPassengerCarriages");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetTrainRenderDerailed, &IVehicle::SetTrainRenderDerailed>(isolate, tpl, "trainRenderDerailed");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetTrainForceDoorsOpen, &IVehicle::SetTrainForceDoorsOpen>(isolate, tpl, "trainForceDoorsOpen");
    V8Helpers::SetAccessor<IVehicle, float, &IVehicle::GetTrainCruiseSpeed, &IVehicle::SetTrainCruiseSpeed>(isolate, tpl, "trainCruiseSpeed");
    V8Helpers::SetAccessor<IVehicle, int8_t, &IVehicle::GetTrainCarriageConfigIndex, &IVehicle::SetTrainCarriageConfigIndex>(isolate, tpl, "trainCarriageConfigIndex");
    V8Helpers::SetAccessor<IVehicle, Ref<IVehicle>, &IVehicle::GetTrainLinkedToBackwardId>(isolate, tpl, "trainLinkedToBackwardId");
    V8Helpers::SetMethod(isolate, tpl, "setTrainLinkedToBackwardId", &SetTrainLinkedToBackwardId);
    V8Helpers::SetAccessor<IVehicle, Ref<IVehicle>, &IVehicle::GetTrainLinkedToForwardId>(isolate, tpl, "trainLinkedToForwardId");
    V8Helpers::SetMethod(isolate, tpl, "setTrainLinkedToForwardId", &SetTrainLinkedToForwardId);
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetTrainUnk1, &IVehicle::SetTrainUnk1>(isolate, tpl, "trainUnk1");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetTrainUnk2, &IVehicle::SetTrainUnk2>(isolate, tpl, "trainUnk2");
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::GetTrainUnk3, &IVehicle::SetTrainUnk3>(isolate, tpl, "trainUnk3");

    // Heli setter
    V8Helpers::SetMethod(isolate, tpl, "setSearchLightTo", &SetSearchLight);

    // Boat setter
    V8Helpers::SetAccessor<IVehicle, bool, &IVehicle::IsBoatAnchorActive, &IVehicle::SetBoatAnchorActive>(isolate, tpl, "boatAnchorActive");
});
