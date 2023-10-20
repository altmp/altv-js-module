
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

using namespace alt;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    std::ostringstream ss;
    ss << "Player{ id: " << std::to_string(player->GetID()) << ", name: " << player->GetName() << " }";

    V8_RETURN_STRING(ss.str());
}

static void CurrentWeaponComponentsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    std::vector<uint32_t> comps = player->GetCurrentWeaponComponents();
    v8::Local<v8::Array> componentsArray = V8Helpers::JSValue(comps);

    V8_RETURN(componentsArray);
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllPlayers()->Clone());
}

static void CountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::PLAYER).size());
}

static void StreamedInGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    auto streamedIn = CV8ScriptRuntime::Instance().GetStreamedInPlayers();
    auto arr = v8::Array::New(isolate, streamedIn.size());
    int i = 0;
    for(auto kv : streamedIn)
    {
        arr->Set(ctx, i, resource->GetOrCreateEntity(kv.second, "Player")->GetJSVal(isolate));
        i++;
    }

    V8_RETURN(arr);
}

static void LocalGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetLocalPlayer());
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, scriptGuid);
    alt::IWorldObject* entity = alt::ICore::Instance().GetWorldObjectByScriptID(scriptGuid);

    if(entity && (entity->GetType() == alt::IEntity::Type::PLAYER || entity->GetType() == alt::IEntity::Type::LOCAL_PLAYER))
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::PLAYER, id);

    if(entity)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void StaticGetByRemoteId(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT32(1, id);

    alt::IBaseObject* entity = alt::ICore::Instance().GetBaseObjectByRemoteID(alt::IBaseObject::Type::PLAYER, id);

    if(entity)
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void GetFilter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_RETURN_BASE_OBJECT(player->GetFilter());
}

static void SetFilter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    if(val->IsNull())
    {
        player->RemoveFilter();
    }
    else if(val->IsObject())
    {
        V8_TO_BASEOBJECT(val, filter);
        player->AddFilter(filter->SharedAs<alt::IAudioFilter>().get());
    }
    else
    {
        V8Helpers::Throw(isolate, "AudioOutput.filter setter expects null or AudioFilter object");
    }
}

static void WeaponHasComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    uint32_t hash;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weaponHash);
        hash = weaponHash;
    }
    else
    {
        V8_ARG_TO_STRING(1, weaponHash);
        hash = alt::ICore::Instance().Hash(weaponHash);
    }

    uint32_t componentHash;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(2, componenthash);
        componentHash = componenthash;
    }
    else
    {
        V8_ARG_TO_STRING(2, componenthash);
        componentHash = alt::ICore::Instance().Hash(componenthash);
    }

    V8_RETURN_BOOLEAN(player->HasWeaponComponent(hash, componentHash));
}

static void GetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_BASE_OBJECT(player, IPlayer);

    uint32_t hash;
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weaponHash);
        hash = weaponHash;
    }
    else
    {
        V8_ARG_TO_STRING(1, weaponHash);
        hash = alt::ICore::Instance().Hash(weaponHash);
    }

    V8_RETURN_UINT(player->GetWeaponTintIndex(hash));
}

extern V8Class v8Entity;
extern V8Class v8Player("Player",
                        v8Entity,
                        [](v8::Local<v8::FunctionTemplate> tpl)
                        {
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

                            V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                            V8Helpers::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);
                            V8Helpers::SetStaticMethod(isolate, tpl, "getByRemoteID", StaticGetByRemoteId);

                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
                            V8Helpers::SetStaticAccessor(isolate, tpl, "count", &CountGetter);

                            V8Helpers::SetStaticAccessor(isolate, tpl, "streamedIn", &StreamedInGetter);
                            V8Helpers::SetStaticAccessor(isolate, tpl, "local", &LocalGetter);

                            // Common getters
                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetName>(isolate, tpl, "name");
                            V8Helpers::SetAccessor<IPlayer, IVehicle*, &IPlayer::GetVehicle>(isolate, tpl, "vehicle");
                            V8Helpers::SetAccessor<IPlayer, uint8_t, &IPlayer::GetSeat>(isolate, tpl, "seat");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsTalking>(isolate, tpl, "isTalking");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetMicLevel>(isolate, tpl, "micLevel");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetHealth>(isolate, tpl, "health");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetMaxHealth>(isolate, tpl, "maxHealth");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetArmour>(isolate, tpl, "armour");
                            V8Helpers::SetAccessor<IPlayer, uint16_t, &IPlayer::GetMaxArmour>(isolate, tpl, "maxArmour");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetSpatialVolume, &IPlayer::SetSpatialVolume>(isolate, tpl, "spatialVolume");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetNonSpatialVolume, &IPlayer::SetNonSpatialVolume>(isolate, tpl, "nonSpatialVolume");
                            V8Helpers::SetAccessor<IPlayer, std::string, &IPlayer::GetTaskData>(isolate, tpl, "taskData");

                            // Weapon getters
                            V8Helpers::SetAccessor(isolate, tpl, "currentWeaponComponents", &CurrentWeaponComponentsGetter);
                            V8Helpers::SetMethod(isolate, tpl, "hasWeaponComponent", &WeaponHasComponent);
                            V8Helpers::SetMethod(isolate, tpl, "getWeaponTintIndex", &GetWeaponTintIndex);
                            V8Helpers::SetAccessor<IPlayer, uint8_t, &IPlayer::GetCurrentWeaponTintIndex>(isolate, tpl, "currentWeaponTintIndex");
                            V8Helpers::SetAccessor<IPlayer, uint32_t, &IPlayer::GetCurrentWeapon>(isolate, tpl, "currentWeapon");
                            V8Helpers::SetAccessor<IPlayer, IEntity*, &IPlayer::GetEntityAimingAt>(isolate, tpl, "entityAimingAt");
                            V8Helpers::SetAccessor<IPlayer, Position, &IPlayer::GetEntityAimOffset>(isolate, tpl, "entityAimOffset");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsFlashlightActive>(isolate, tpl, "flashlightActive");
                            V8Helpers::SetAccessor<IPlayer, Position, &IPlayer::GetAimPos>(isolate, tpl, "aimPos");

                            // Gamestate getters
                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsJumping>(isolate, tpl, "isJumping");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsInRagdoll>(isolate, tpl, "isInRagdoll");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsAiming>(isolate, tpl, "isAiming");
                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsShooting>(isolate, tpl, "isShooting");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsReloading>(isolate, tpl, "isReloading");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsDead>(isolate, tpl, "isDead");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetMoveSpeed>(isolate, tpl, "moveSpeed");
                            V8Helpers::SetAccessor<IPlayer, Rotation, &IPlayer::GetHeadRotation>(isolate, tpl, "headRot");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetForwardSpeed>(isolate, tpl, "forwardSpeed");
                            V8Helpers::SetAccessor<IPlayer, float, &IPlayer::GetStrafeSpeed>(isolate, tpl, "strafeSpeed");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsEnteringVehicle>(isolate, tpl, "isEnteringVehicle");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsLeavingVehicle>(isolate, tpl, "isLeavingVehicle");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsOnLadder>(isolate, tpl, "isOnLadder");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsInMelee>(isolate, tpl, "isInMelee");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsInCover>(isolate, tpl, "isInCover");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsParachuting>(isolate, tpl, "isParachuting");

                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsSuperJumpEnabled>(isolate, tpl, "isSuperJumpEnabled");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsCrouching>(isolate, tpl, "isCrouching");
                            V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsStealthy>(isolate, tpl, "isStealthy");

                            V8Helpers::SetAccessor(isolate, tpl, "filter", &GetFilter, &SetFilter);
                        });
