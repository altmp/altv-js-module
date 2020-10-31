
#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Class.h"

static void StaticGetInputMuted(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_RETURN_BOOLEAN(alt::ICore::Instance().IsVoiceInputMuted());
}

static void StaticSetInputMuted(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();
	V8_TO_BOOLEAN(value, state);

	alt::ICore::Instance().SetVoiceInputMuted(state);
}

static V8Class v8Voice("Voice", "", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetStaticAccessor(isolate, tpl, "muteInput", StaticGetInputMuted, StaticSetInputMuted);
});
