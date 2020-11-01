
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"

#include "cpp-sdk/entities/IPlayer.h"
#include "cpp-sdk/entities/IVehicle.h"

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, player->GetName().CStr()).ToLocalChecked());
}

static void VehicleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();
    alt::Ref<alt::IVehicle> vehicle = player->GetVehicle();

    if (vehicle)
        info.GetReturnValue().Set(resource->GetOrCreateEntity(vehicle.Get(), "Vehicle")->GetJSVal(isolate));
    else
        info.GetReturnValue().Set(v8::Null(info.GetIsolate()));
}

static void TalkingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    info.GetReturnValue().Set(v8::Boolean::New(isolate, player->IsTalking()));
}

static void MicLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    info.GetReturnValue().Set(v8::Number::New(isolate, player->GetMicLevel()));
}

static void GiveWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    V8_CHECK(info[1]->IsNumber(), "ammoCount must be a number");
    uint32_t ammoCount = info[1]->NumberValue(ctx).ToChecked();

    player->GiveWeapon(weaponHash, ammoCount, false); // todo: allow setter for selected
}

static void RemoveWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), player->RemoveWeapon(weaponHash)));
}

static void RemoveAllWeapons(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    player->RemoveAllWeapons();
}

static void AddWeaponComponent(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    V8_CHECK(info[1]->IsNumber(), "componentHash must be a number");
    uint32_t componentHash = info[1]->NumberValue(ctx).ToChecked();

    player->AddWeaponComponent(weaponHash, componentHash);
}

static void RemoveWeaponComponent(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    V8_CHECK(info[1]->IsNumber(), "componentHash must be a number");
    uint32_t componentHash = info[1]->NumberValue(ctx).ToChecked();

    player->RemoveWeaponComponent(weaponHash, componentHash);
}

static void WeaponHasComponent(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    V8_CHECK(info[1]->IsNumber(), "componentHash must be a number");
    uint32_t componentHash = info[1]->NumberValue(ctx).ToChecked();

    info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), player->HasWeaponComponent(weaponHash, componentHash)));
}

static void SetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    V8_CHECK(info[1]->IsNumber(), "tintIndex must be a number");
    uint8_t tintIndex = info[1]->NumberValue(ctx).ToChecked();

    player->SetWeaponTintIndex(weaponHash, tintIndex);
}

static void GetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), player->GetWeaponTintIndex(weaponHash)));
}

static void SetCurrentWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
    uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

    player->SetCurrentWeapon(weaponHash);
}

static void GetCurrentWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    auto ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

    info.GetReturnValue().Set(v8::Number::New(info.GetIsolate(), player->GetCurrentWeapon()));
}

static void AllGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
    V8_CHECK(resource, "invalid resource");

    v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate());

    uint16_t i = 0;
    for (auto player : alt::ICore::Instance().GetPlayers())
    {
        if (player)
            arr->Set(ctx, i++, resource->GetOrCreateEntity(player.Get(), "Player")->GetJSVal(isolate));
    };

    info.GetReturnValue().Set(arr);
}

static void LocalGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "Invalid resource");

    auto localPlayer = alt::ICore::Instance().GetLocalPlayer();
    if (!localPlayer)
        info.GetReturnValue().Set(v8::Null(isolate));
    else
        info.GetReturnValue().Set(resource->GetOrCreateEntity(localPlayer.Get(), "Player")->GetJSVal(isolate));
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

static V8Class v8player("Player", "Entity", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "all").ToLocalChecked(), &AllGetter);
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "local").ToLocalChecked(), &LocalGetter);

    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "name").ToLocalChecked(), &NameGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "vehicle").ToLocalChecked(), &VehicleGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isTalking").ToLocalChecked(), &TalkingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "micLevel").ToLocalChecked(), &MicLevelGetter);

    if (alt::ICore::Instance().IsSandbox())
    {
        proto->Set(isolate, "giveWeapon", v8::FunctionTemplate::New(isolate, &GiveWeapon));
        proto->Set(isolate, "removeWeapon", v8::FunctionTemplate::New(isolate, &RemoveWeapon));
        proto->Set(isolate, "removeAllWeapons", v8::FunctionTemplate::New(isolate, &RemoveAllWeapons));
        proto->Set(isolate, "addWeaponComponent", v8::FunctionTemplate::New(isolate, &AddWeaponComponent));
        proto->Set(isolate, "removeWeaponComponent", v8::FunctionTemplate::New(isolate, &RemoveWeaponComponent));
        proto->Set(isolate, "weaponHasComponent", v8::FunctionTemplate::New(isolate, &WeaponHasComponent));

        proto->Set(isolate, "setWeaponTintIndex", v8::FunctionTemplate::New(isolate, &SetWeaponTintIndex));
        proto->Set(isolate, "getWeaponTintIndex", v8::FunctionTemplate::New(isolate, &GetWeaponTintIndex));

        proto->Set(isolate, "setCurrentWeapon", v8::FunctionTemplate::New(isolate, &SetCurrentWeapon));
        proto->Set(isolate, "getCurrentWeapon", v8::FunctionTemplate::New(isolate, &GetCurrentWeapon));
    }
});
