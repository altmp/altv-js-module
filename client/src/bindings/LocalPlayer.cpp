#include "V8Helpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "helpers/BindHelpers.h"

#include "cpp-sdk/objects/ILocalPlayer.h"

extern V8Class v8Player;
extern V8Class v8LocalPlayer("LocalPlayer", v8Player, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetAccessor<alt::ILocalPlayer, uint16_t, &alt::ILocalPlayer::GetCurrentAmmo>(isolate, tpl, "currentAmmo");

    V8Helpers::SetAccessor<alt::ILocalPlayer, float, &alt::ILocalPlayer::GetCurrentWeaponRecoil>(isolate, tpl, "currentWeaponRecoil");
    V8Helpers::SetAccessor<alt::ILocalPlayer, float, &alt::ILocalPlayer::GetCurrentWeaponSpread>(isolate, tpl, "currentWeaponSpread");
    V8Helpers::SetAccessor<alt::ILocalPlayer, float, &alt::ILocalPlayer::GetCurrentWeaponDamage>(isolate, tpl, "currentWeaponDamage");
    V8Helpers::SetAccessor<alt::ILocalPlayer, float, &alt::ILocalPlayer::GetCurrentWeaponRange>(isolate, tpl, "currentWeaponRange");
});
