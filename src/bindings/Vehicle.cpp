#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

#include "vehicle/Appearance.h"
#include "vehicle/GameState.h"
#include "vehicle/Health.h"
#include "vehicle/Damage.h"
#include "vehicle/ScriptGameState.h"

using namespace V8::Vehicle;

static void DriverGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
	V8_RETURN_BASE_OBJECT(_this->GetDriver());
}

static void GetAttached(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
	V8_RETURN_BASE_OBJECT(_this->GetAttached());
}

static void GetAttachedTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
	V8_RETURN_BASE_OBJECT(_this->GetAttachedTo());
}

static void DestroyedGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
	V8_RETURN_BOOLEAN(_this->IsDestroyed());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 7, "7 args expected");
	V8_CHECK(info[0]->IsString() || info[0]->IsNumber(), "string or number expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	uint32_t modelHash;

	if (info[0]->IsNumber())
	{
		modelHash = info[0]->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value();
	}
	else
	{
		v8::String::Utf8Value model(isolate, info[0]->ToString(isolate));
		modelHash = alt::ICore::Instance().Hash(*model);
	}

	v8::Local<v8::Number> x = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[3]->ToNumber(isolate);

	v8::Local<v8::Number> rx = info[4]->ToNumber(isolate);
	v8::Local<v8::Number> ry = info[5]->ToNumber(isolate);
	v8::Local<v8::Number> rz = info[6]->ToNumber(isolate);

	alt::Position pos(x->Value(), y->Value(), z->Value());
	alt::Rotation rot(rx->Value(), ry->Value(), rz->Value());

	Ref<IVehicle> veh = alt::ICore::Instance().CreateVehicle(modelHash, pos, rot);

	if (veh)
		resource->BindEntity(info.This(), veh.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create Vehicle");
}

static void AllGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	info.GetReturnValue().Set(resource->GetAllVehicles());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	uint16_t id = info[0]->ToInteger(isolate)->Value();

	alt::Ref<alt::IEntity> entity = alt::ICore::Instance().GetEntityByID(id);

	if (entity && entity->GetType() == alt::IEntity::Type::VEHICLE)
	{
		V8_RETURN_BASE_OBJECT(entity);
	}
	else
	{
		V8_RETURN_NULL();
	}
}

