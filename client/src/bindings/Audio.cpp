#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/script-objects/IAudio.h"
#include "cpp-sdk/script-objects/IAudioOutput.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(1, 4);

    V8_ARG_TO_STRING(1, source);
    V8_ARG_TO_NUMBER_OPT(2, volume, 1.f);
    V8_ARG_TO_BOOLEAN_OPT(3, radio, false);
    V8_ARG_TO_BOOLEAN_OPT(4, clearCache, true);

    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    auto audio = alt::ICore::Instance().CreateAudio(source, volume, radio, clearCache, origin, resource->GetResource());
    V8_BIND_BASE_OBJECT(audio, "Failed to create Audio");
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    static_cast<CV8ResourceImpl*>(resource)->SubscribeAudio(audio, evName, fun, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    static_cast<CV8ResourceImpl*>(resource)->UnsubscribeAudio(audio, evName, fun);
}

static void GetEventListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_ARG_TO_STRING(1, eventName);

    std::vector<V8Helpers::EventCallback*> handlers = static_cast<CV8ResourceImpl*>(resource)->GetAudioHandlers(audio, eventName);

    auto array = v8::Array::New(isolate, handlers.size());
    for(int i = 0; i < handlers.size(); i++)
    {
        array->Set(ctx, i, handlers[i]->fn.Get(isolate));
    }

    V8_RETURN(array);
}

static void AddOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, entity, alt::IAudioOutput, "AudioOutput");
    audio->AddOutput(entity);
}

static void RemoveOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, entity, alt::IAudioOutput, "AudioOutput");
    audio->RemoveOutput(entity);
}

static void GetOutputs(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    auto list = audio->GetOutputs();
    auto arr = v8::Array::New(isolate, list->GetSize());
    for(int i = 0; i < list->GetSize(); i++)
    {
        auto val = list->Get(i);
        if(val->GetType() == alt::IMValue::Type::BASE_OBJECT)
        {
            auto baseObj = resource->GetBaseObjectOrNull(std::dynamic_pointer_cast<alt::IMValueBaseObject>(val)->RawValue());
            arr->Set(ctx, i, baseObj);
        }
        else if(val->GetType() == alt::IMValue::Type::UINT)
            arr->Set(ctx, i, v8::Integer::NewFromUnsigned(isolate, std::dynamic_pointer_cast<alt::IMValueUInt>(val)->Value()));
    }

    V8_RETURN(arr);
}

static void Seek(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_NUMBER(1, time);

    audio->Seek(time);
}

static void AllAudioGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto objects = alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::AUDIO);
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}

static void AudioCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::AUDIO).size());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::AUDIO, id);

    if(baseObject)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

extern V8Class v8BaseObject;
extern V8Class v8Audio("Audio",
                       v8BaseObject,
                       &Constructor,
                       [](v8::Local<v8::FunctionTemplate> tpl)
                       {
                           using namespace alt;
                           v8::Isolate* isolate = v8::Isolate::GetCurrent();

                           V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

                           V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllAudioGetter);
                           V8Helpers::SetStaticAccessor(isolate, tpl, "count", &AudioCountGetter);

                           V8Helpers::SetMethod(isolate, tpl, "on", &On);
                           V8Helpers::SetMethod(isolate, tpl, "off", &Off);
                           V8Helpers::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);

                           V8Helpers::SetAccessor<IAudio, const std::string&, &IAudio::GetSource, &IAudio::SetSource>(isolate, tpl, "source");
                           V8Helpers::SetAccessor<IAudio, bool, &IAudio::IsLoop, &IAudio::SetLoop>(isolate, tpl, "looped");
                           V8Helpers::SetAccessor<IAudio, float, &IAudio::GetVolume, &IAudio::SetVolume>(isolate, tpl, "volume");
                           V8Helpers::SetAccessor<IAudio, double, &IAudio::GetCurrentTime>(isolate, tpl, "currentTime");
                           V8Helpers::SetAccessor<IAudio, double, &IAudio::GetMaxTime>(isolate, tpl, "maxTime");
                           V8Helpers::SetAccessor<IAudio, bool, &IAudio::IsPlaying>(isolate, tpl, "playing");

                           V8Helpers::SetMethod(isolate, tpl, "addOutput", &AddOutput);
                           V8Helpers::SetMethod(isolate, tpl, "removeOutput", &RemoveOutput);
                           V8Helpers::SetMethod(isolate, tpl, "getOutputs", &GetOutputs);

                           V8Helpers::SetMethod<IAudio, &IAudio::Play>(isolate, tpl, "play");
                           V8Helpers::SetMethod<IAudio, &IAudio::Pause>(isolate, tpl, "pause");
                           V8Helpers::SetMethod<IAudio, &IAudio::Reset>(isolate, tpl, "reset");
                           V8Helpers::SetMethod(isolate, tpl, "seek", &Seek);
                       });
