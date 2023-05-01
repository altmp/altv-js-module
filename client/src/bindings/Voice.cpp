
#include "V8Helpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"

static void StaticGetInputMuted(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsVoiceInputMuted());
}

static void StaticSetInputMuted(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_TO_BOOLEAN(value, state);

    alt::ICore::Instance().SetVoiceInputMuted(state);
}

static void StaticGetVoiceActivityInputEnabled(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsVoiceActivationEnabled());
}

static void StaticSetVoiceActivityInputEnabled(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_TO_BOOLEAN(value, enabled);

    auto state = alt::ICore::Instance().ToggleVoiceActivation(enabled);
    V8_CHECK(state != alt::PermissionState::DENIED, "No permissions");
    V8_CHECK(state != alt::PermissionState::UNSPECIFIED, "Permissions not specified");
}

static void StaticGetVoiceActivationKey(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_UINT(alt::ICore::Instance().GetVoiceActivationKey());
}

static void AreVoiceControlsEnabled(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_BOOLEAN(alt::ICore::Instance().AreVoiceControlsEnabled());
}

static void StaticGetVoiceActivationLevel(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_NUMBER(alt::ICore::Instance().GetVoiceActivationLevel());
}

static void StaticSetVoiceActivationLevel(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_TO_NUMBER(value, level);

    auto state = alt::ICore::Instance().SetVoiceActivationLevel(level);
    V8_CHECK(state != alt::PermissionState::DENIED, "No permissions");
    V8_CHECK(state != alt::PermissionState::UNSPECIFIED, "Permissions not specified");
}

static void StaticGetNoiseSuppressionEnabled(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsNoiseSuppressionEnabled());
}

static void StaticSetNoiseSuppressionEnabled(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_TO_BOOLEAN(value, enabled);

    auto state = alt::ICore::Instance().ToggleNoiseSuppression(enabled);
    V8_CHECK(state != alt::PermissionState::DENIED, "No permissions");
    V8_CHECK(state != alt::PermissionState::UNSPECIFIED, "Permissions not specified");
}

static void ToggleInput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_BOOLEAN(1, enabled);

    auto state = alt::ICore::Instance().ToggleVoiceInput(enabled);
    V8_CHECK(state != alt::PermissionState::DENIED, "No permissions");
    V8_CHECK(state != alt::PermissionState::UNSPECIFIED, "Permissions not specified");
}

extern V8Class v8Voice("Voice", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "muteInput", StaticGetInputMuted, StaticSetInputMuted);
    V8Helpers::SetStaticAccessor(isolate, tpl, "activityInputEnabled", StaticGetVoiceActivityInputEnabled, StaticSetVoiceActivityInputEnabled);
    V8Helpers::SetStaticAccessor(isolate, tpl, "activationLevel", StaticGetVoiceActivationLevel, StaticSetVoiceActivationLevel);
    V8Helpers::SetStaticAccessor(isolate, tpl, "activationKey", StaticGetVoiceActivationKey);
    V8Helpers::SetStaticAccessor(isolate, tpl, "voiceControlsEnabled", AreVoiceControlsEnabled);
    V8Helpers::SetStaticAccessor(isolate, tpl, "noiseSuppressionEnabled", StaticGetNoiseSuppressionEnabled, StaticSetNoiseSuppressionEnabled);
    V8Helpers::SetStaticMethod(isolate, tpl, "toggleInput", ToggleInput);
});
