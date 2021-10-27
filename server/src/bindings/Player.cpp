#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "V8BindHelpers.h"

using namespace alt;

static void CurrentWeaponComponentsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    alt::Array<uint32_t> comps = _this->GetCurrentWeaponComponents();
    v8::Local<v8::Array> componentsArray = V8::JSValue(comps);

    V8_RETURN(componentsArray);
}

static void Spawn(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN_MAX(1, 4);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    alt::Position position;
    uint32_t delay = 0;
    if(info.Length() == 1 || info.Length() == 2)
    {
        V8_ARG_TO_VECTOR3(1, pos);
        if(info.Length() == 2)
        {
            V8_ARG_TO_UINT(2, spawnDelay);
            delay = spawnDelay;
        }
        position = { pos[0], pos[1], pos[2] };
    }
    else if(info.Length() == 3 || info.Length() == 4)
    {
        V8_ARG_TO_NUMBER(1, x);
        V8_ARG_TO_NUMBER(2, y);
        V8_ARG_TO_NUMBER(3, z);
        if(info.Length() == 4)
        {
            V8_ARG_TO_UINT(4, spawnDelay);
            delay = spawnDelay;
        }
        position = { x, y, z };
    }

    _this->Spawn(position, delay);
}

static void SetDateTime(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(6);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_INT(1, day);
    V8_ARG_TO_INT(2, month);
    V8_ARG_TO_INT(3, year);
    V8_ARG_TO_INT(4, hour);
    V8_ARG_TO_INT(5, minute);
    V8_ARG_TO_INT(6, second);

    _this->SetDateTime(day, month, year, hour, minute, second);
}

static void SetWeather(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_UINT(1, weather);

    _this->SetWeather(weather);
}

static void GiveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(3);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_UINT(1, weaponHash);
    V8_ARG_TO_INT(2, ammo);
    V8_ARG_TO_BOOLEAN(3, equipNow);

    _this->GiveWeapon(weaponHash, ammo, equipNow);
}

static void RemoveWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_UINT(1, weaponHash);

    _this->RemoveWeapon(weaponHash);
}

static void AddWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_UINT(1, weaponHash);
    V8_ARG_TO_UINT(2, component);

    _this->AddWeaponComponent(weaponHash, component);
}

static void RemoveWeaponComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_UINT(1, weaponHash);
    V8_ARG_TO_UINT(2, component);

    _this->RemoveWeaponComponent(weaponHash, component);
}

static void SetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    V8_ARG_TO_UINT(1, weaponHash);
    V8_ARG_TO_INT(2, tintIndex);

    _this->SetWeaponTintIndex(weaponHash, tintIndex);
}

static void Kick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(0, 1);

    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    if(info.Length() == 1)
    {
        V8_ARG_TO_STRING(1, reason);
        _this->Kick(reason);
    }
    else
    {
        _this->Kick();
    }
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
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 4);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);
    V8_ARG_TO_UINT(2, drawable);
    V8_ARG_TO_UINT(3, texture);

    if(info.Length() == 3)
    {
        player->SetClothes(component, drawable, texture, 2);
    }
    else if(info.Length() == 4)
    {
        V8_ARG_TO_INT(4, palette);
        player->SetClothes(component, drawable, texture, palette);
    }
}

static void SetDlcClothes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN_MAX(4, 5);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, dlc);
    V8_ARG_TO_UINT(2, component);
    V8_ARG_TO_UINT(3, drawable);
    V8_ARG_TO_UINT(4, texture);

    V8_CHECK(drawable < 128, "Drawable can't be higher than 127");

    if(info.Length() == 4)
    {
        player->SetDlcClothes(component, drawable, texture, 2, dlc);
    }
    else if(info.Length() == 5)
    {
        V8_ARG_TO_INT(5, palette);
        player->SetDlcClothes(component, drawable, texture, palette, dlc);
    }
}

static void GetClothes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);

    V8_NEW_OBJECT(clothes);

    auto cloth = player->GetClothes(component);
    V8_OBJECT_SET_INT(clothes, "drawable", cloth.drawableId);
    V8_OBJECT_SET_INT(clothes, "texture", cloth.textureId);
    V8_OBJECT_SET_INT(clothes, "palette", cloth.paletteId);

    V8_RETURN(clothes);
}

