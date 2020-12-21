#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"
#include <algorithm>

using namespace alt;

static void PingGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetPing());
}

static void IPGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_STRING(player->GetIP().CStr());
}

static void NameGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_STRING(player->GetName().CStr());
}

static void VehicleGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_RETURN_BASE_OBJECT(_this->GetVehicle());
}

static void SeatGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetSeat());
}

static void HealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetHealth());
}

static void HealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	player->SetHealth(value->ToUint32(ctx).ToLocalChecked()->Value());
}

static void MaxHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetMaxHealth());
}

static void MaxHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	player->SetMaxHealth(value->ToUint32(ctx).ToLocalChecked()->Value());
}

static void ArmourGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetArmour());
}

static void ArmourSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	player->SetArmour(value->ToUint32(ctx).ToLocalChecked()->Value());
}

static void MaxArmourGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetMaxArmour());
}

static void MaxArmourSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	player->SetMaxArmour(value->ToUint32(ctx).ToLocalChecked()->Value());
}

static void CurrentWeaponGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetCurrentWeapon());
}

static void CurrentWeaponSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	player->SetCurrentWeapon(value->ToUint32(ctx).ToLocalChecked()->Value());
}

static void CurrentWeaponTintIndexGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_UINT32(player->GetCurrentWeaponTintIndex());
}

static void CurrentWeaponComponentsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	alt::Array<uint32_t> comps = player->GetCurrentWeaponComponents();
	v8::Local<v8::Array> componentsArray = v8::Array::New(isolate, comps.GetSize());
	
	for (uint32_t i = 0; i < comps.GetSize(); ++i)
		componentsArray->Set(i, v8::Integer::NewFromUnsigned(isolate, comps[i]));

	V8_RETURN(componentsArray);
}

static void EntityAimingAtGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_BASE_OBJECT(_this->GetEntityAimingAt());
}

static void EntityAimOffsetGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	alt::Position aimOffset = player->GetEntityAimOffset();

	V8_RETURN(resource->CreateVector3(aimOffset));
}

static void FlashlightActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_BOOLEAN(player->IsFlashlightActive());
}

static void SocialIdGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_STRING(std::to_string(player->GetSocialID()).c_str());
}

static void HwidHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_STRING(std::to_string(player->GetHwidHash()).c_str());
}

static void HwidExHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_RETURN_STRING(std::to_string(player->GetHwidExHash()).c_str());
}

static void AuthTokenGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	V8_RETURN_STRING(player->GetAuthToken().CStr());
}

static void Spawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN2(3, 4);
	
	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);

	uint32_t timeDelay = 0;
	if (info.Length() == 4)
	{
		V8_ARG_TO_INTEGER(4, delay);
		timeDelay = delay;
	}

	player->Spawn({ float(x), float(y), float(z) }, timeDelay);
}

static void SetDateTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(6);

	int day = std::clamp(static_cast<int>(info[0]->ToInteger(isolate)->Value()), 0, 30);
	int month = std::clamp(static_cast<int>(info[1]->ToInteger(isolate)->Value()), 0, 11);
	int year = std::clamp(static_cast<int>(info[2]->ToInteger(isolate)->Value()), 0, 9999);
	int hour = std::clamp(static_cast<int>(info[3]->ToInteger(isolate)->Value()), 0, 23);
	int minute = std::clamp(static_cast<int>(info[4]->ToInteger(isolate)->Value()), 0, 59);
	int second = std::clamp(static_cast<int>(info[5]->ToInteger(isolate)->Value()), 0, 59);

	player->SetDateTime(day, month, year, hour, minute, second);
}

static void SetWeather(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_INTEGER(1, weather);

	player->SetWeather(weather);
}

static void GiveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(3);
	
	V8_ARG_TO_INTEGER(1, weaponHash);
	V8_ARG_TO_INTEGER(2, ammo);
	V8_ARG_TO_BOOLEAN(3, equipNow);

	player->GiveWeapon(weaponHash, ammo, equipNow);
}

static void RemoveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_INTEGER(1, weaponHash);

	player->RemoveWeapon(weaponHash);
}

static void RemoveAllWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	
	player->RemoveAllWeapons();
}

static void ClearBloodDamage(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	player->ClearBloodDamage();
}