extern V8Class v8Entity;
extern V8Class v8Vehicle("Vehicle", v8Entity, Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

	V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

	tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "all"), &AllGetter);

	//Common getter/setters
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "driver"), &DriverGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "destroyed"), &DestroyedGetter);

	//Appearance getters/setters
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "modKit"), &ModKitGetter, &ModKitSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "modKitsCount"), &ModKitsCountGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "primaryColor"), &PrimaryColorGetter, &PrimaryColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "secondaryColor"), &SecondaryColorGetter, &SecondaryColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "customPrimaryColor"), &CustomPrimaryColorGetter, &CustomPrimaryColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "customSecondaryColor"), &CustomSecondaryColorGetter, &CustomSecondaryColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "tireSmokeColor"), &TireSmokeColorGetter, &TireSmokeColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "neonColor"), &NeonColorGetter, &NeonColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "pearlColor"), &PearlColorGetter, &PearlColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "wheelColor"), &WheelColorGetter, &WheelColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "interiorColor"), &InteriorColorGetter, &InteriorColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "dashboardColor"), &DashboardColorGetter, &DashboardColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "customTires"), &CustomTiresGetter, &CustomTiresSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "darkness"), &DarknessGetter, &DarknessSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "windowTint"), &WindowTintGetter, &WindowTintSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "neon"), &NeonActiveGetter, &NeonActiveSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "dirtLevel"), &DirtLevelGetter, &DirtLevelSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "numberPlateIndex"), &NumberPlateIndexGetter, &NumberPlateIndexSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "numberPlateText"), &NumberPlateTextGetter, &NumberPlateTextSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "livery"), &LiveryGetter, &LiverySetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "roofLivery"), &RoofLiveryGetter, &RoofLiverySetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "wheelType"), &WheelTypeGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "frontWheels"), &FrontWheelsGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "rearWheels"), &RearWheelsGetter);

	//Appearance methods
	proto->Set(v8::String::NewFromUtf8(isolate, "getModsCount"), v8::FunctionTemplate::New(isolate, &GetModsCount));
	proto->Set(v8::String::NewFromUtf8(isolate, "setMod"), v8::FunctionTemplate::New(isolate, &SetMod));
	proto->Set(v8::String::NewFromUtf8(isolate, "getMod"), v8::FunctionTemplate::New(isolate, &GetMod));
	proto->Set(v8::String::NewFromUtf8(isolate, "setExtra"), v8::FunctionTemplate::New(isolate, &SetExtra));
	proto->Set(v8::String::NewFromUtf8(isolate, "getExtra"), v8::FunctionTemplate::New(isolate, &GetExtra));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWheels"), v8::FunctionTemplate::New(isolate, &SetWheels));
	proto->Set(v8::String::NewFromUtf8(isolate, "setRearWheels"), v8::FunctionTemplate::New(isolate, &SetRearWheels));
	proto->Set(v8::String::NewFromUtf8(isolate, "getAppearanceDataBase64"), v8::FunctionTemplate::New(isolate, &GetAppearanceData));
	proto->Set(v8::String::NewFromUtf8(isolate, "setAppearanceDataBase64"), v8::FunctionTemplate::New(isolate, &SetAppearanceData));

	//Gamestate getters/setters
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "engineOn"), &EngineOnGetter, &EngineOnSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "handbrakeActive"), &HandbrakeActiveGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "headlightColor"), &HeadlightColorGetter, &HeadlightColorSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "sirenActive"), &SirenActiveGetter, &SirenActiveSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lockState"), &LockStateGetter, &LockStateSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "daylightOn"), &DaylightOnGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "nightlightOn"), &NightlightOnGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "roofState"), &RoofStateGetter, &RoofStateSetter);

	proto->Set(v8::String::NewFromUtf8(isolate, "isRoofOpened"), v8::FunctionTemplate::New(isolate, &IsRoofOpenedDeprecated));
	proto->Set(v8::String::NewFromUtf8(isolate, "setRoofOpened"), v8::FunctionTemplate::New(isolate, &SetRoofOpenedDeprecated));

	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "flamethrowerActive"), &FlamethrowerActiveGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "activeRadioStation"), &ActiveRadioStationGetter, &ActiveRadioStationSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lightsMultiplier"), &LightsMultiplierGetter, &LightsMultiplierSetter);
	//Gamestate methods
	proto->Set(v8::String::NewFromUtf8(isolate, "getDoorState"), v8::FunctionTemplate::New(isolate, &GetDoorState));
	proto->Set(v8::String::NewFromUtf8(isolate, "setDoorState"), v8::FunctionTemplate::New(isolate, &SetDoorState));
	proto->Set(v8::String::NewFromUtf8(isolate, "isWindowOpened"), v8::FunctionTemplate::New(isolate, &GetWindowOpened));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWindowOpened"), v8::FunctionTemplate::New(isolate, &SetWindowOpened));
	proto->Set(v8::String::NewFromUtf8(isolate, "getGamestateDataBase64"), v8::FunctionTemplate::New(isolate, &GetGamestateData));
	proto->Set(v8::String::NewFromUtf8(isolate, "setGamestateDataBase64"), v8::FunctionTemplate::New(isolate, &SetGamestateData));

	//Health getters/setters
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "engineHealth"), &EngineHealthGetter, &EngineHealthSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "petrolTankHealth"), &PetrolTankHealthGetter, &PetrolTankHealthSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "bodyHealth"), &BodyHealthGetter, &BodyHealthSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "bodyAdditionalHealth"), &BodyAdditionalHealthGetter, &BodyAdditionalHealthSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "wheelsCount"), &WheelsCountGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "repairsCount"), &RepairsCountGetter);
	/*proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lastAttacker"), &LastAttackerGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "lastAttackedWith"), &LastDamagedWithGetter);*/
	//Health methods
	proto->Set(v8::String::NewFromUtf8(isolate, "setWheelBurst"), v8::FunctionTemplate::New(isolate, &SetWheelBurst));
	proto->Set(v8::String::NewFromUtf8(isolate, "isWheelBurst"), v8::FunctionTemplate::New(isolate, &GetWheelBurst));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWheelHasTire"), v8::FunctionTemplate::New(isolate, &SetWheelHasTire));
	proto->Set(v8::String::NewFromUtf8(isolate, "doesWheelHasTire"), v8::FunctionTemplate::New(isolate, &GetWheelHasTire));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWheelDetached"), v8::FunctionTemplate::New(isolate, &SetWheelDetached));
	proto->Set(v8::String::NewFromUtf8(isolate, "isWheelDetached"), v8::FunctionTemplate::New(isolate, &GetIsWheelDetached));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWheelOnFire"), v8::FunctionTemplate::New(isolate, &SetWheelOnFire));
	proto->Set(v8::String::NewFromUtf8(isolate, "isWheelOnFire"), v8::FunctionTemplate::New(isolate, &GetIsWheelOnFire));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWheelHealth"), v8::FunctionTemplate::New(isolate, &SetWheelHealth));
	proto->Set(v8::String::NewFromUtf8(isolate, "getWheelHealth"), v8::FunctionTemplate::New(isolate, &GetWheelHealth));
	proto->Set(v8::String::NewFromUtf8(isolate, "getHealthDataBase64"), v8::FunctionTemplate::New(isolate, &GetHealthData));
	proto->Set(v8::String::NewFromUtf8(isolate, "setHealthDataBase64"), v8::FunctionTemplate::New(isolate, &SetHealthData));

	//Damage getters/setters
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hasArmoredWindows"), &HasArmoredWindowsGetter);
	//Damage methods
	proto->Set(v8::String::NewFromUtf8(isolate, "getPartDamageLevel"), v8::FunctionTemplate::New(isolate, &GetPartDamageLevel));
	proto->Set(v8::String::NewFromUtf8(isolate, "setPartDamageLevel"), v8::FunctionTemplate::New(isolate, &SetPartDamageLevel));
	proto->Set(v8::String::NewFromUtf8(isolate, "getPartBulletHoles"), v8::FunctionTemplate::New(isolate, &GetPartBulletHoles));
	proto->Set(v8::String::NewFromUtf8(isolate, "setPartBulletHoles"), v8::FunctionTemplate::New(isolate, &SetPartBulletHoles));
	proto->Set(v8::String::NewFromUtf8(isolate, "isLightDamaged"), v8::FunctionTemplate::New(isolate, &IsLightDamaged));
	proto->Set(v8::String::NewFromUtf8(isolate, "setLightDamaged"), v8::FunctionTemplate::New(isolate, &SetLightDamaged));
	proto->Set(v8::String::NewFromUtf8(isolate, "isWindowDamaged"), v8::FunctionTemplate::New(isolate, &IsWindowDamaged));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWindowDamaged"), v8::FunctionTemplate::New(isolate, &SetWindowDamaged));
	proto->Set(v8::String::NewFromUtf8(isolate, "isSpecialLightDamaged"), v8::FunctionTemplate::New(isolate, &IsSpecialLightDamaged));
	proto->Set(v8::String::NewFromUtf8(isolate, "setSpecialLightDamaged"), v8::FunctionTemplate::New(isolate, &SetSpecialLightDamaged));
	proto->Set(v8::String::NewFromUtf8(isolate, "getBumperDamageLevel"), v8::FunctionTemplate::New(isolate, &GetBumperDamageLevel));
	proto->Set(v8::String::NewFromUtf8(isolate, "setBumperDamageLevel"), v8::FunctionTemplate::New(isolate, &SetBumperDamageLevel));
	proto->Set(v8::String::NewFromUtf8(isolate, "getArmoredWindowHealth"), v8::FunctionTemplate::New(isolate, &GetArmoredWindowHealth));
	proto->Set(v8::String::NewFromUtf8(isolate, "setArmoredWindowHealth"), v8::FunctionTemplate::New(isolate, &SetArmoredWindowHealth));
	proto->Set(v8::String::NewFromUtf8(isolate, "getArmoredWindowShootCount"), v8::FunctionTemplate::New(isolate, &GetArmoredWindowShootCount));
	proto->Set(v8::String::NewFromUtf8(isolate, "setArmoredWindowShootCount"), v8::FunctionTemplate::New(isolate, &SetArmoredWindowShootCount));
	proto->Set(v8::String::NewFromUtf8(isolate, "getDamageStatusBase64"), v8::FunctionTemplate::New(isolate, &GetDamageStatus));
	proto->Set(v8::String::NewFromUtf8(isolate, "setDamageStatusBase64"), v8::FunctionTemplate::New(isolate, &SetDamageStatus));
	proto->Set(v8::String::NewFromUtf8(isolate, "repair"), v8::FunctionTemplate::New(isolate, &SetFixed));

	//Script getters/setters
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "manualEngineControl"), &ManualEngineControlGetter, &ManualEngineControlSetter);

	//Script methods
	proto->Set(v8::String::NewFromUtf8(isolate, "getScriptDataBase64"), v8::FunctionTemplate::New(isolate, &GetScriptData));
	proto->Set(v8::String::NewFromUtf8(isolate, "setScriptDataBase64"), v8::FunctionTemplate::New(isolate, &SetScriptData));


	proto->Set(v8::String::NewFromUtf8(isolate, "getAttached"), v8::FunctionTemplate::New(isolate, &GetAttached));
	proto->Set(v8::String::NewFromUtf8(isolate, "getAttachedTo"), v8::FunctionTemplate::New(isolate, &GetAttachedTo));
});