static void GetDlcClothes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);

    V8_NEW_OBJECT(clothes);

    auto cloth = player->GetDlcClothes(component);
    V8_OBJECT_SET_INT(clothes, "drawable", cloth.drawableId);
    V8_OBJECT_SET_INT(clothes, "texture", cloth.textureId);
    V8_OBJECT_SET_INT(clothes, "palette", cloth.paletteId);
    V8_OBJECT_SET_UINT(clothes, "dlc", cloth.dlc);

    V8_RETURN(clothes);
}

static void SetProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(3);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);
    V8_ARG_TO_UINT(2, drawable);
    V8_ARG_TO_UINT(3, texture);

    player->SetProps(component, drawable, texture);
}

static void SetDlcProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(4);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, dlc);
    V8_ARG_TO_UINT(2, component);
    V8_ARG_TO_UINT(3, drawable);
    V8_ARG_TO_UINT(4, texture);

    player->SetDlcProps(component, drawable, texture, dlc);
}

static void ClearProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);

    player->ClearProps(component);
}

static void GetProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);

    V8_NEW_OBJECT(prop);

    auto props = player->GetProps(component);
    V8_OBJECT_SET_INT(prop, "drawable", props.drawableId);
    V8_OBJECT_SET_INT(prop, "texture", props.textureId);

    V8_RETURN(prop);
}

static void GetDlcProps(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, component);

    V8_NEW_OBJECT(prop);

    auto props = player->GetDlcProps(component);
    V8_OBJECT_SET_INT(prop, "drawable", props.drawableId);
    V8_OBJECT_SET_INT(prop, "texture", props.textureId);
    V8_OBJECT_SET_UINT(prop, "dlc", props.dlc);

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

static void SetIntoVehicle(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_BASE_OBJECT(1, vehicle, alt::IVehicle, "Vehicle");
    V8_ARG_TO_UINT(2, seat);

    player->SetIntoVehicle(vehicle, seat);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllPlayers());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::Ref<alt::IEntity> entity = alt::ICore::Instance().GetEntityByID(id);

    if(entity && entity->GetType() == alt::IEntity::Type::PLAYER)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void PlayAmbientSpeech(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(3);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_STRING(1, speechName);
    V8_ARG_TO_STRING(2, speechParam);
    V8_ARG_TO_UINT(3, speechDictHash);

    player->PlayAmbientSpeech(speechName, speechParam, speechDictHash);
}

static void SetHeadOverlay(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(3);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, overlayID);
    V8_ARG_TO_UINT(2, index);
    V8_ARG_TO_NUMBER(3, opacity);

    player->SetHeadOverlay(overlayID, index, opacity);
}

static void RemoveHeadOverlay(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, overlayID);

    player->RemoveHeadOverlay(overlayID);
}

static void SetHeadOverlayColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(4);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, overlayID);
    V8_ARG_TO_UINT(2, colorType);
    V8_ARG_TO_UINT(3, colorIndex);
    V8_ARG_TO_UINT(4, secondColorIndex);

    player->SetHeadOverlayColor(overlayID, colorType, colorIndex, secondColorIndex);
}

static void GetHeadOverlay(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, overlayID);

    V8_NEW_OBJECT(headOverlayData);

    auto headOverlay = player->GetHeadOverlay(overlayID);
    V8_OBJECT_SET_UINT(headOverlayData, "index", headOverlay.index);
    V8_OBJECT_SET_NUMBER(headOverlayData, "opacity", headOverlay.opacity);
    V8_OBJECT_SET_UINT(headOverlayData, "colorType", headOverlay.colorType);
    V8_OBJECT_SET_UINT(headOverlayData, "colorIndex", headOverlay.colorIndex);
    V8_OBJECT_SET_UINT(headOverlayData, "secondColorIndex", headOverlay.secondColorIndex);

    V8_RETURN(headOverlayData);
}

static void SetFaceFeature(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, index);
    V8_ARG_TO_NUMBER(2, scale);

    player->SetFaceFeature(index, scale);
}

static void GetFaceFeatureScale(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, index);

    V8_RETURN_NUMBER(player->GetFaceFeatureScale(index));
}

static void RemoveFaceFeature(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, index);

    player->RemoveFaceFeature(index);
}

static void SetHeadBlendPaletteColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(4);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, id);
    V8_ARG_TO_UINT(2, red);
    V8_ARG_TO_UINT(3, green);
    V8_ARG_TO_UINT(4, blue);

    player->SetHeadBlendPaletteColor(id, red, green, blue);
}

static void GetHeadBlendPaletteColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, id);

    V8_RETURN_RGBA(player->GetHeadBlendPaletteColor(id));
}