static void AddWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_UINT32(1, weaponHash);
	V8_ARG_TO_UINT32(2, component);

	player->AddWeaponComponent(weaponHash, component);
}

static void RemoveWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_UINT32(1, weaponHash);
	V8_ARG_TO_UINT32(2, component);

	player->RemoveWeaponComponent(weaponHash, component);
}

static void SetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);
	V8_CHECK_ARGS_LEN(2);

	V8_ARG_TO_UINT32(1, weaponHash);
	V8_ARG_TO_UINT32(2, tintIndex);

	player->SetWeaponTintIndex(weaponHash, tintIndex);
}

static void Kick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN2(0, 1);
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	
	if (info.Length() == 1)
	{
		V8_ARG_TO_STRING(1, reason);
		_this->Kick(reason);
	}
	else
	{
		_this->Kick();
	}
}

static void AllGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_RETURN(resource->GetAllPlayers());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(1);
	
	V8_ARG_TO_INTEGER(1, id);

	alt::Ref<alt::IEntity> entity = alt::ICore::Instance().GetEntityByID(id);

	if (entity && entity->GetType() == alt::IEntity::Type::PLAYER)
	{
		V8_RETURN_BASE_OBJECT(entity);
	}
	else
	{
		V8_RETURN_NULL();
	}
}

extern V8Class v8Entity;
extern V8Class v8Player("Player", v8Entity, nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

	V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

	tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "all"), &AllGetter);

	V8::SetAccessor(isolate, tpl, "ping", &PingGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "ip"), &IPGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "name"), &NameGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "vehicle"), &VehicleGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "seat"), &SeatGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "health"), &HealthGetter, &HealthSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "maxHealth"), &MaxHealthGetter, &MaxHealthSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "armour"), &ArmourGetter, &ArmourSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "maxArmour"), &MaxArmourGetter, &MaxArmourSetter);

	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "entityAimingAt"), &EntityAimingAtGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "entityAimOffset"), &EntityAimOffsetGetter);

	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "currentWeapon"), &CurrentWeaponGetter, &CurrentWeaponSetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "currentWeaponComponents"), &CurrentWeaponComponentsGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "currentWeaponTintIndex"), &CurrentWeaponTintIndexGetter);

	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "socialId"), &SocialIdGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hwidHash"), &HwidHashGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "hwidExHash"), &HwidExHashGetter);
	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "authToken"), &AuthTokenGetter);

	proto->SetAccessor(v8::String::NewFromUtf8(isolate, "flashlightActive"), &FlashlightActiveGetter);

	proto->Set(v8::String::NewFromUtf8(isolate, "spawn"), v8::FunctionTemplate::New(isolate, &Spawn));
	proto->Set(v8::String::NewFromUtf8(isolate, "setDateTime"), v8::FunctionTemplate::New(isolate, &SetDateTime));
	proto->Set(v8::String::NewFromUtf8(isolate, "setWeather"), v8::FunctionTemplate::New(isolate, &SetWeather));

	proto->Set(v8::String::NewFromUtf8(isolate, "clearBloodDamage"), v8::FunctionTemplate::New(isolate, &ClearBloodDamage));
	proto->Set(v8::String::NewFromUtf8(isolate, "giveWeapon"), v8::FunctionTemplate::New(isolate, &GiveWeapon));
	proto->Set(v8::String::NewFromUtf8(isolate, "removeWeapon"), v8::FunctionTemplate::New(isolate, &RemoveWeapon));
	proto->Set(v8::String::NewFromUtf8(isolate, "removeAllWeapons"), v8::FunctionTemplate::New(isolate, &RemoveAllWeapons));

	proto->Set(v8::String::NewFromUtf8(isolate, "addWeaponComponent"), v8::FunctionTemplate::New(isolate, &AddWeaponComponent));
	proto->Set(v8::String::NewFromUtf8(isolate, "removeWeaponComponent"), v8::FunctionTemplate::New(isolate, &RemoveWeaponComponent));

	proto->Set(v8::String::NewFromUtf8(isolate, "setWeaponTintIndex"), v8::FunctionTemplate::New(isolate, &SetWeaponTintIndex));

	proto->Set(v8::String::NewFromUtf8(isolate, "kick"), v8::FunctionTemplate::New(isolate, &Kick));
});
