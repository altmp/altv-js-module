#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IAudioFilter.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    auto& core = alt::ICore::Instance();

    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);

    alt::IResource* altres = V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext());
    V8_CHECK(altres, "invalid resource");
    
    IAudioFilter* filter = core.CreateAudioFilter(core.Hash(name), altres);
    if(!filter)
    {
        V8Helpers::Throw(isolate, "Failed to create AudioFilter");
        return;
    }

    V8_BIND_BASE_OBJECT(filter, "Failed to create AudioFilter");
}

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    auto filter = info.This();
    V8_OBJECT_GET_INT(filter, "hash", hash);

    std::ostringstream ss;
    ss << "AudioFilter{ hash: " << hash << " }";

    V8_RETURN_STRING(ss.str());
}

static void AddRotateEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_NUMBER(1, fRate);
    V8_ARG_TO_INT(2, priority);

    V8_RETURN_UINT(filter->AddRotateEffect(fRate, priority));
}

static void AddVolumeEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_NUMBER(1, fVolume);
    V8_ARG_TO_INT(2, priority);

    V8_RETURN_UINT(filter->AddVolumeEffect(fVolume, priority));
}

static void AddPeakeqEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(6);
    V8_ARG_TO_INT(1, lBand);
    V8_ARG_TO_NUMBER(2, fBandwidth);
    V8_ARG_TO_NUMBER(3, fQ);
    V8_ARG_TO_NUMBER(4, fCenter);
    V8_ARG_TO_NUMBER(5, fGain);
    V8_ARG_TO_INT(6, priority);

    V8_RETURN_UINT(filter->AddPeakeqEffect(lBand, fBandwidth, fQ, fCenter, fGain, priority));
}

static void AddDampEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(6);
    V8_ARG_TO_NUMBER(1, fTarget);
    V8_ARG_TO_NUMBER(2, fQuiet);
    V8_ARG_TO_NUMBER(3, fRate);
    V8_ARG_TO_NUMBER(4, fGain);
    V8_ARG_TO_NUMBER(5, fDelay);
    V8_ARG_TO_INT(6, priority);

    V8_RETURN_UINT(filter->AddDampEffect(fTarget, fQuiet, fRate, fGain, fDelay, priority));
}

static void AddAutowahEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(7);
    V8_ARG_TO_NUMBER(1, fDryMix);
    V8_ARG_TO_NUMBER(2, fWetMix);
    V8_ARG_TO_NUMBER(3, fFeedback);
    V8_ARG_TO_NUMBER(4, fRate);
    V8_ARG_TO_NUMBER(5, fRange);
    V8_ARG_TO_NUMBER(6, fFreq);
    V8_ARG_TO_INT(7, priority);

    V8_RETURN_UINT(filter->AddAutowahEffect(fDryMix, fWetMix, fFeedback, fRate, fRange, fFreq, priority));
}

static void AddPhaserEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(7);
    V8_ARG_TO_NUMBER(1, fDryMix);
    V8_ARG_TO_NUMBER(2, fWetMix);
    V8_ARG_TO_NUMBER(3, fFeedback);
    V8_ARG_TO_NUMBER(4, fRate);
    V8_ARG_TO_NUMBER(5, fRange);
    V8_ARG_TO_NUMBER(6, fFreq);
    V8_ARG_TO_UINT(7, priority);

    V8_RETURN_UINT(filter->AddPhaserEffect(fDryMix, fWetMix, fFeedback, fRate, fRange, fFreq, priority));
}

static void AddChorusEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(6);
    V8_ARG_TO_NUMBER(1, fDryMix);
    V8_ARG_TO_NUMBER(2, fWetMix);
    V8_ARG_TO_NUMBER(3, fFeedback);
    V8_ARG_TO_NUMBER(4, fMinSweep);
    V8_ARG_TO_NUMBER(5, fMaxSweep);
    V8_ARG_TO_NUMBER(6, fRate);
    V8_ARG_TO_UINT(7, priority);

    V8_RETURN_UINT(filter->AddChorusEffect(fDryMix, fWetMix, fFeedback, fMinSweep, fMaxSweep, fRate, priority));
}

static void AddDistortionEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(6);
    V8_ARG_TO_NUMBER(1, fDrive);
    V8_ARG_TO_NUMBER(2, fDryMix);
    V8_ARG_TO_NUMBER(3, fWetMix);
    V8_ARG_TO_NUMBER(4, fFeedback);
    V8_ARG_TO_NUMBER(5, fVolume);
    V8_ARG_TO_INT(6, priority);

    V8_RETURN_UINT(filter->AddDistortionEffect(fDrive, fDryMix, fWetMix, fFeedback, fVolume, priority));
}

