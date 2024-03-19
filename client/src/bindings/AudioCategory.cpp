#include "cpp-sdk/ICore.h"
#include "V8Class.h"
#include "V8Helpers.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    if(!info[0]->IsString())
    {
        V8_CHECK(false, "Argument 0 must be a string");
    }

    V8_ARG_TO_STRING(1, audioCategoryStr);
    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::AUDIO_CATEGORY);
    info.This()->SetInternalField(1, v8::String::NewFromUtf8(isolate, audioCategoryStr.c_str()).ToLocalChecked());
}

// Getters
static void NameGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_STRING(audioCategory->GetName());
}
static void VolumeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetVolume());
}
static void DistanceRolloffScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetDistanceRolloffScale());
}
static void PlateauRolloffScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetPlateauRolloffScale());
}
static void OcclusionDampingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetOcclusionDamping());
}
static void EnvironmentalFilterDampingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetEnvironmentalFilterDamping());
}
static void SourceReverbDampingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetSourceReverbDamping());
}
static void DistanceReverbDampingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetDistanceReverbDamping());
}
static void InteriorReverbDampingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetInteriorReverbDamping());
}
static void EnvironmentalLoudnessGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetEnvironmentalLoudness());
}
static void UnderwaterWetLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetUnderwaterWetLevel());
}
static void StonedWetLevelGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_NUMBER(audioCategory->GetStonedWetLevel());
}
static void PitchGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_INT(audioCategory->GetPitch());
}
static void LowPassFilterCutoffGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_INT(audioCategory->GetLowPassFilterCutoff());
}
static void HighPassFilterCutoffGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_RETURN_INT(audioCategory->GetHighPassFilterCutoff());
}

// Setters
static void VolumeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetVolume(value);
}
static void DistanceRolloffScaleSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetDistanceRolloffScale(value);
}
static void PlateauRolloffScaleSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetPlateauRolloffScale(value);
}
static void OcclusionDampingSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetOcclusionDamping(value);
}
static void EnvironmentalFilterDampingSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetEnvironmentalFilterDamping(value);
}
static void SourceReverbDampingSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetSourceReverbDamping(value);
}
static void DistanceReverbDampingSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetDistanceReverbDamping(value);
}
static void InteriorReverbDampingSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetInteriorReverbDamping(value);
}
static void EnvironmentalLoudnessSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetEnvironmentalLoudness(value);
}
static void UnderwaterWetLevelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetUnderwaterWetLevel(value);
}
static void StonedWetLevelSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_NUMBER(val, value);
    audioCategory->SetStonedWetLevel(value);
}
static void PitchSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_INT32(val, value);
    audioCategory->SetPitch(value);
}
static void LowPassFilterCutoffSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_INT32(val, value);
    audioCategory->SetLowPassFilterCutoff(value);
}
static void HighPassFilterCutoffSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    V8_TO_INT32(val, value);
    audioCategory->SetHighPassFilterCutoff(value);
}

// Methods
static void GetForName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, audioCategoryStr);

    std::vector<v8::Local<v8::Value>> args{ V8Helpers::JSValue(audioCategoryStr) };
    extern V8Class v8AudioCategory;
    V8_RETURN(v8AudioCategory.New(isolate->GetEnteredOrMicrotaskContext(), args));
}
static void Reset(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_STRING(1, audioCategoryStr);

    auto audioCategory = alt::ICore::Instance().GetAudioCategory(audioCategoryStr);
    V8_CHECK(audioCategory, "Audio category not found");

    audioCategory->Reset();
}

extern V8Class v8AudioCategory("AudioCategory",
                            Constructor,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();
                                tpl->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(V8Class::InternalFields::COUNT));

                                V8Helpers::SetStaticMethod(isolate, tpl, "getForName", &GetForName);

                                V8Helpers::SetAccessor(isolate, tpl, "name", &NameGetter);
                                V8Helpers::SetAccessor(isolate, tpl, "volume", &VolumeGetter, &VolumeSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "distanceRolloffScale", &DistanceRolloffScaleGetter, &DistanceRolloffScaleSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "plateauRolloffScale", &PlateauRolloffScaleGetter, &PlateauRolloffScaleSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "occlusionDamping", &OcclusionDampingGetter, &OcclusionDampingSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "environmentalFilterDamping", &EnvironmentalFilterDampingGetter, &EnvironmentalFilterDampingSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "sourceReverbDamping", &SourceReverbDampingGetter, &SourceReverbDampingSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "distanceReverbDamping", &DistanceReverbDampingGetter, &DistanceReverbDampingSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "interiorReverbDamping", &InteriorReverbDampingGetter, &InteriorReverbDampingSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "environmentalLoudness", &EnvironmentalLoudnessGetter, &EnvironmentalLoudnessSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "underwaterWetLevel", &UnderwaterWetLevelGetter, &UnderwaterWetLevelSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "stonedWetLevel", &StonedWetLevelGetter, &StonedWetLevelSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "pitch", &PitchGetter, &PitchSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "lowPassFilterCutoff", &LowPassFilterCutoffGetter, &LowPassFilterCutoffSetter);
                                V8Helpers::SetAccessor(isolate, tpl, "highPassFilterCutoff", &HighPassFilterCutoffGetter, &HighPassFilterCutoffSetter);

                                V8Helpers::SetMethod(isolate, tpl, "reset", &Reset);
                            });
