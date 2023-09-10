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
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(0, 1);

    V8_ARG_TO_INT_OPT(1, categoryHash, alt::ICore::Instance().Hash("radio"));

    auto output = alt::ICore::Instance().CreateFrontendOutput(categoryHash, resource->GetResource());
    V8_BIND_BASE_OBJECT(output, "Failed to create AudioOutputFrontend");
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::AUDIO_OUTPUT_FRONTEND, id);

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
extern V8Class v8AudioOutputFrontend("AudioOutputFrontend",
                                     v8AudioOutput,
                                     &Constructor,
                                     [](v8::Local<v8::FunctionTemplate> tpl)
                                     {
                                         v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                         V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                                     });
