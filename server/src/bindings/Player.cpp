#include "stdafx.h"

#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void CurrentWeaponComponentsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);

    std::vector<uint32_t> comps = _this->GetCurrentWeaponComponents();
    v8::Local<v8::Array> componentsArray = V8Helpers::JSValue(comps);

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
        // (model: number | string, pos: IVector3) overload
        if(info[0]->IsNumber() || info[0]->IsString())
        {
            if(info[0]->IsNumber())
            {
                V8_ARG_TO_UINT(1, model);
                _this->SetModel(model);
            }
            else
            {
                V8_ARG_TO_STRING(1, model);
                _this->SetModel(alt::ICore::Instance().Hash(model));
            }
            V8_ARG_TO_VECTOR3(2, pos);
            position = { pos[0], pos[1], pos[2] };
        }
        // (pos: IVector3, delay?: number) overload
        else
        {
            V8_ARG_TO_VECTOR3(1, pos);
            position = { pos[0], pos[1], pos[2] };
            if(info.Length() == 2)
            {
                V8_ARG_TO_UINT(2, spawnDelay);
                delay = spawnDelay;
            }
        }
    }
    // (x: number, y: number, z: number, delay?: number) overload
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

    uint32_t weaponHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, modelName);
        weaponHash = alt::ICore::Instance().Hash(modelName);
    }
    else
    {
        V8_ARG_TO_UINT(1, _weaponHash);
        weaponHash = _weaponHash;
    }

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
    V8_RETURN_STRING(std::to_string(_this->GetSocialID()));
}

static void HwidHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
    V8_RETURN_STRING(std::to_string(_this->GetHwidHash()));
}

static void HwidExHashGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
    V8_RETURN_STRING(std::to_string(_this->GetHwidExHash()));
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
        V8_RETURN(player->SetClothes(component, drawable, texture, 2));
    }
    else if(info.Length() == 4)
    {
        V8_ARG_TO_INT(4, palette);
        V8_RETURN(player->SetClothes(component, drawable, texture, palette));
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
        V8_RETURN(player->SetDlcClothes(component, drawable, texture, 2, dlc));
    }
    else if(info.Length() == 5)
    {
        V8_ARG_TO_INT(5, palette);
        V8_RETURN(player->SetDlcClothes(component, drawable, texture, palette, dlc));
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

    V8_RETURN(player->SetProps(component, drawable, texture));
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

    V8_RETURN(player->SetDlcProps(component, drawable, texture, dlc));
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

    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, entity);
        V8_RETURN_BOOLEAN(player->IsEntityInStreamingRange(entity));
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, entity, IEntity, "Entity");
        V8_RETURN_BOOLEAN(player->IsEntityInStreamingRange(entity->GetID()));
    }
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

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetPlayers().size());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IEntity* entity = alt::ICore::Instance().GetEntityByID(id);

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

static void PlayAnimation(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN_MAX(2, 10);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_STRING(1, animDict);
    V8_ARG_TO_STRING(2, animName);
    float blendInSpeed = 8.0f;
    if(info.Length() > 2)
    {
        V8_ARG_TO_NUMBER(3, blendInSpeedVal);
        blendInSpeed = blendInSpeedVal;
    }
    float blendOutSpeed = 8.0f;
    if(info.Length() > 3)
    {
        V8_ARG_TO_NUMBER(4, blendOutSpeedVal);
        blendOutSpeed = blendOutSpeedVal;
    }
    int duration = -1;
    if(info.Length() > 4)
    {
        V8_ARG_TO_INT(5, durationVal);
        duration = durationVal;
    }
    int flag = 0;
    if(info.Length() > 5)
    {
        V8_ARG_TO_INT(6, flagVal);
        flag = flagVal;
    }
    float playbackRate = 1.0f;
    if(info.Length() > 6)
    {
        V8_ARG_TO_NUMBER(7, playbackRateVal);
        playbackRate = playbackRateVal;
    }
    bool lockX = false;
    if(info.Length() > 7)
    {
        V8_ARG_TO_BOOLEAN(8, lockXVal);
        lockX = lockXVal;
    }
    bool lockY = false;
    if(info.Length() > 8)
    {
        V8_ARG_TO_BOOLEAN(9, lockYVal);
        lockY = lockYVal;
    }
    bool lockZ = false;
    if(info.Length() > 9)
    {
        V8_ARG_TO_BOOLEAN(10, lockZVal);
        lockZ = lockZVal;
    }

    player->PlayAnimation(animDict, animName, blendInSpeed, blendOutSpeed, duration, flag, playbackRate, lockX, lockY, lockZ);
}

