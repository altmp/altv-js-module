#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/objects/IWorldObject.h"
#include "cpp-sdk/script-objects/IAudioOutput.h"
#include "cpp-sdk/script-objects/IAudioFrontendOutput.h"

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
    V8_CHECK_ARGS_LEN_MIN_MAX(0, 1);

    V8_ARG_TO_INT_OPT(1, categoryHash, alt::ICore::Instance().Hash("radio"));

    auto output = alt::ICore::Instance().CreateFrontendOutput(categoryHash, resource->GetResource());
    V8_BIND_BASE_OBJECT(output, "Failed to create AudioOutputFrontend");
}

static void AllAudioOutputGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto objects = alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::AUDIO_OUTPUT);
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}

static void AudioOutputCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::AUDIO_OUTPUT).size());
}

static void GetFilter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(output, alt::IAudioOutput);

    V8_RETURN_BASE_OBJECT(output->GetFilter());
}

static void SetFilter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(output, alt::IAudioOutput);

    if(val->IsNull())
    {
        output->RemoveFilter();
    }
    else if(val->IsObject())
    {
        V8_TO_BASEOBJECT(val, filter);
        output->AddFilter(filter->SharedAs<alt::IAudioFilter>().get());
    }
    else
    {
        V8Helpers::Throw(isolate, "AudioOutput.filter setter expects null or AudioFilter object");
    }
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
                                 V8Helpers::SetAccessor<IAudioOutput, uint32_t, &IAudioOutput::GetCategory>(isolate, tpl, "category");
                                 V8Helpers::SetAccessor(isolate, tpl, "filter", &GetFilter, &SetFilter);
                             });

extern V8Class v8AudioOutputFrontend("AudioOutputFrontend",
                                     v8AudioOutput,
                                     &ConstructorFrontend,
                                     [](v8::Local<v8::FunctionTemplate> tpl) {

                                     });
