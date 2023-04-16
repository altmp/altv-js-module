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

    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);
    auto doc = alt::ICore::Instance().CreateDocument(url, origin, resource->GetResource());
    V8_BIND_BASE_OBJECT(doc, "Failed to create Rml document");
}

static void TitleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(document, alt::IRmlDocument);

    V8_RETURN_STRING(document->GetTitle());
}

static void TitleSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(document, alt::IRmlDocument);

    V8_TO_STRING(val, title);
    document->SetTitle(title);
}

static void VisibleSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(document, alt::IRmlDocument);

    V8_TO_BOOLEAN(val, visible);
    if(visible) document->Show();
}

static void Show(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(document, alt::IRmlDocument);
    V8_CHECK_ARGS_LEN_MIN_MAX(0, 2);

    V8_ARG_TO_BOOLEAN_OPT(1, isModal, false);
    V8_ARG_TO_BOOLEAN_OPT(2, focused, true);
    document->Show(isModal, focused);
}

static void CreateElement(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(document, alt::IRmlDocument);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, tag);

    alt::IRmlElement* element = document->CreateElement(tag);
    V8_CHECK(element, "Failed to create element");

    V8_RETURN(resource->GetBaseObjectOrNull(element));
}

static void CreateTextNode(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(document, alt::IRmlDocument);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, text);

    alt::IRmlElement* node = document->CreateTextNode(text);
    V8_CHECK(node, "Failed to create text node");

    V8_RETURN(resource->GetBaseObjectOrNull(node));
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::RML_DOCUMENT, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::RML_DOCUMENT)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

extern V8Class v8RmlElement;
extern V8Class v8RmlDocument("RmlDocument",
                             v8RmlElement,
                             &DocumentConstructor,
                             [](v8::Local<v8::FunctionTemplate> tpl)
                             {
                                 using namespace alt;
                                 v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                 V8Helpers::SetAccessor<alt::IRmlDocument, uint32_t, &alt::IRmlDocument::GetEntityID>(isolate, tpl, "id");
                                 V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

                                 V8Helpers::SetAccessor<alt::IRmlDocument, const std::string&, &alt::IRmlDocument::GetTitle, &alt::IRmlDocument::SetTitle>(isolate, tpl, "title");
                                 V8Helpers::SetAccessor<alt::IRmlDocument, const std::string&, &alt::IRmlDocument::GetSourceUrl>(isolate, tpl, "sourceUrl");
                                 V8Helpers::SetAccessor<alt::IRmlDocument, bool, &alt::IRmlDocument::IsVisible>(isolate, tpl, "isVisible");
                                 V8Helpers::SetAccessor<alt::IRmlDocument, bool, &alt::IRmlDocument::IsModal>(isolate, tpl, "isModal");

                                 V8Helpers::SetAccessor<alt::IRmlDocument, IRmlElement*, &alt::IRmlDocument::GetBody>(isolate, tpl, "body");

                                 V8Helpers::SetMethod(isolate, tpl, "show", &Show);
                                 V8Helpers::SetMethod<alt::IRmlDocument, &alt::IRmlDocument::Hide>(isolate, tpl, "hide");

                                 V8Helpers::SetMethod<alt::IRmlDocument, &alt::IRmlDocument::Update>(isolate, tpl, "update");

                                 V8Helpers::SetMethod(isolate, tpl, "createElement", &CreateElement);
                                 V8Helpers::SetMethod(isolate, tpl, "createTextNode", &CreateTextNode);
                             });
