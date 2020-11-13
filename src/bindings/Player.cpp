
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"

#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

using namespace alt;

static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_STRING(player->GetName().CStr());
}

static void VehicleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BASE_OBJECT(player->GetVehicle());
}

static void TalkingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsTalking());
}

static void MicLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_NUMBER(player->GetMicLevel());
}

static void CurrentWeaponComponentsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    alt::Array<uint32_t> comps = player->GetCurrentWeaponComponents();

    v8::Local<v8::Array> componentsArray = v8::Array::New(isolate, comps.GetSize());

    for (uint32_t i = 0; i < comps.GetSize(); ++i)
        componentsArray->Set(ctx, i, v8::Integer::NewFromUnsigned(isolate, comps[i]));

    V8_RETURN(componentsArray);
}

static void CurrentWeaponTintIndexGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetCurrentWeaponTintIndex());
}

static void CurrentWeaponGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetCurrentWeapon());
}

static void IsJumpingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsJumping());
}

static void IsInRagdollGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsInRagdoll());
}

static void IsAimingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsAiming());
}

static void IsShootingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsShooting());
}

static void IsReloadingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsReloading());
}

static void ArmourGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetArmour());
}

static void MaxArmourGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetMaxArmour());
}

static void MoveSpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_NUMBER(player->GetMoveSpeed());
}

static void AimPosGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN(resource->CreateVector3(player->GetAimPos()));
}

static void HeadRotationGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN(resource->CreateVector3(player->GetHeadRotation()));
}

static void SeatGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetSeat());
}

static void EntityAimingAtGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BASE_OBJECT(player->GetEntityAimingAt());
}

static void EntityAimOffsetGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN(resource->CreateVector3(player->GetEntityAimOffset()));
}

static void FlashlightActiveGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsFlashlightActive());
}

static void HealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetHealth());
}

static void MaxHealthGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_INTEGER(player->GetMaxHealth());
}

static void IsDeadGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BOOLEAN(player->IsDead());
}


// static void GiveWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
//     uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

//     V8_CHECK(info[1]->IsNumber(), "ammoCount must be a number");
//     uint32_t ammoCount = info[1]->NumberValue(ctx).ToChecked();

//     player->GiveWeapon(weaponHash, ammoCount, false); // todo: allow setter for selected
// }

// static void RemoveWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
//     uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

//     info.GetReturnValue().Set(v8::Boolean::New(info.GetIsolate(), player->RemoveWeapon(weaponHash)));
// }

// static void RemoveAllWeapons(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     player->RemoveAllWeapons();
// }

// static void AddWeaponComponent(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
//     uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

//     V8_CHECK(info[1]->IsNumber(), "componentHash must be a number");
//     uint32_t componentHash = info[1]->NumberValue(ctx).ToChecked();

//     player->AddWeaponComponent(weaponHash, componentHash);
// }

// static void RemoveWeaponComponent(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
//     uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

//     V8_CHECK(info[1]->IsNumber(), "componentHash must be a number");
//     uint32_t componentHash = info[1]->NumberValue(ctx).ToChecked();

//     player->RemoveWeaponComponent(weaponHash, componentHash);
// }

/*
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
*/

// static void SetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
//     uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

//     V8_CHECK(info[1]->IsNumber(), "tintIndex must be a number");
//     uint8_t tintIndex = info[1]->NumberValue(ctx).ToChecked();

//     player->SetWeaponTintIndex(weaponHash, tintIndex);
// }

/*
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
*/

// static void SetCurrentWeapon(const v8::FunctionCallbackInfo<v8::Value> &info)
// {
//     v8::Isolate *isolate = info.GetIsolate();
//     auto ctx = isolate->GetEnteredContext();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     alt::Ref<alt::IPlayer> player = _this->GetHandle().As<alt::IPlayer>();

//     V8_CHECK(info[0]->IsNumber(), "weaponHash must be a number");
//     uint32_t weaponHash = info[0]->NumberValue(ctx).ToChecked();

//     player->SetCurrentWeapon(weaponHash);
// }

/*
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
*/

static void AllGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Array> arr = v8::Array::New(isolate);

    uint16_t i = 0;
    for (auto player : alt::ICore::Instance().GetPlayers())
    {
        if (player)
            arr->Set(ctx, i++, resource->GetOrCreateEntity(player.Get(), "Player")->GetJSVal(isolate));
    };

    V8_RETURN(arr);
}

static void LocalGetter(v8::Local<v8::Name> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetLocalPlayer());
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid).As<alt::IPlayer>());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, id);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByID(id).As<alt::IPlayer>());
}

extern V8Class v8Entity;
extern V8Class v8Player("Player", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "all").ToLocalChecked(), &AllGetter);
    tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, "local").ToLocalChecked(), &LocalGetter);

    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "name").ToLocalChecked(), &NameGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "vehicle").ToLocalChecked(), &VehicleGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isTalking").ToLocalChecked(), &TalkingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "micLevel").ToLocalChecked(), &MicLevelGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "currentWeaponComponents").ToLocalChecked(), &CurrentWeaponComponentsGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "currentWeaponTintIndex").ToLocalChecked(), &CurrentWeaponTintIndexGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "currentWeapon").ToLocalChecked(), &CurrentWeaponGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isJumping").ToLocalChecked(), &IsJumpingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isInRagdoll").ToLocalChecked(), &IsInRagdollGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isAiming").ToLocalChecked(), &IsAimingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isShooting").ToLocalChecked(), &IsShootingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isReloading").ToLocalChecked(), &IsReloadingGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "armour").ToLocalChecked(), &ArmourGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "maxArmour").ToLocalChecked(), &MaxArmourGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "moveSpeed").ToLocalChecked(), &MoveSpeedGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "aimPos").ToLocalChecked(), &AimPosGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "headRot").ToLocalChecked(), &HeadRotationGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "seat").ToLocalChecked(), &SeatGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "entityAimingAt").ToLocalChecked(), &EntityAimingAtGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "entityAimOffset").ToLocalChecked(), &EntityAimOffsetGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "flashlightActive").ToLocalChecked(), &FlashlightActiveGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "health").ToLocalChecked(), &HealthGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "maxHealth").ToLocalChecked(), &MaxHealthGetter);
    proto->SetAccessor(v8::String::NewFromUtf8(isolate, "isDead").ToLocalChecked(), &IsDeadGetter);

    /*
    if (alt::ICore::Instance().IsSandbox())
    {
        proto->Set(isolate, "weaponHasComponent", v8::FunctionTemplate::New(isolate, &WeaponHasComponent));
        proto->Set(isolate, "getWeaponTintIndex", v8::FunctionTemplate::New(isolate, &GetWeaponTintIndex));
        proto->Set(isolate, "getCurrentWeapon", v8::FunctionTemplate::New(isolate, &GetCurrentWeapon));
    }
    */
});
