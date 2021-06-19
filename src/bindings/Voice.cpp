
#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Class.h"

static void StaticGetInputMuted(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	
	V8_RETURN_BOOLEAN(alt::ICore::Instance().IsVoiceInputMuted());
}

static void StaticSetInputMuted(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_TO_BOOLEAN(value, state);

	alt::ICore::Instance().SetVoiceInputMuted(state);
}

static void StaticGetVoiceActivityInputEnabled(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_RETURN_BOOLEAN(alt::ICore::Instance().IsVoiceActivationEnabled());
}

static void StaticGetVoiceActivationKey(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_RETURN_UINT(alt::ICore::Instance().GetVoiceActivationKey());
}

extern V8Class v8Voice("Voice", [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticAccessor(isolate, tpl, "muteInput", StaticGetInputMuted, StaticSetInputMuted);
	V8::SetStaticAccessor(isolate, tpl, "activityInputEnabled", StaticGetVoiceActivityInputEnabled);
	V8::SetStaticAccessor(isolate, tpl, "activationKey", StaticGetVoiceActivationKey);
});