static void SetHeadOverlay(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(3);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, overlayID);
    V8_ARG_TO_UINT(2, index);
    V8_ARG_TO_NUMBER(3, opacity);

    V8_RETURN(player->SetHeadOverlay(overlayID, index, opacity));
}

static void RemoveHeadOverlay(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    V8_ARG_TO_UINT(1, overlayID);

    V8_RETURN(player->RemoveHeadOverlay(overlayID));
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

    V8_RETURN(player->SetHeadOverlayColor(overlayID, colorType, colorIndex, secondColorIndex));
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

    V8_RETURN(player->SetFaceFeature(index, scale));
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

    V8_RETURN(player->RemoveFaceFeature(index));
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

    V8_RETURN(player->SetHeadBlendPaletteColor(id, red, green, blue));
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

    V8_RETURN(player->SetEyeColor(eyeColor));
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

static void GetWeapons(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    std::vector<alt::Weapon> weapons = player->GetWeapons();
    size_t size = weapons.size();
    v8::Local<v8::Array> weaponsArr = v8::Array::New(isolate, (int)size);
    for(alt::Size i = 0; i < size; i++)
    {
        alt::Weapon& weapon = weapons[i];
        V8_NEW_OBJECT(weaponObj);
        V8_OBJECT_SET_UINT(weaponObj, "hash", weapon.hash);
        V8_OBJECT_SET_UINT(weaponObj, "tintIndex", weapon.tintIndex);

        auto& components = weapon.components;
        v8::Local<v8::Array> componentsArr = v8::Array::New(isolate);
        size_t idx = 0;
        for(auto it : components)
        {
            componentsArr->Set(ctx, idx++, V8Helpers::JSValue(it));
        }
        weaponObj->Set(ctx, V8Helpers::JSValue("components"), componentsArr);

        weaponsArr->Set(ctx, i, weaponObj);
    }

    V8_RETURN(weaponsArr);
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_ARG_TO_STRING(1, eventName);

    MValueArgs mvArgs;
    for(int i = 1; i < info.Length(); ++i) mvArgs.emplace_back(V8Helpers::V8ToMValue(info[i], false));

    alt::ICore::Instance().TriggerClientEvent(player, eventName, mvArgs);
}

static void EmitRaw(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_ARG_TO_STRING(1, eventName);

    MValueArgs mvArgs;
    for(int i = 1; i < info.Length(); ++i)
    {
        alt::MValueByteArray result = V8Helpers::V8ToRawBytes(info[i]);
        V8_CHECK(!result.IsEmpty(), "Failed to serialize value");
        mvArgs.emplace_back(result);
    }

    alt::ICore::Instance().TriggerClientEvent(player, eventName, mvArgs);
}

static void HasLocalMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_ARG_TO_STRING(1, key);

    V8_RETURN(player->HasLocalMetaData(key));
}

static void SetLocalMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, value);
    player->SetLocalMetaData(key, value);
}

static void GetLocalMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_ARG_TO_STRING(1, key);

    V8_RETURN_MVALUE(player->GetLocalMetaData(key));
}

static void DeleteLocalMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_ARG_TO_STRING(1, key);

    player->DeleteLocalMetaData(key);
}

static void GetLocalMetaDataKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    const std::vector<std::string> list = player->GetLocalMetaDataKeys();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

static void DiscordIDGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(_this, IPlayer);
    V8_RETURN_STRING(std::to_string(_this->GetDiscordId()));
}

