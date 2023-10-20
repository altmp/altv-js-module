#include "V8Helpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

#include "cpp-sdk/objects/ILocalPlayer.h"

extern V8Class v8WeaponData;
static void CurrentWeaponDataGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::ILocalPlayer);

    auto weaponData = player->GetCurrentWeaponData();
    if(!weaponData)
    {
        V8_RETURN_NULL();
        return;
    }
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(weaponData->GetNameHash()) };
    V8_RETURN(v8WeaponData.New(ctx, args));
}

static void GetWeaponAmmo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::ILocalPlayer);

    V8_CHECK_ARGS_LEN(1);

    uint32_t weaponHash = 0;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weapon);
        weaponHash = weapon;
    }
    else if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, weaponStr);
        weaponHash = alt::ICore::Instance().Hash(weaponStr);
    }

    V8_RETURN_UINT(player->GetWeaponAmmo(weaponHash));
}

static void hasWeapon(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::ILocalPlayer);

    V8_CHECK_ARGS_LEN(1);

    uint32_t weaponHash = 0;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weapon);
        weaponHash = weapon;
    }
    else if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, weaponStr);
        weaponHash = alt::ICore::Instance().Hash(weaponStr);
    }

    V8_RETURN_BOOLEAN(player->HasWeapon(weaponHash));
}

static void GetWeapons(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::ILocalPlayer);

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

static void GetWeaponComponents(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::ILocalPlayer);

    V8_CHECK_ARGS_LEN(1);

    uint32_t weaponHash = 0;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weapon);
        weaponHash = weapon;
    }
    else if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, weaponStr);
        weaponHash = alt::ICore::Instance().Hash(weaponStr);
    }

    const std::vector<uint32_t> list = player->GetWeaponComponents(weaponHash);
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

extern V8Class v8Player;
extern V8Class v8LocalPlayer("LocalPlayer",
                             v8Player,
                             [](v8::Local<v8::FunctionTemplate> tpl)
                             {
                                 v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                 V8Helpers::SetAccessor<alt::ILocalPlayer, uint16_t, &alt::ILocalPlayer::GetCurrentAmmo>(isolate, tpl, "currentAmmo");
                                 V8Helpers::SetMethod(isolate, tpl, "getWeaponAmmo", &GetWeaponAmmo);
                                 V8Helpers::SetMethod(isolate, tpl, "getWeaponComponents", &GetWeaponComponents);
                                 V8Helpers::SetMethod(isolate, tpl, "hasWeapon", &hasWeapon);
                                 V8Helpers::SetAccessor(isolate, tpl, "weapons", &GetWeapons);

                                 V8Helpers::SetAccessor(isolate, tpl, "currentWeaponData", &CurrentWeaponDataGetter);
                                 V8Helpers::SetAccessor<alt::ILocalPlayer, float, &alt::ILocalPlayer::GetStamina, &alt::ILocalPlayer::SetStamina>(isolate, tpl, "stamina");
                                 V8Helpers::SetAccessor<alt::ILocalPlayer, float, &alt::ILocalPlayer::GetMaxStamina, &alt::ILocalPlayer::SetMaxStamina>(isolate, tpl, "maxStamina");
                                 V8Helpers::SetAccessor<alt::IWorldObject, int32_t, &alt::IWorldObject::GetDimension>(isolate, tpl, "dimension");
                             });
