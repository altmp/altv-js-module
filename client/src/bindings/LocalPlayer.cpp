#include "V8Helpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "V8BindHelpers.h"

#include "cpp-sdk/objects/ILocalPlayer.h"

extern V8Class v8Player;
extern V8Class v8LocalPlayer("LocalPlayer", v8Player, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8::SetAccessor<alt::ILocalPlayer, uint16_t, &alt::ILocalPlayer::GetCurrentAmmo>(isolate, tpl, "currentAmmo");
});
