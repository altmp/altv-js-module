#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IAudio.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(2, 4);

    V8_ARG_TO_STRING(1, source);
    V8_ARG_TO_NUMBER(2, volume);

    uint32_t category = 0;
    bool frontend = false;
    if(info.Length() == 3 || info.Length() == 4)
    {
        if(info[2]->IsNumber())
        {
            V8_ARG_TO_UINT(3, categ);
            category = categ;
        }
        else
        {
            V8_ARG_TO_STRING(3, categ);
            category = alt::ICore::Instance().Hash(categ);
        }
        if(info.Length() == 4)
        {
            V8_ARG_TO_BOOLEAN(4, frntnd);
            frontend = frntnd;
        }
    }

    auto audio = alt::ICore::Instance().CreateAudio(source, volume, category, frontend, resource->GetResource());
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

static void CategoryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_UINT(audio->GetCategory());
}

static void CategorySetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    int64_t category;
    if(val->IsNumber())
    {
        V8_TO_INTEGER(val, categ);
        category = categ;
    }
    else if(val->IsString())
    {
        V8_TO_STRING(val, categ);
        category = alt::ICore::Instance().Hash(categ);
    }
    audio->SetCategory(category);
}

static void AddOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    if(info[0]->IsInt32() || info[0]->IsUint32())
    {
        V8_ARG_TO_INT(1, scriptId);
        audio->AddOutput(scriptId);
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, entity, alt::IEntity, "Entity");
        audio->AddOutput(entity);
    }
}

static void RemoveOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    if(info[0]->IsInt32() || info[0]->IsUint32())
    {
        V8_ARG_TO_INT(1, scriptId);
        audio->RemoveOutput(scriptId);
    }
    else
    {
        V8_ARG_TO_BASE_OBJECT(1, entity, alt::IEntity, "Entity");
        audio->RemoveOutput(entity);
    }
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
            auto baseObj = resource->GetBaseObjectOrNull(val.As<alt::IMValueBaseObject>()->RawValue());
            arr->Set(ctx, i, baseObj);
        }
        else if(val->GetType() == alt::IMValue::Type::UINT)
            arr->Set(ctx, i, v8::Integer::NewFromUnsigned(isolate, val.As<alt::IMValueUInt>()->Value()));
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
    auto objects = alt::ICore::Instance().GetAudios();
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}


static void AudioCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetAudios().size());
}

extern V8Class v8BaseObject;
extern V8Class v8Audio("Audio",
                       v8BaseObject,
                       &Constructor,
                       [](v8::Local<v8::FunctionTemplate> tpl)
                       {
                           using namespace alt;
                           v8::Isolate* isolate = v8::Isolate::GetCurrent();

                           V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllAudioGetter);
                           V8Helpers::SetStaticAccessor(isolate, tpl, "count", &AudioCountGetter);

                           V8Helpers::SetMethod(isolate, tpl, "on", &On);
                           V8Helpers::SetMethod(isolate, tpl, "off", &Off);
                           V8Helpers::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);

                           V8Helpers::SetAccessor<IAudio, uint32_t, &IAudio::GetID>(isolate, tpl, "id");
                           V8Helpers::SetAccessor<IAudio, const std::string&, &IAudio::GetSource, &IAudio::SetSource>(isolate, tpl, "source");
                           V8Helpers::SetAccessor<IAudio, bool, &IAudio::IsLoop, &IAudio::SetLoop>(isolate, tpl, "looped");
                           V8Helpers::SetAccessor<IAudio, float, &IAudio::GetVolume, &IAudio::SetVolume>(isolate, tpl, "volume");
                           V8Helpers::SetAccessor(isolate, tpl, "category", &CategoryGetter, &CategorySetter);
                           V8Helpers::SetAccessor<IAudio, bool, &IAudio::IsFrontendPlay>(isolate, tpl, "frontendPlay");
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

                           V8Helpers::SetAccessor<IAudio, uint32_t, &IAudio::GetRemoteID>(isolate, tpl, "remoteId");
                       });