static void AddCompressor2Effect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(6);
    V8_ARG_TO_NUMBER(1, fGain);
    V8_ARG_TO_NUMBER(2, fThreshold);
    V8_ARG_TO_NUMBER(3, fRatio);
    V8_ARG_TO_NUMBER(4, fAttack);
    V8_ARG_TO_NUMBER(5, fRelease);
    V8_ARG_TO_INT(6, priority);

    V8_RETURN_UINT(filter->AddCompressor2Effect(fGain, fThreshold, fRatio, fAttack, fRelease, priority));
}

static void AddBqfEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(7);
    V8_ARG_TO_INT(1, lFilter);
    V8_ARG_TO_NUMBER(2, fCenter);
    V8_ARG_TO_NUMBER(3, fGain);
    V8_ARG_TO_NUMBER(4, fBandwidth);
    V8_ARG_TO_NUMBER(5, fQ);
    V8_ARG_TO_NUMBER(6, fS);
    V8_ARG_TO_INT(7, priority);

    V8_RETURN_UINT(filter->AddBqfEffect(lFilter, fCenter, fGain, fBandwidth, fQ, fS, priority));
}

static void AddEcho4Effect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(4);
    V8_ARG_TO_NUMBER(1, fDryMix);
    V8_ARG_TO_NUMBER(2, fWetMix);
    V8_ARG_TO_NUMBER(3, fFeedback);
    V8_ARG_TO_NUMBER(4, fDelay);
    V8_ARG_TO_UINT(5, priority);

    V8_RETURN_UINT(filter->AddEcho4Effect(fDryMix, fWetMix, fFeedback, fDelay, priority));
}

static void AddPitchshiftEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(5);
    V8_ARG_TO_NUMBER(1, fPitchShift);
    V8_ARG_TO_NUMBER(2, fSemitones);
    V8_ARG_TO_INT(3, lFFTsize);
    V8_ARG_TO_INT(4, lOsamp);
    V8_ARG_TO_INT(5, priority);

    V8_RETURN_UINT(filter->AddPitchshiftEffect(fPitchShift, fSemitones, lFFTsize, lOsamp, priority));
}

static void AddFreeverbEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(7);
    V8_ARG_TO_NUMBER(1, fDryMix);
    V8_ARG_TO_NUMBER(2, fWetMix);
    V8_ARG_TO_NUMBER(3, fRoomSize);
    V8_ARG_TO_NUMBER(4, fDamp);
    V8_ARG_TO_NUMBER(5, fWidth);
    V8_ARG_TO_UINT(6, lMode);
    V8_ARG_TO_INT(7, priority);

    V8_RETURN_UINT(filter->AddFreeverbEffect(fDryMix, fWetMix, fRoomSize, fDamp, fWidth, lMode, priority));
}

static void RemoveEffect(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(filter, alt::IAudioFilter);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_UINT(1, hfxHandler);

    V8_RETURN_BOOLEAN(filter->RemoveEffect(hfxHandler));
}

extern V8Class v8BaseObject;
extern V8Class v8AudioFilter("AudioFilter",
                         v8BaseObject,
                         &Constructor,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                             v8::Isolate* isolate = v8::Isolate::GetCurrent();

                             V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

                             V8Helpers::SetAccessor<IAudioFilter, uint32_t, &IAudioFilter::GetHash>(isolate, tpl, "hash");

                             V8Helpers::SetMethod(isolate, tpl, "addRotateEffect", &AddRotateEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addVolumeEffect", &AddVolumeEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addPeakeqEffect", &AddPeakeqEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addDampEffect", &AddDampEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addAutowahEffect", &AddAutowahEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addPhaserEffect", &AddPhaserEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addChorusEffect", &AddChorusEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addDistortionEffect", &AddDistortionEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addCompressor2Effect", &AddCompressor2Effect);
                             V8Helpers::SetMethod(isolate, tpl, "addBqfEffect", &AddBqfEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addEcho4Effect", &AddEcho4Effect);
                             V8Helpers::SetMethod(isolate, tpl, "addPitchshiftEffect", &AddPitchshiftEffect);
                             V8Helpers::SetMethod(isolate, tpl, "addFreeverbEffect", &AddFreeverbEffect);

                             V8Helpers::SetMethod(isolate, tpl, "removeEffect", &RemoveEffect);
                         });