static void SetHeadBlendData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(9);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, shapeFirstID);
    V8_ARG_TO_UINT(2, shapeSecondID);
    V8_ARG_TO_UINT(3, shapeThirdID);
    V8_ARG_TO_UINT(4, skinFirstID);
    V8_ARG_TO_UINT(5, skinSecondID);
    V8_ARG_TO_UINT(6, skinThirdID);
    V8_ARG_TO_NUMBER(7, shapeMix);
    V8_ARG_TO_NUMBER(8, skinMix);
    V8_ARG_TO_NUMBER(9, thirdMix);

    player->SetHeadBlendData(shapeFirstID, shapeSecondID, shapeThirdID, skinFirstID, skinSecondID, skinThirdID, shapeMix, skinMix, thirdMix);
}

static void GetHeadBlendData(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_NEW_OBJECT(headBlendData);

    auto headBlend = player->GetHeadBlendData();
    V8_OBJECT_SET_UINT(headBlendData, "shapeFirstID", headBlend.shapeFirstID);
    V8_OBJECT_SET_UINT(headBlendData, "shapeSecondID", headBlend.shapeSecondID);
    V8_OBJECT_SET_UINT(headBlendData, "shapeThirdID", headBlend.shapeThirdID);
    V8_OBJECT_SET_UINT(headBlendData, "skinFirstID", headBlend.skinFirstID);
    V8_OBJECT_SET_UINT(headBlendData, "skinSecondID", headBlend.skinSecondID);
    V8_OBJECT_SET_UINT(headBlendData, "skinThirdID", headBlend.skinThirdID);
    V8_OBJECT_SET_NUMBER(headBlendData, "shapeMix", headBlend.shapeMix);
    V8_OBJECT_SET_NUMBER(headBlendData, "skinMix", headBlend.skinMix);
    V8_OBJECT_SET_NUMBER(headBlendData, "thirdMix", headBlend.thirdMix);

    V8_RETURN(headBlendData);
}

static void SetEyeColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_INT(1, eyeColor);

    player->SetEyeColor(eyeColor);
}

static void GetEyeColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_RETURN_INT(player->GetEyeColor());
}

static void SetHairColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, hairColor);

    player->SetHairColor(hairColor);
}

static void GetHairColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_RETURN_UINT(player->GetHairColor());
}

static void SetHairHighlightColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, hairHighlightColor);

    player->SetHairHighlightColor(hairHighlightColor);
}

