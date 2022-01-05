#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IRml.h"

static void DocumentCostructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, url);

    auto audio = alt::ICore::Instance().CreateDocument(std::string(url.CStr()), resource->GetResource());
    V8_BIND_BASE_OBJECT(audio, "Failed to create Rml document");
}

extern V8Class v8BaseObject;
extern V8Class v8RmlDocument("RmlDocument",v8BaseObject, &DocumentCostructor,
[](v8::Local<v8::FunctionTemplate> tpl)
{
    using namespace alt;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    
});
