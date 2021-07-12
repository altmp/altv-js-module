#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"

#include "cpp-sdk/objects/ILocalPlayer.h"

static void CurrentAmmoGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::ILocalPlayer);

    V8_RETURN_UINT(player->GetCurrentAmmo());
}

extern V8Class v8Player;
extern V8Class v8LocalPlayer("LocalPlayer", v8Player, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8::SetAccessor(isolate, tpl, "currentAmmo", &CurrentAmmoGetter);
});
