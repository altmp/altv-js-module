
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

    alt::Array<uint32_t> comps = player->GetCurrentWeaponComponents();
    v8::Local<v8::Array> componentsArray = V8Helpers::JSValue(comps);

    V8_RETURN(componentsArray);
}

static void WeaponHasComponent(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_INT(1, weaponHash);
    V8_ARG_TO_INT(2, componentHash);

    V8_RETURN_BOOLEAN(player->HasWeaponComponent(weaponHash, componentHash));
}

static void GetWeaponTintIndex(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(player, alt::IPlayer);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INT(1, weaponHash);

    V8_RETURN_INT(player->GetWeaponTintIndex(weaponHash));
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllPlayers()->Clone());
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
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid));
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IEntity* entity = alt::ICore::Instance().GetEntityByID(id);

    if(entity && (entity->GetType() == alt::IEntity::Type::PLAYER || entity->GetType() == alt::IEntity::Type::LOCAL_PLAYER))
    {
        V8_RETURN_BASE_OBJECT(entity);
    }
    else
    {
        V8_RETURN_NULL();
    }
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

                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);
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

                            // Weapon getters
                            V8Helpers::SetAccessor(isolate, tpl, "currentWeaponComponents", &CurrentWeaponComponentsGetter);
                            // V8Helpers::SetAccessor(isolate, tpl, "currentWeaponTintIndex", &CurrentWeaponTintIndexGetter);
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

                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsSuperJumpEnabled>(isolate, tpl, "isSuperJumpEnabled");
                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsCrouching>(isolate, tpl, "isCrouching");
                            // V8Helpers::SetAccessor<IPlayer, bool, &IPlayer::IsStealthy>(isolate, tpl, "isStealthy");
                        });
