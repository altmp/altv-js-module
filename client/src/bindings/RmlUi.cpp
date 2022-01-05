#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IRml.h"

static void DocumentConstructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, url);

    auto doc = alt::ICore::Instance().CreateDocument(url.ToString(), resource->GetResource());
    V8_BIND_BASE_OBJECT(doc, "Failed to create Rml document");
}

extern V8Class v8BaseObject;
extern V8Class v8RmlDocument("RmlDocument",v8BaseObject, &DocumentConstructor,
[](v8::Local<v8::FunctionTemplate> tpl)
{
    using namespace alt;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    
});
