#include "../helpers/V8Helpers.h"
#include "../helpers/V8ResourceImpl.h"
#include "../helpers/V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IAudio.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(2, 3);

    V8_ARG_TO_STRING(1, source);
    V8_ARG_TO_NUMBER(2, volume);

    uint32_t category = 0;
    if(info.Length() == 3)
    {
        if(info[2]->IsNumber())
        {
            V8_ARG_TO_UINT32(3, categ);
            category = categ;
        }
        else if(info[2]->IsString())
        {
            V8_ARG_TO_STRING(3, categ);
            category = alt::ICore::Instance().Hash(categ);
        }
    }

    auto audio = alt::ICore::Instance().CreateAudio(source, volume, category, resource->GetResource());
    V8_BIND_BASE_OBJECT(audio, "Failed to create Audio");
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    static_cast<CV8ResourceImpl*>(resource)->SubscribeAudio(audio, evName.ToString(), fun, V8::SourceLocation::GetCurrent(isolate));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    static_cast<CV8ResourceImpl*>(resource)->UnsubscribeAudio(audio, evName.ToString(), fun);
}

static void GetEventListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_ARG_TO_STRING(1, eventName);

    std::vector<V8::EventCallback*> handlers = static_cast<CV8ResourceImpl*>(resource)->GetAudioHandlers(audio, eventName.ToString());

    auto array = v8::Array::New(isolate, handlers.size());
    for (int i = 0; i < handlers.size(); i++)
    {
        array->Set(ctx, i, handlers[i]->fn.Get(isolate));
    }

    V8_RETURN(array);
}

static void SourceGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_ALT_STRING(audio->GetSource());
}

static void SourceSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    
    V8_TO_STRING(val, source);
    audio->SetSource(source);
}

static void LoopedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_BOOLEAN(audio->IsLoop());
}

static void LoopedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    
    V8_TO_BOOLEAN(val, looped);
    audio->SetLoop(looped);
}

static void VolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_NUMBER(audio->GetVolume());
}

static void VolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    
    V8_TO_NUMBER(val, volume);
    audio->SetVolume(volume);
}

static void CategoryGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_UINTEGER(audio->GetCategory());
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

static void FrontendPlayGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_BOOLEAN(audio->IsFrontendPlay());
}

static void FrontendPlaySetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    
    V8_TO_BOOLEAN(val, frontendPlay);
    audio->SetFrontendPlay(frontendPlay);
}

static void AddOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    if(info[0]->IsInt32() || info[0]->IsUint32())
    {
        V8_ARG_TO_INTEGER(1, scriptId);
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
        V8_ARG_TO_INTEGER(1, scriptId);
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
            auto baseObj = resource->GetBaseObjectOrNull(val.As<alt::IMValueBaseObject>()->Value());
            arr->Set(ctx, i, baseObj);
        }
        else if(val->GetType() == alt::IMValue::Type::UINT) 
            arr->Set(ctx, i, v8::Integer::NewFromUnsigned(isolate, val.As<alt::IMValueUInt>()->Value()));
    }

    V8_RETURN(arr);
}

static void CurrentTimeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_NUMBER(audio->GetCurrentTime());
}

static void MaxTimeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_NUMBER(audio->GetMaxTime());
}

static void PlayingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    V8_RETURN_BOOLEAN(audio->IsPlaying());
}

static void Play(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    audio->Play();
}

static void Pause(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    audio->Pause();
}

static void Reset(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);

    audio->Reset();
}

static void Seek(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IAudio);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_NUMBER(1, time);

    audio->Seek(time);
}

extern V8Class v8BaseObject;
extern V8Class v8Audio("Audio", v8BaseObject, &Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    V8::SetMethod(isolate, tpl, "on", &On);
    V8::SetMethod(isolate, tpl, "off", &Off);
    V8::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);

    V8::SetAccessor(isolate, tpl, "source", &SourceGetter, &SourceSetter);
    V8::SetAccessor(isolate, tpl, "looped", &LoopedGetter, &LoopedSetter);
    V8::SetAccessor(isolate, tpl, "volume", &VolumeGetter, &VolumeSetter);
    V8::SetAccessor(isolate, tpl, "category", &CategoryGetter, &CategorySetter);
    V8::SetAccessor(isolate, tpl, "frontendPlay", &FrontendPlayGetter, &FrontendPlaySetter);
    V8::SetAccessor(isolate, tpl, "currentTime", &CurrentTimeGetter);
    V8::SetAccessor(isolate, tpl, "maxTime", &MaxTimeGetter);
    V8::SetAccessor(isolate, tpl, "playing", &PlayingGetter);

    V8::SetMethod(isolate, tpl, "addOutput", &AddOutput);
    V8::SetMethod(isolate, tpl, "removeOutput", &RemoveOutput);
    V8::SetMethod(isolate, tpl, "getOutputs", &GetOutputs);

    V8::SetMethod(isolate, tpl, "play", &Play);
    V8::SetMethod(isolate, tpl, "pause", &Pause);
    V8::SetMethod(isolate, tpl, "reset", &Reset);
    V8::SetMethod(isolate, tpl, "seek", &Seek);
});
