#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"
#include "helpers/V8BindHelpers.h"

using namespace alt;

static void CurrentWeaponComponentsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	alt::Array<uint32_t> comps = _this->GetCurrentWeaponComponents();
	v8::Local<v8::Array> componentsArray = v8::Array::New(isolate, comps.GetSize());
	
	for (uint32_t i = 0; i < comps.GetSize(); ++i)
		componentsArray->Set(ctx, i, v8::Integer::NewFromUnsigned(isolate, comps[i]));

	V8_RETURN(componentsArray);
}

static void Spawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN2(3, 4);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_NUMBER(1, x);
	V8_ARG_TO_NUMBER(2, y);
	V8_ARG_TO_NUMBER(3, z);

	if (info.Length() == 4)
	{
		V8_ARG_TO_INTEGER(3, delay);
		_this->Spawn({ float(x), float(y), float(z) }, delay);
	}
	else
	{
		_this->Spawn({ float(x), float(y), float(z) }, 0);
	}
}

static void SetDateTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(6);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, day);
	V8_ARG_TO_INTEGER(2, month);
	V8_ARG_TO_INTEGER(3, year);
	V8_ARG_TO_INTEGER(4, hour);
	V8_ARG_TO_INTEGER(5, minute);
	V8_ARG_TO_INTEGER(6, second);

	_this->SetDateTime(day, month, year, hour, minute, second);
}

static void SetWeather(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, weather);

	_this->SetWeather(weather);
}

static void GiveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(3);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, weaponHash);
	V8_ARG_TO_INTEGER(2, ammo);
	V8_ARG_TO_BOOLEAN(3, equipNow);

	_this->GiveWeapon(weaponHash, ammo, equipNow);
}

static void RemoveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, weaponHash);

	_this->RemoveWeapon(weaponHash);
}

static void RemoveAllWeapons(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	_this->RemoveAllWeapons();
}

static void ClearBloodDamage(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	_this->ClearBloodDamage();
}

static void AddWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(2);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, weaponHash);
	V8_ARG_TO_INTEGER(2, component);

	_this->AddWeaponComponent(weaponHash, component);
}

static void RemoveWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(2);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, weaponHash);
	V8_ARG_TO_INTEGER(2, component);

	_this->RemoveWeaponComponent(weaponHash, component);
}

static void SetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(2);

	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

	V8_ARG_TO_INTEGER(1, weaponHash);
	V8_ARG_TO_INTEGER(2, tintIndex);

	_this->SetWeaponTintIndex(weaponHash, tintIndex);
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

static void SocialIdGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	Log::Warning << "player.socialId is deprecated and will be removed in the future. Consider using player.socialID" << Log::Endl;
	
	V8_GET_ISOLATE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_STRING(std::to_string(_this->GetSocialID()).c_str());
}

static void SocialIDGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_STRING(std::to_string(_this->GetSocialID()).c_str());
}

static void HwidHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_STRING(std::to_string(_this->GetHwidHash()).c_str());
}

static void HwidExHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE();
	V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
	V8_RETURN_STRING(std::to_string(_this->GetHwidExHash()).c_str());
}

static void SetClothes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN_MIN_MAX(3, 5);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);
	V8_ARG_TO_INTEGER(2, drawable);
	V8_ARG_TO_INTEGER(3, texture);

	if(info.Length() == 3)
	{
		player->SetClothes(component, drawable, texture, 2);
	}
	else if(info.Length() == 4)
	{
		V8_ARG_TO_INTEGER(4, palette);
		player->SetClothes(component, drawable, texture, palette);
	}
	else if(info.Length() == 5)
	{
		V8_ARG_TO_INTEGER(4, palette);
		V8_ARG_TO_INTEGER(5, dlc);
		player->SetDlcClothes(component, drawable, texture, palette, dlc);
	}
}

static void GetClothes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN2(1, 2);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);

	bool dlcCloth = false;
	if(info.Length() == 2)
	{
		V8_ARG_TO_BOOLEAN(2, dlc);
		dlcCloth = dlc;
	}

	V8_NEW_OBJECT(clothes);
	if(!dlcCloth)
	{
		auto cloth = player->GetClothes(component);
		V8_OBJECT_SET_INTEGER(clothes, "drawable", cloth.drawableId);
		V8_OBJECT_SET_INTEGER(clothes, "texture", cloth.textureId);
		V8_OBJECT_SET_INTEGER(clothes, "palette", cloth.paletteId);
	}
	else
	{
		auto cloth = player->GetDlcClothes(component);
		V8_OBJECT_SET_INTEGER(clothes, "drawable", cloth.drawableId);
		V8_OBJECT_SET_INTEGER(clothes, "texture", cloth.textureId);
		V8_OBJECT_SET_INTEGER(clothes, "palette", cloth.paletteId);
		V8_OBJECT_SET_INTEGER(clothes, "dlc", cloth.dlc);
	}

	V8_RETURN(clothes);
}

