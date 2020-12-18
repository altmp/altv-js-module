#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void PingGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetPing()));
}

static void IPGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, player->GetIP().CStr()));
}

static void NameGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, player->GetName().CStr()));
}

static void VehicleGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_BASE_OBJECT(_this->GetVehicle());
}

static void SeatGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetSeat()));
}

static void HealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetHealth()));
}

static void HealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();
	player->SetHealth(value->ToInt32(isolate)->Value());
}

static void MaxHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetMaxHealth()));
}

static void MaxHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();
	player->SetMaxHealth(value->ToInt32(isolate)->Value());
}

static void ArmourGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetArmour()));
}

static void ArmourSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();
	player->SetArmour(value->ToInt32(isolate)->Value());
}

static void MaxArmourGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetMaxArmour()));
}

static void MaxArmourSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();
	player->SetMaxArmour(value->ToInt32(isolate)->Value());
}

static void CurrentWeaponGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetCurrentWeapon()));
}

static void CurrentWeaponSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();
	player->SetCurrentWeapon(value->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void CurrentWeaponTintIndexGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, player->GetCurrentWeaponTintIndex()));
}

static void CurrentWeaponComponentsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	alt::Array<uint32_t> comps = player->GetCurrentWeaponComponents();

	v8::Local<v8::Array> componentsArray = v8::Array::New(isolate, comps.GetSize());
	
	for (uint32_t i = 0; i < comps.GetSize(); ++i)
		componentsArray->Set(i, v8::Integer::NewFromUnsigned(isolate, comps[i]));

	info.GetReturnValue().Set(componentsArray);
}

static void EntityAimingAtGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_BASE_OBJECT(_this->GetEntityAimingAt());
}

static void EntityAimOffsetGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	alt::Position aimOffset = player->GetEntityAimOffset();

	info.GetReturnValue().Set(resource->CreateVector3(aimOffset));
}

static void FlashlightActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::Boolean::New(isolate, player->IsFlashlightActive()));
}

static void SocialIdGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, std::to_string(player->GetSocialID()).c_str()));
}

static void HwidHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, std::to_string(player->GetHwidHash()).c_str()));
}

static void HwidExHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, std::to_string(player->GetHwidExHash()).c_str()));
}

static void AuthTokenGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, player->GetAuthToken().CStr()));
}

static void Spawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 3 || info.Length() == 4, "3 or 4 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Number> x = info[0]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[2]->ToNumber(isolate);
	uint32_t timeDelay = 0;
	if (info.Length() == 4)
	{
		v8::Local<v8::Integer> delay = info[3]->ToInteger(isolate);
		timeDelay = delay->Value();
	}

	player->Spawn({ float(x->Value()), float(y->Value()), float(z->Value()) }, timeDelay);
}

static void SetDateTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 6, "6 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> day = info[0]->ToInteger(isolate);
	v8::Local<v8::Integer> month = info[1]->ToInteger(isolate);
	v8::Local<v8::Integer> year = info[2]->ToInteger(isolate);
	v8::Local<v8::Integer> hour = info[3]->ToInteger(isolate);
	v8::Local<v8::Integer> minute = info[4]->ToInteger(isolate);
	v8::Local<v8::Integer> second = info[5]->ToInteger(isolate);

	player->SetDateTime(day->Value(), month->Value(), year->Value(), hour->Value(), minute->Value(), second->Value());
}

static void SetWeather(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> weather = info[0]->ToInteger(isolate);

	player->SetWeather(weather->Value());
}

static void GiveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 3, "3 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> weaponHash = info[0]->ToInteger(isolate);
	v8::Local<v8::Integer> ammo = info[1]->ToInteger(isolate);
	v8::Local<v8::Boolean> equipNow = info[2]->ToBoolean(isolate);

	player->GiveWeapon(weaponHash->Value(), ammo->Value(), equipNow->Value());
}

static void RemoveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> weaponHash = info[0]->ToInteger(isolate);

	player->RemoveWeapon(weaponHash->Value());
}

static void RemoveAllWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	player->RemoveAllWeapons();
}

static void ClearBloodDamage(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	player->ClearBloodDamage();
}

static void AddWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> weaponHash = info[0]->ToInteger(isolate);
	v8::Local<v8::Integer> component = info[1]->ToInteger(isolate);

	player->AddWeaponComponent(weaponHash->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value(), component->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void RemoveWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> weaponHash = info[0]->ToInteger(isolate);
	v8::Local<v8::Integer> component = info[1]->ToInteger(isolate);

	player->RemoveWeaponComponent(weaponHash->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value(), component->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void SetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	V8Entity* _this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	Ref<IPlayer> player = _this->GetHandle().As<IPlayer>();

	v8::Local<v8::Integer> weaponHash = info[0]->ToInteger(isolate);
	v8::Local<v8::Integer> tintIndex = info[1]->ToInteger(isolate);

	player->SetWeaponTintIndex(weaponHash->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value(), tintIndex->ToUint32(isolate->GetEnteredContext()).ToLocalChecked()->Value());
}

static void Kick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK(info.Length() == 0 || info.Length() == 1, "0 or 1 arguments expected");

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

static void Despawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE(info);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	player->Despawn();
}

static void AllGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	info.GetReturnValue().Set(resource->GetAllPlayers());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	uint16_t id = info[0]->ToInteger(isolate)->Value();

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
	proto->Set(v8::String::NewFromUtf8(isolate, "despawn"), v8::FunctionTemplate::New(isolate, &Despawn));
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
