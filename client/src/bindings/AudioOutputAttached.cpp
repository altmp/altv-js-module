#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"

#include "../CV8ScriptRuntime.h"

#include "cpp-sdk/objects/IWorldObject.h"
#include "cpp-sdk/script-objects/IAudioOutput.h"
#include "cpp-sdk/script-objects/IAudioAttachedOutput.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(1, 2);

    V8_ARG_TO_BASE_OBJECT(1, entity, alt::IWorldObject, "IWorldObject");
    V8_ARG_TO_INT_OPT(2, categoryHash, alt::ICore::Instance().Hash("radio"));

    auto output = alt::ICore::Instance().CreateAttachedOutput(categoryHash, entity, resource->GetResource());
    V8_BIND_BASE_OBJECT(output, "Failed to create AudioOutputAttached");
}

static void EntityGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IAudioAttachedOutput);
    V8_RETURN_BASE_OBJECT(ent->GetEntity());
}

static void EntitySetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(_this, alt::IAudioAttachedOutput);

    V8_TO_WORLDOBJECT(val, entity);

    _this->SetEntity(entity);
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::AUDIO_OUTPUT_ATTACHED, id);

    if(baseObject)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

using namespace alt;

extern V8Class v8AudioOutput;
extern V8Class v8AudioOutputAttached("AudioOutputAttached",
                                     v8AudioOutput,
                                     &Constructor,
                                     [](v8::Local<v8::FunctionTemplate> tpl)
                                     {
                                         v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                         V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

                                         V8Helpers::SetAccessor(isolate, tpl, "entity", &EntityGetter, &EntitySetter);
                                     });