static void SetProp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN_MIN_MAX(3, 4);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);
	V8_ARG_TO_INTEGER(2, drawable);
	V8_ARG_TO_INTEGER(3, texture);

	if(info.Length() == 3)
	{
		player->SetProps(component, drawable, texture);
	}
	else if(info.Length() == 4)
	{
		V8_ARG_TO_INTEGER(4, dlc);
		player->SetDlcProps(component, drawable, texture, dlc);
	}

	Log::Warning << "player.setProp() is deprecated and will be removed in the future. Consider using player.setProps()." << Log::Endl;		
}

static void SetProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN_MIN_MAX(3, 4);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);
	V8_ARG_TO_INTEGER(2, drawable);
	V8_ARG_TO_INTEGER(3, texture);

	if(info.Length() == 3)
	{
		player->SetProps(component, drawable, texture);
	}
	else if(info.Length() == 4)
	{
		V8_ARG_TO_INTEGER(4, dlc);
		player->SetDlcProps(component, drawable, texture, dlc);
	}
}

static void ClearProp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);

	player->ClearProps(component);

	Log::Warning << "player.clearProp() is deprecated and will be removed in the future. Consider using player.clearProps()." << Log::Endl;		
}

static void ClearProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);

	player->ClearProps(component);
}

static void GetProp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN2(1, 2);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);

	bool dlcProp = false;
	if(info.Length() == 2)
	{
		V8_ARG_TO_BOOLEAN(2, dlc);
		dlcProp = dlc;
	}

	V8_NEW_OBJECT(prop);
	if(!dlcProp)
	{
		auto props = player->GetProps(component);
		V8_OBJECT_SET_INTEGER(prop, "drawable", props.drawableId);
		V8_OBJECT_SET_INTEGER(prop, "texture", props.textureId);
	}
	else
	{
		auto props = player->GetDlcProps(component);
		V8_OBJECT_SET_INTEGER(prop, "drawable", props.drawableId);
		V8_OBJECT_SET_INTEGER(prop, "texture", props.textureId);
		V8_OBJECT_SET_INTEGER(prop, "dlc", props.dlc);
	}

	Log::Warning << "player.getProp() is deprecated and will be removed in the future. Consider using player.getProps()." << Log::Endl;		
	V8_RETURN(prop);
}

static void GetProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN2(1, 2);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_INTEGER(1, component);

	bool dlcProp = false;
	if(info.Length() == 2)
	{
		V8_ARG_TO_BOOLEAN(2, dlc);
		dlcProp = dlc;
	}

	V8_NEW_OBJECT(prop);
	if(!dlcProp)
	{
		auto props = player->GetProps(component);
		V8_OBJECT_SET_INTEGER(prop, "drawable", props.drawableId);
		V8_OBJECT_SET_INTEGER(prop, "texture", props.textureId);
	}
	else
	{
		auto props = player->GetDlcProps(component);
		V8_OBJECT_SET_INTEGER(prop, "drawable", props.drawableId);
		V8_OBJECT_SET_INTEGER(prop, "texture", props.textureId);
		V8_OBJECT_SET_INTEGER(prop, "dlc", props.dlc);
	}

	V8_RETURN(prop);
}

