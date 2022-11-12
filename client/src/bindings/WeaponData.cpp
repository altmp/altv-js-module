#include "cpp-sdk/ICore.h"
#include "V8Class.h"
#include "V8Helpers.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    uint32_t weaponHash;
    if(info[0]->IsString())
    {
        V8_ARG_TO_STRING(1, weaponHashStr);
        weaponHash = alt::ICore::Instance().Hash(weaponHashStr);
    }
    else if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, weaponHashNum);
        weaponHash = weaponHashNum;
    }
    else
        V8_CHECK(false, "Argument 0 must be a string or a number");

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    info.This()->SetInternalField(0, v8::Integer::NewFromUnsigned(isolate, weaponHash));
}

// Getters
static void ModelHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_UINT(weaponData->GetModelHash());
}
static void NameHashGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_UINT(weaponData->GetNameHash());
}
static void RecoilShakeAmplitudeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetRecoilShakeAmplitude());
}
static void RecoilAccuracyMaxGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetRecoilAccuracyMax());
}
static void RecoilAccuracyToAllowHeadshotPlayerGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetRecoilAccuracyToAllowHeadshotPlayer());
}
static void RecoilRecoveryRateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetRecoilRecoveryRate());
}
static void AnimReloadRateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetAnimReloadRate());
}
static void VehicleReloadRateGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetVehicleReloadTime());
}
static void LockOnRangeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetLockOnRange());
}
static void AccuracySpreadGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetAccuracySpread());
}
static void RangeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetRange());
}
static void DamageGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetDamage());
}
static void ClipSizeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_UINT(weaponData->GetClipSize());
}
static void TimeBetweenShotsGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_UINT(weaponData->GetTimeBetweenShots());
}
static void HeadshotDamageModifierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetHeadshotDamageModifier());
}
static void PlayerDamageModifierGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");
    V8_RETURN_NUMBER(weaponData->GetPlayerDamageModifier());
}

// Setters
static void RecoilShakeAmplitudeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetRecoilShakeAmplitude(value);
}
static void RecoilAccuracyMaxSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetRecoilAccuracyMax(value);
}
static void RecoilAccuracyToAllowHeadshotPlayerSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetRecoilAccuracyToAllowHeadshotPlayer(value);
}
static void RecoilRecoveryRateSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetRecoilRecoveryRate(value);
}
static void AnimReloadRateSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetAnimReloadRate(value);
}
static void VehicleReloadTimeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetVehicleReloadTime(value);
}
static void LockOnRangeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetLockOnRange(value);
}
static void AccuracySpreadSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetAccuracySpread(value);
}
static void RangeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetRange(value);
}
static void DamageSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetDamage(value);
}
static void HeadshotDamageModifierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetHeadshotDamageModifier(value);
}
static void PlayerDamageModifierSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, weaponHash);

    auto weaponData = alt::ICore::Instance().GetWeaponData(weaponHash);
    V8_CHECK(weaponData, "Weapon data not found");

    V8_TO_NUMBER(val, value);
    weaponData->SetPlayerDamageModifier(value);
}

static void GetForHash(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, hash);

    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(hash) };
    extern V8Class v8WeaponData;
    V8_RETURN(v8WeaponData.New(isolate->GetEnteredOrMicrotaskContext(), args));
}

extern V8Class v8WeaponData("WeaponData",
                            Constructor,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();
                                tpl->InstanceTemplate()->SetInternalFieldCount(1);

                                V8Helpers::SetStaticMethod(isolate, tpl, "getForHash", &GetForHash);

                                V8Helpers::SetAccessor(isolate, tpl, "modelHash", &ModelHashGetter);
                                V8Helpers::SetAccessor(isolate, tpl, "nameHash", &NameHashGetter);
                                V8Helpers::SetAccessor(isolate, tpl, "recoilShakeAmplitude", &RecoilShakeAmplitudeGetter, &RecoilShakeAmplitudeSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "recoilAccuracyMax", &RecoilAccuracyMaxGetter, &RecoilAccuracyMaxSetter);
                                V8Helpers::SetAccessor(
                                  isolate, tpl, "recoilAccuracyToAllowHeadshotPlayer", &RecoilAccuracyToAllowHeadshotPlayerGetter, &RecoilAccuracyToAllowHeadshotPlayerSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "recoilRecoveryRate", &RecoilRecoveryRateGetter, &RecoilRecoveryRateSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "animReloadRate", &AnimReloadRateGetter, &AnimReloadRateSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "vehicleReloadTime", &VehicleReloadRateGetter, &VehicleReloadTimeSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "lockOnRange", &LockOnRangeGetter, &LockOnRangeSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "accuracySpread", &AccuracySpreadGetter, &AccuracySpreadSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "range", &RangeGetter, &RangeSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "damage", &DamageGetter, &DamageSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "clipSize", &ClipSizeGetter);
                                V8Helpers::SetAccessor(isolate, tpl, "timeBetweenShots", &TimeBetweenShotsGetter);
                                V8Helpers::SetAccessor(isolate, tpl, "headshotDamageModifier", &HeadshotDamageModifierGetter, &HeadshotDamageModifierSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "playerDamageModifier", &PlayerDamageModifierGetter, &PlayerDamageModifierSetter);
                            });