extern V8Class v8Entity;
extern V8Class v8Player("Player",
                        v8Entity,
                        nullptr,
                        [](v8::Local<v8::FunctionTemplate> tpl)
                        {
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

                            V8Helpers::SetStaticMethod(isolate, tpl, "getByID", &StaticGetByID);
                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                            V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

                            V8Helpers::SetMethod(isolate, tpl, "emit", &Emit);
                            V8Helpers::SetMethod(isolate, tpl, "emitRaw", &EmitRaw);

                            V8Helpers::SetMethod(isolate, tpl, "hasLocalMeta", &HasLocalMeta);
                            V8Helpers::SetMethod(isolate, tpl, "setLocalMeta", &SetLocalMeta);
                            V8Helpers::SetMethod(isolate, tpl, "getLocalMeta", &GetLocalMeta);
                            V8Helpers::SetMethod(isolate, tpl, "deleteLocalMeta", &DeleteLocalMeta);
                            V8Helpers::SetMethod(isolate, tpl, "getLocalMetaKeys", &GetLocalMetaDataKeys);

                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetPing>(isolate, tpl, "ping");
                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetIP>(isolate, tpl, "ip");
                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetName>(isolate, tpl, "name");
                            V8Helpers::SetAccessor<IPlayer, IVehicle*, &IPlayer::GetVehicle>(isolate, tpl, "vehicle");
                            V8Helpers::SetAccessor<IPlayer, uint8_t, &IPlayer::GetSeat>(isolate, tpl, "seat");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetHealth, &IPlayer::SetHealth>(isolate, tpl, "health");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetMaxHealth, &IPlayer::SetMaxHealth>(isolate, tpl, "maxHealth");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetArmour, &IPlayer::SetArmour>(isolate, tpl, "armour");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetMaxArmour, &IPlayer::SetMaxArmour>(isolate, tpl, "maxArmour");
                            V8Helpers::SetAccessor(isolate, tpl, "weapons", &GetWeapons);
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsDead>(isolate, tpl, "isDead");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsInRagdoll>(isolate, tpl, "isInRagdoll");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsAiming>(isolate, tpl, "isAiming");
                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsJumping>(isolate, tpl, "isJumping");
                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsShooting>(isolate, tpl, "isShooting");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsReloading>(isolate, tpl, "isReloading");
                            V8Helpers::SetAccessor<IPlayer, Position, &IPlayer::GetAimPos>(isolate, tpl, "aimPos");
                            V8Helpers::SetAccessor<IPlayer, Rotation, &IPlayer::GetHeadRotation>(isolate, tpl, "headRot");

                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsSuperJumpEnabled>(isolate, tpl, "isSuperJumpEnabled");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsCrouching>(isolate, tpl, "isCrouching");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsStealthy>(isolate, tpl, "isStealthy");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsSpawned>(isolate, tpl, "isSpawned");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetForwardSpeed>(isolate, tpl, "forwardSpeed");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetStrafeSpeed>(isolate, tpl, "strafeSpeed");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsEnteringVehicle>(isolate, tpl, "isEnteringVehicle");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsLeavingVehicle>(isolate, tpl, "isLeavingVehicle");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsOnLadder>(isolate, tpl, "isOnLadder");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsInMelee>(isolate, tpl, "isInMelee");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsInCover>(isolate, tpl, "isInCover");

                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetCurrentAnimationDict>(isolate, tpl, "currentAnimationDict");
                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetCurrentAnimationName>(isolate, tpl, "currentAnimationName");

                            V8Helpers::SetAccessor<IPlayer, IEntity*, &IPlayer::GetEntityAimingAt>(isolate, tpl, "entityAimingAt");
                            V8Helpers::SetAccessor<IPlayer, Position, &IPlayer::GetEntityAimOffset>(isolate, tpl, "entityAimOffset");

                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetCurrentWeapon, &IPlayer::SetCurrentWeapon>(isolate, tpl, "currentWeapon");
                            V8Helpers::SetAccessor(isolate, tpl, "currentWeaponComponents", &CurrentWeaponComponentsGetter);
                            V8Helpers::SetAccessor<IPlayer, uint8_t, &IPlayer::GetCurrentWeaponTintIndex>(isolate, tpl, "currentWeaponTintIndex");

                            V8Helpers::SetAccessor(isolate, tpl, "socialID", &SocialIDGetter);
                            V8Helpers::SetAccessor(isolate, tpl, "hwidHash", &HwidHashGetter);
                            V8Helpers::SetAccessor(isolate, tpl, "hwidExHash", &HwidExHashGetter);
                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetSocialClubName>(isolate, tpl, "socialClubName");

                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetAuthToken>(isolate, tpl, "authToken");
                            V8Helpers::SetAccessor(isolate, tpl, "discordID", &DiscordIDGetter);
                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetCloudAuthHash>(isolate, tpl, "cloudAuthHash");

                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsFlashlightActive>(isolate, tpl, "flashlightActive");

                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetMoveSpeed>(isolate, tpl, "moveSpeed");

                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::GetInvincible, &IPlayer::SetInvincible>(isolate, tpl, "invincible");

                            V8Helpers::SetMethod(isolate, tpl, "spawn", &Spawn);
                            V8Helpers::SetMethod<IPlayer, &IPlayer::Despawn>(isolate, tpl, "despawn");
                            V8Helpers::SetMethod(isolate, tpl, "setDateTime", &SetDateTime);
                            V8Helpers::SetMethod(isolate, tpl, "setWeather", &SetWeather);

                            V8Helpers::SetMethod<IPlayer, &IPlayer::ClearBloodDamage>(isolate, tpl, "clearBloodDamage");
                            V8Helpers::SetMethod(isolate, tpl, "giveWeapon", &GiveWeapon);
                            V8Helpers::SetMethod(isolate, tpl, "removeWeapon", &RemoveWeapon);
                            V8Helpers::SetMethod<IPlayer, &IPlayer::RemoveAllWeapons>(isolate, tpl, "removeAllWeapons");

                            V8Helpers::SetMethod(isolate, tpl, "addWeaponComponent", &AddWeaponComponent);
                            V8Helpers::SetMethod(isolate, tpl, "removeWeaponComponent", &RemoveWeaponComponent);

                            V8Helpers::SetMethod(isolate, tpl, "setWeaponTintIndex", &SetWeaponTintIndex);

                            V8Helpers::SetMethod(isolate, tpl, "kick", &Kick);

                            V8Helpers::SetMethod(isolate, tpl, "isEntityInStreamRange", &IsEntityInStreamRange);

                            V8Helpers::SetMethod(isolate, tpl, "setClothes", &SetClothes);
                            V8Helpers::SetMethod(isolate, tpl, "setDlcClothes", &SetDlcClothes);
                            V8Helpers::SetMethod(isolate, tpl, "getClothes", &GetClothes);
                            V8Helpers::SetMethod(isolate, tpl, "getDlcClothes", &GetDlcClothes);

                            V8Helpers::SetMethod(isolate, tpl, "setProp", &SetProps);
                            V8Helpers::SetMethod(isolate, tpl, "setDlcProp", &SetDlcProps);
                            V8Helpers::SetMethod(isolate, tpl, "getProp", &GetProps);
                            V8Helpers::SetMethod(isolate, tpl, "getDlcProp", &GetDlcProps);
                            V8Helpers::SetMethod(isolate, tpl, "clearProp", &ClearProps);

                            V8Helpers::SetMethod(isolate, tpl, "setIntoVehicle", &SetIntoVehicle);

                            V8Helpers::SetMethod(isolate, tpl, "playAmbientSpeech", &PlayAmbientSpeech);
                            V8Helpers::SetMethod(isolate, tpl, "playAnimation", &PlayAnimation);
                            V8Helpers::SetMethod<IPlayer, &IPlayer::ClearTasks>(isolate, tpl, "clearTasks");

                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetInteriorLocation>(isolate, tpl, "currentInterior");
                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetLastDamagedBodyPart, &IPlayer::SetLastDamagedBodyPart>(isolate, tpl, "lastDamagedBodyPart");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::GetSendNames, &IPlayer::SetSendNames>(isolate, tpl, "sendNames");

                            // Appearance getter & setter
                            V8Helpers::SetMethod(isolate, tpl, "setHeadOverlay", &SetHeadOverlay);
                            V8Helpers::SetMethod(isolate, tpl, "removeHeadOverlay", &RemoveHeadOverlay);
                            V8Helpers::SetMethod(isolate, tpl, "setHeadOverlayColor", &SetHeadOverlayColor);
                            V8Helpers::SetMethod(isolate, tpl, "getHeadOverlay", &GetHeadOverlay);
                            V8Helpers::SetMethod(isolate, tpl, "setFaceFeature", &SetFaceFeature);
                            V8Helpers::SetMethod(isolate, tpl, "getFaceFeatureScale", &GetFaceFeatureScale);
                            V8Helpers::SetMethod(isolate, tpl, "removeFaceFeature", &RemoveFaceFeature);
                            V8Helpers::SetMethod(isolate, tpl, "setHeadBlendPaletteColor", &SetHeadBlendPaletteColor);
                            V8Helpers::SetMethod(isolate, tpl, "getHeadBlendPaletteColor", &GetHeadBlendPaletteColor);
                            V8Helpers::SetMethod(isolate, tpl, "setHeadBlendData", &SetHeadBlendData);
                            V8Helpers::SetMethod(isolate, tpl, "getHeadBlendData", &GetHeadBlendData);
                            V8Helpers::SetMethod(isolate, tpl, "setEyeColor", &SetEyeColor);
                            V8Helpers::SetMethod(isolate, tpl, "getEyeColor", &GetEyeColor);
                            V8Helpers::SetMethod(isolate, tpl, "setHairColor", &SetHairColor);
                            V8Helpers::SetMethod(isolate, tpl, "getHairColor", &GetHairColor);
                            V8Helpers::SetMethod(isolate, tpl, "setHairHighlightColor", &SetHairHighlightColor);
                            V8Helpers::SetMethod(isolate, tpl, "getHairHighlightColor", &GetHairHighlightColor);
                        });