static void IsEntityInStreamRange(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_CHECK_ARGS_LEN(1);
	V8_GET_THIS_BASE_OBJECT(player, IPlayer);

	V8_ARG_TO_BASE_OBJECT(1, entity, IEntity, "Entity");
	
	V8_RETURN_BOOLEAN(player->IsEntityInStreamingRange(entity));
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	info.GetReturnValue().Set(resource->GetAllPlayers());
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

	V8::SetStaticMethod(isolate, tpl, "getByID", &StaticGetByID);
	V8::SetStaticAccessor(isolate, tpl, "all", &AllGetter);

	V8::SetAccessor<IPlayer, uint32_t, &IPlayer::GetPing>(isolate, tpl, "ping");
	V8::SetAccessor<IPlayer, StringView, &IPlayer::GetIP>(isolate, tpl, "ip");
	V8::SetAccessor<IPlayer, StringView, &IPlayer::GetName>(isolate, tpl, "name");
	V8::SetAccessor<IPlayer, Ref<IVehicle>, &IPlayer::GetVehicle>(isolate, tpl, "vehicle");
	V8::SetAccessor<IPlayer, uint8_t, &IPlayer::GetSeat>(isolate, tpl, "seat");
	V8::SetAccessor<IPlayer, uint16_t, &IPlayer::GetHealth, &IPlayer::SetHealth>(isolate, tpl, "health");
	V8::SetAccessor<IPlayer, uint16_t, &IPlayer::GetMaxHealth, &IPlayer::SetMaxHealth>(isolate, tpl, "maxHealth");
	V8::SetAccessor<IPlayer, uint16_t, &IPlayer::GetArmour, &IPlayer::SetArmour>(isolate, tpl, "armour");
	V8::SetAccessor<IPlayer, uint16_t, &IPlayer::GetMaxArmour, &IPlayer::SetMaxArmour>(isolate, tpl, "maxArmour");

	V8::SetAccessor<IPlayer, Ref<IEntity>, &IPlayer::GetEntityAimingAt>(isolate, tpl, "entityAimingAt");
	V8::SetAccessor<IPlayer, Position, &IPlayer::GetEntityAimOffset>(isolate, tpl, "entityAimOffset");

	V8::SetAccessor<IPlayer, uint32_t, &IPlayer::GetCurrentWeapon, &IPlayer::SetCurrentWeapon>(isolate, tpl, "currentWeapon");
	V8::SetAccessor(isolate, tpl, "currentWeaponComponents", &CurrentWeaponComponentsGetter);
	V8::SetAccessor<IPlayer, uint8_t, &IPlayer::GetCurrentWeaponTintIndex>(isolate, tpl, "currentWeaponTintIndex");

	V8::SetAccessor(isolate, tpl, "socialId", &SocialIdGetter);
	V8::SetAccessor(isolate, tpl, "socialID", &SocialIDGetter);
	V8::SetAccessor(isolate, tpl, "hwidHash", &HwidHashGetter);
	V8::SetAccessor(isolate, tpl, "hwidExHash", &HwidExHashGetter);

	V8::SetAccessor<IPlayer, StringView, &IPlayer::GetAuthToken>(isolate, tpl, "authToken");

	V8::SetAccessor<IPlayer, bool, &IPlayer::IsFlashlightActive>(isolate, tpl, "flashlightActive");

	V8::SetMethod(isolate, tpl, "spawn", &Spawn);
	V8::SetMethod(isolate, tpl, "setDateTime", &SetDateTime);
	V8::SetMethod(isolate, tpl, "setWeather", &SetWeather);

	V8::SetMethod(isolate, tpl, "clearBloodDamage", &ClearBloodDamage);
	V8::SetMethod(isolate, tpl, "giveWeapon", &GiveWeapon);
	V8::SetMethod(isolate, tpl, "removeWeapon", &RemoveWeapon);
	V8::SetMethod(isolate, tpl, "removeAllWeapons", &RemoveAllWeapons);

	V8::SetMethod(isolate, tpl, "addWeaponComponent", &AddWeaponComponent);
	V8::SetMethod(isolate, tpl, "removeWeaponComponent", &RemoveWeaponComponent);

	V8::SetMethod(isolate, tpl, "setWeaponTintIndex", &SetWeaponTintIndex);

	V8::SetMethod(isolate, tpl, "kick", &Kick);

	V8::SetMethod(isolate, tpl, "isEntityInStreamRange", &IsEntityInStreamRange);
	
	V8::SetMethod(isolate, tpl, "setClothes", &SetClothes);
	V8::SetMethod(isolate, tpl, "getClothes", &GetClothes);

	V8::SetMethod(isolate, tpl, "setProp", &SetProp);
	V8::SetMethod(isolate, tpl, "getProp", &GetProp);
	V8::SetMethod(isolate, tpl, "clearProp", &ClearProp);
	
	V8::SetMethod(isolate, tpl, "setProps", &SetProps);
	V8::SetMethod(isolate, tpl, "getProps", &GetProps);
	V8::SetMethod(isolate, tpl, "clearProps", &ClearProps);
});