static void GetHairHighlightColor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_RETURN_UINT(player->GetHairHighlightColor());
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
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsDead>(isolate, tpl, "isDead");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsInRagdoll>(isolate, tpl, "isInRagdoll");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsAiming>(isolate, tpl, "isAiming");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsJumping>(isolate, tpl, "isJumping");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsShooting>(isolate, tpl, "isShooting");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsReloading>(isolate, tpl, "isReloading");
    V8::SetAccessor<IPlayer, Position, &IPlayer::GetAimPos>(isolate, tpl, "aimPos");
    V8::SetAccessor<IPlayer, Rotation, &IPlayer::GetHeadRotation>(isolate, tpl, "headRot");

    V8::SetAccessor<IPlayer, bool, &IPlayer::IsSuperJumpEnabled>(isolate, tpl, "isSuperJumpEnabled");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsCrouching>(isolate, tpl, "isCrouching");
    V8::SetAccessor<IPlayer, bool, &IPlayer::IsStealthy>(isolate, tpl, "isStealthy");

    V8::SetAccessor<IPlayer, Ref<IEntity>, &IPlayer::GetEntityAimingAt>(isolate, tpl, "entityAimingAt");
    V8::SetAccessor<IPlayer, Position, &IPlayer::GetEntityAimOffset>(isolate, tpl, "entityAimOffset");

    V8::SetAccessor<IPlayer, uint32_t, &IPlayer::GetCurrentWeapon, &IPlayer::SetCurrentWeapon>(isolate, tpl, "currentWeapon");
    V8::SetAccessor(isolate, tpl, "currentWeaponComponents", &CurrentWeaponComponentsGetter);
    V8::SetAccessor<IPlayer, uint8_t, &IPlayer::GetCurrentWeaponTintIndex>(isolate, tpl, "currentWeaponTintIndex");

    V8::SetAccessor(isolate, tpl, "socialID", &SocialIDGetter);
    V8::SetAccessor(isolate, tpl, "hwidHash", &HwidHashGetter);
    V8::SetAccessor(isolate, tpl, "hwidExHash", &HwidExHashGetter);

    V8::SetAccessor<IPlayer, StringView, &IPlayer::GetAuthToken>(isolate, tpl, "authToken");

    V8::SetAccessor<IPlayer, bool, &IPlayer::IsFlashlightActive>(isolate, tpl, "flashlightActive");

    V8::SetAccessor<IPlayer, float, &IPlayer::GetMoveSpeed>(isolate, tpl, "moveSpeed");

    V8::SetAccessor<IPlayer, bool, &IPlayer::GetInvincible, &IPlayer::SetInvincible>(isolate, tpl, "invincible");

    V8::SetMethod(isolate, tpl, "spawn", &Spawn);
    V8::SetMethod(isolate, tpl, "setDateTime", &SetDateTime);
    V8::SetMethod(isolate, tpl, "setWeather", &SetWeather);

    V8::SetMethod<IPlayer, &IPlayer::ClearBloodDamage>(isolate, tpl, "clearBloodDamage");
    V8::SetMethod(isolate, tpl, "giveWeapon", &GiveWeapon);
    V8::SetMethod(isolate, tpl, "removeWeapon", &RemoveWeapon);
    V8::SetMethod<IPlayer, &IPlayer::RemoveAllWeapons>(isolate, tpl, "removeAllWeapons");

    V8::SetMethod(isolate, tpl, "addWeaponComponent", &AddWeaponComponent);
    V8::SetMethod(isolate, tpl, "removeWeaponComponent", &RemoveWeaponComponent);

    V8::SetMethod(isolate, tpl, "setWeaponTintIndex", &SetWeaponTintIndex);

    V8::SetMethod(isolate, tpl, "kick", &Kick);

    V8::SetMethod(isolate, tpl, "isEntityInStreamRange", &IsEntityInStreamRange);

    V8::SetMethod(isolate, tpl, "setClothes", &SetClothes);
    V8::SetMethod(isolate, tpl, "setDlcClothes", &SetDlcClothes);
    V8::SetMethod(isolate, tpl, "getClothes", &GetClothes);
    V8::SetMethod(isolate, tpl, "getDlcClothes", &GetDlcClothes);

    V8::SetMethod(isolate, tpl, "setProp", &SetProps);
    V8::SetMethod(isolate, tpl, "setDlcProp", &SetDlcProps);
    V8::SetMethod(isolate, tpl, "getProp", &GetProps);
    V8::SetMethod(isolate, tpl, "getDlcProp", &GetDlcProps);
    V8::SetMethod(isolate, tpl, "clearProp", &ClearProps);

    V8::SetMethod(isolate, tpl, "setIntoVehicle", &SetIntoVehicle);

    V8::SetMethod(isolate, tpl, "playAmbientSpeech", &PlayAmbientSpeech);

    //Appearance getter & setter
    V8::SetMethod(isolate, tpl, "setHeadOverlay", &SetHeadOverlay);
    V8::SetMethod(isolate, tpl, "removeHeadOverlay", &RemoveHeadOverlay);
    V8::SetMethod(isolate, tpl, "setHeadOverlayColor", &SetHeadOverlayColor);
    V8::SetMethod(isolate, tpl, "getHeadOverlay", &GetHeadOverlay);
    V8::SetMethod(isolate, tpl, "setFaceFeature", &SetFaceFeature);
    V8::SetMethod(isolate, tpl, "getFaceFeatureScale", &GetFaceFeatureScale);
    V8::SetMethod(isolate, tpl, "removeFaceFeature", &RemoveFaceFeature);
    V8::SetMethod(isolate, tpl, "setHeadBlendPaletteColor", &SetHeadBlendPaletteColor);
    V8::SetMethod(isolate, tpl, "getHeadBlendPaletteColor", &GetHeadBlendPaletteColor);
    V8::SetMethod(isolate, tpl, "setHeadBlendData", &SetHeadBlendData);
    V8::SetMethod(isolate, tpl, "getHeadBlendData", &GetHeadBlendData);
    V8::SetMethod(isolate, tpl, "setEyeColor", &SetEyeColor);
    V8::SetMethod(isolate, tpl, "getEyeColor", &GetEyeColor);
    V8::SetMethod(isolate, tpl, "setHairColor", &SetHairColor);
    V8::SetMethod(isolate, tpl, "getHairColor", &GetHairColor);
    V8::SetMethod(isolate, tpl, "setHairHighlightColor", &SetHairHighlightColor);
    V8::SetMethod(isolate, tpl, "getHairHighlightColor", &GetHairHighlightColor);
});
