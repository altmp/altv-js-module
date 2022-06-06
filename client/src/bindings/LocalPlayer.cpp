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

    alt::Ref<alt::IWeaponData> weaponData = player->GetCurrentWeaponData();
    if(!weaponData)
    {
        V8_RETURN_NULL();
        return;
    }
    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(weaponData->GetNameHash()) };
    V8_RETURN(v8WeaponData.New(ctx, args));
}

extern V8Class v8Player;
extern V8Class v8LocalPlayer("LocalPlayer", v8Player, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetAccessor<alt::ILocalPlayer, uint16_t, &alt::ILocalPlayer::GetCurrentAmmo>(isolate, tpl, "currentAmmo");

    V8Helpers::SetAccessor(isolate, tpl, "currentWeaponData", &CurrentWeaponDataGetter);
});
