#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IAudio.h"
#include "cpp-sdk/script-objects/IAudioOutput.h"
#include "cpp-sdk/script-objects/IAudioFrontendOutput.h"
#include "cpp-sdk/script-objects/IAudioWorldOutput.h"
#include "cpp-sdk/script-objects/IAudioAttachedOutput.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK(false, "You can't use constructor of abstract class");
}

static void ConstructorFrontend(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(0);

    auto output = alt::ICore::Instance().CreateFrontendOutput(0, resource->GetResource());
    V8_BIND_BASE_OBJECT(output, "Failed to create AudioOutputFrontend");
}

static void ConstructorWorld(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_VECTOR3(1, pos);
    
    auto output = alt::ICore::Instance().CreateWorldOutput(0, pos, resource->GetResource());
    V8_BIND_BASE_OBJECT(output, "Failed to create AudioOutputWorld");
}

static void ConstructorAttached(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, entity, alt::IWorldObject, "IWorldObject");

    auto output = alt::ICore::Instance().CreateAttachedOutput(0, entity, resource->GetResource());
    V8_BIND_BASE_OBJECT(output, "Failed to create AudioOutputAttached");
}

static void AllAudioOutputGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto objects = alt::ICore::Instance().GetAudioOutputs();
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}

static void AudioOutputCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetAudioOutputs().size());
}

using namespace alt;

extern V8Class v8BaseObject;
extern V8Class v8AudioOutput("AudioOutput",
    v8BaseObject,
    &Constructor,
    [](v8::Local<v8::FunctionTemplate> tpl)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();

        V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllAudioOutputGetter);
        V8Helpers::SetStaticAccessor(isolate, tpl, "count", &AudioOutputCountGetter);

        V8Helpers::SetAccessor<IAudioOutput, bool, &IAudioOutput::IsMuted, &IAudioOutput::SetMuted>(isolate, tpl, "muted");
        V8Helpers::SetAccessor<IAudioOutput, float, &IAudioOutput::GetVolume, &IAudioOutput::SetVolume>(isolate, tpl, "volume");
        V8Helpers::SetAccessor<IAudioOutput, uint32_t, &IAudioOutput::GetCategory, &IAudioOutput::SetCategory>(isolate, tpl, "category");
    });


extern V8Class v8AudioOutputFrontend("AudioOutputFrontend",
    v8AudioOutput,
    &ConstructorFrontend,
    [](v8::Local<v8::FunctionTemplate> tpl)
    {

    });

extern V8Class v8AudioOutputWorld("AudioOutputWorld",
    v8AudioOutput,
    &ConstructorWorld,
    [](v8::Local<v8::FunctionTemplate> tpl)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        V8Helpers::SetAccessor<IAudioWorldOutput, alt::Position, &IAudioWorldOutput::GetPosition, &IAudioWorldOutput::SetPosition>(isolate, tpl, "pos");
    });

extern V8Class v8AudioOutputAttached("AudioOutputAttached",
    v8AudioOutput,
    &ConstructorAttached,
    [](v8::Local<v8::FunctionTemplate> tpl)
    {

    });
