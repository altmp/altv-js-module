
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"

#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE(info);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, player->GetName().CStr()).ToLocalChecked());
}

static void VehicleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BASE_OBJECT(player->GetVehicle());
}

static void TalkingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE(info);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsTalking());
}

static void MicLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE(info);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetMicLevel());
}

static void WeaponHasComponent(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);
    
    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INTEGER(1, weaponHash);
    V8_ARG_TO_INTEGER(2, componentHash);

    V8_RETURN_BOOLEAN(player->HasWeaponComponent(weaponHash, componentHash));
}

static void GetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, weaponHash);

    V8_RETURN_INTEGER(player->GetWeaponTintIndex(weaponHash));
}

static void GetCurrentWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE(info);
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetCurrentWeapon());
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate());

    uint16_t i = 0;
    for (auto player : alt::ICore::Instance().GetPlayers())
    {
        if (player)
            arr->Set(ctx, i++, resource->GetOrCreateEntity(player.Get(), "Player")->GetJSVal(isolate));
    };

    V8_RETURN(arr);
}

static void LocalGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetLocalPlayer());
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid).As<alt::IPlayer>());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, id);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByID(id).As<alt::IPlayer>());
}

extern V8Class v8Entity;
extern V8Class v8Player("Player", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

    V8::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
    V8::SetStaticAccessor(isolate, tpl, "local", &LocalGetter);

    V8::SetAccessor(isolate, tpl, "name", &NameGetter);
    V8::SetAccessor(isolate, tpl, "vehicle", &VehicleGetter);
    V8::SetAccessor(isolate, tpl, "isTalking", &TalkingGetter);
    V8::SetAccessor(isolate, tpl, "micLevel", &MicLevelGetter);

    if (alt::ICore::Instance().IsSandbox())
    {
        proto->Set(isolate, "weaponHasComponent", v8::FunctionTemplate::New(isolate, &WeaponHasComponent));
        proto->Set(isolate, "getWeaponTintIndex", v8::FunctionTemplate::New(isolate, &GetWeaponTintIndex));
        proto->Set(isolate, "getCurrentWeapon", v8::FunctionTemplate::New(isolate, &GetCurrentWeapon));
    }
});
