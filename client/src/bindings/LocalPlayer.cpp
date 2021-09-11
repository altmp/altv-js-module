#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8BindHelpers.h"

#include "cpp-sdk/objects/ILocalPlayer.h"

extern V8Class v8Player;
extern V8Class v8LocalPlayer("LocalPlayer", v8Player, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8::SetAccessor<alt::ILocalPlayer, uint16_t, &alt::ILocalPlayer::GetCurrentAmmo>(isolate, tpl, "currentAmmo");
});
