#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IRml.h"

static void SetProperty(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, name);
    V8_ARG_TO_STRING(2, val);
    V8_RETURN_BOOLEAN(element->SetProperty(name, val));
}

static void RemoveProperty(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->RemoveProperty(name));
}

static void HasProperty(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->HasProperty(name));
}

static void HasLocalProperty(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->HasLocalProperty(name));
}

static void GetProperty(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_STRING(element->GetProperty(name));
}

static void GetLocalProperty(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_STRING(element->GetLocalProperty(name));
}

static void GetPropertyAbsoluteValue(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_NUMBER(element->GetPropertyAbsoluteValue(name));
}

static void SetAttribute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_STRING(1, name);
    V8_ARG_TO_STRING(2, value);
    element->SetAttribute(name, value);
}

static void RemoveAttribute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->RemoveAttribute(name));
}

static void HasAttribute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->HasAttribute(name));
}

static void GetAttribute(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_STRING(element->GetAttribute(name));
}

static void GetAttributes(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_NEW_OBJECT(attributes);

    for(auto attr : element->GetAttributes())
    {
        V8_OBJECT_SET_STRING(attributes, attr.first.c_str(), attr.second);
    }

    V8_RETURN(attributes);
}

static void AppendChild(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, child, alt::IRmlElement, "IRmlElement");

    element->AppendChild(child);
}

static void InsertBefore(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_BASE_OBJECT(1, child, alt::IRmlElement, "IRmlElement");
    V8_ARG_TO_BASE_OBJECT(2, adjacent, alt::IRmlElement, "IRmlElement");

    element->InsertBefore(child, adjacent);
}

static void ReplaceChild(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(2);

    V8_ARG_TO_BASE_OBJECT(1, newElem, alt::IRmlElement, "IRmlElement");
    V8_ARG_TO_BASE_OBJECT(2, oldElem, alt::IRmlElement, "IRmlElement");

    element->ReplaceChild(newElem, oldElem);
}

static void RemoveChild(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, child, alt::IRmlElement, "IRmlElement");

    element->RemoveChild(child);
}

static void AddClass(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->AddClass(name));
}

static void RemoveClass(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->RemoveClass(name));
}

static void HasClass(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->HasClass(name));
}

static void GetClassList(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    const std::vector<std::string> list = element->GetClassList();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

static void AddPseudoClass(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->AddPseudoClass(name));
}

static void RemovePseudoClass(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->RemovePseudoClass(name));
}

static void HasPseudoClass(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, name);
    V8_RETURN_BOOLEAN(element->HasPseudoClass(name));
}

static void GetPseudoClassList(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    const std::vector<std::string> list = element->GetPseudoClassList();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

static void SetOffset(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN2(2, 3);

    V8_ARG_TO_BASE_OBJECT(1, el, alt::IRmlElement, "RmlElement");
    V8_ARG_TO_VECTOR2(2, offset);
    V8_ARG_TO_BOOLEAN_OPT(3, fixed, false);

    element->SetOffset(el, offset, fixed);
}

static void IsPointWithinElement(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_VECTOR2(1, point);

    V8_RETURN_BOOLEAN(element->IsPointWithinElement(point));
}

static void StyleGetterHandler(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_DATA(alt::IRmlElement, element);

    V8_TO_STRING(property, name);
    V8_RETURN_STRING(element->GetProperty(name));
}

static void StyleSetterHandler(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_DATA(alt::IRmlElement, element);

    V8_TO_STRING(property, name);
    V8_TO_STRING(value, val);
    element->SetProperty(name, val);
    V8_RETURN(value);
}

static void StyleDeleterHandler(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Boolean>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_DATA(alt::IRmlElement, element);

    V8_TO_STRING(property, name);
    V8_RETURN_BOOLEAN(element->RemoveProperty(name));
}
static void StyleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_RETURN(V8Helpers::CreateCustomObject(isolate, element, StyleGetterHandler, StyleSetterHandler, StyleDeleterHandler));
}

static void ChildrenGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    size_t size = element->GetChildCount();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, resource->GetBaseObjectOrNull(element->GetChild(i)));
    }

    V8_RETURN(arr);
}

static void GetClosest(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, selectors);

    V8_RETURN_BASE_OBJECT(element->GetClosest(selectors));
}

static void GetElementByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, id);

    V8_RETURN_BASE_OBJECT(element->GetElementByID(id));
}

static void GetElementsByTagName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, tag);

    const std::vector<alt::IRmlElement*> elements = element->GetElementsByTagName(tag);
    size_t size = elements.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, resource->GetBaseObjectOrNull(elements[i]));
    }

    V8_RETURN(arr);
}

static void GetElementsByClassName(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, className);

    const std::vector<alt::IRmlElement*> elements = element->GetElementsByClassName(className);
    size_t size = elements.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, resource->GetBaseObjectOrNull(elements[i]));
    }

    V8_RETURN(arr);
}

static void QuerySelector(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, selector);

    V8_RETURN_BASE_OBJECT(element->QuerySelector(selector));
}

static void QuerySelectorAll(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, selector);

    const std::vector<alt::IRmlElement*> elements = element->QuerySelectorAll(selector);
    size_t size = elements.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, resource->GetBaseObjectOrNull(elements[i]));
    }

    V8_RETURN(arr);
}

static void Focus(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_RETURN(element->Focus());
}

static void ScrollIntoView(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_ARG_TO_BOOLEAN_OPT(1, alignToTop, true);

    element->ScrollIntoView(alignToTop);
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    static_cast<CV8ResourceImpl*>(resource)->SubscribeRml(element, evName, fun, V8Helpers::SourceLocation::GetCurrent(isolate));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    static_cast<CV8ResourceImpl*>(resource)->UnsubscribeRml(element, evName, fun);
}

static void GetEventListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(element, alt::IRmlElement);

    V8_ARG_TO_STRING(1, eventName);

    std::vector<V8Helpers::EventCallback*> handlers = static_cast<CV8ResourceImpl*>(resource)->GetRmlHandlers(element, eventName);

    auto array = v8::Array::New(isolate, handlers.size());
    for(int i = 0; i < handlers.size(); i++)
    {
        array->Set(ctx, i, handlers[i]->fn.Get(isolate));
    }

    V8_RETURN(array);
}

extern V8Class v8BaseObject;
extern V8Class v8RmlElement("RmlElement",
                            v8BaseObject,
                            nullptr,
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                using namespace alt;
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                V8Helpers::SetMethod(isolate, tpl, "on", &On);
                                V8Helpers::SetMethod(isolate, tpl, "off", &Off);
                                V8Helpers::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);

                                V8Helpers::SetAccessor<alt::IRmlElement, Vector2f, &alt::IRmlElement::GetRelativeOffset>(isolate, tpl, "relativeOffset");
                                V8Helpers::SetAccessor<alt::IRmlElement, Vector2f, &alt::IRmlElement::GetAbsoluteOffset>(isolate, tpl, "absoluteOffset");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetBaseline>(isolate, tpl, "baseline");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetZIndex>(isolate, tpl, "zIndex");
                                V8Helpers::SetAccessor<alt::IRmlElement, Vector2f, &alt::IRmlElement::GetContainingBlock>(isolate, tpl, "containingBlock");

                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlElement*, &alt::IRmlElement::GetFocusedElement>(isolate, tpl, "focusedElement");

                                V8Helpers::SetAccessor<alt::IRmlElement, const std::string&, &alt::IRmlElement::GetTagName>(isolate, tpl, "tagName");
                                V8Helpers::SetAccessor<alt::IRmlElement, const std::string&, &alt::IRmlElement::GetID, &alt::IRmlElement::SetID>(isolate, tpl, "id");

                                V8Helpers::SetAccessor<alt::IRmlElement, bool, &alt::IRmlElement::IsOwned>(isolate, tpl, "isOwned");

                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetAbsoluteLeft>(isolate, tpl, "absoluteLeft");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetAbsoluteTop>(isolate, tpl, "absoluteTop");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetClientLeft>(isolate, tpl, "clientLeft");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetClientTop>(isolate, tpl, "clientTop");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetClientWidth>(isolate, tpl, "clientWidth");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetClientHeight>(isolate, tpl, "clientHeight");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetOffsetLeft>(isolate, tpl, "offsetLeft");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetOffsetTop>(isolate, tpl, "offsetTop");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetOffsetWidth>(isolate, tpl, "offsetWidth");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetOffsetHeight>(isolate, tpl, "offsetHeight");

                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetScrollLeft, &alt::IRmlElement::SetScrollLeft>(isolate, tpl, "scrollLeft");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetScrollTop, &alt::IRmlElement::SetScrollTop>(isolate, tpl, "scrollTop");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetScrollWidth>(isolate, tpl, "scrollWidth");
                                V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetScrollHeight>(isolate, tpl, "scrollHeight");

                                V8Helpers::SetAccessor<alt::IRmlElement, bool, &alt::IRmlElement::IsVisible>(isolate, tpl, "isVisible");

                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlElement*, &alt::IRmlElement::GetParent>(isolate, tpl, "parent");
                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlElement*, &alt::IRmlElement::GetNextSibling>(isolate, tpl, "nextSibling");
                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlElement*, &alt::IRmlElement::GetPreviousSibling>(isolate, tpl, "previousSibling");
                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlElement*, &alt::IRmlElement::GetFirstChild>(isolate, tpl, "firstChild");
                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlElement*, &alt::IRmlElement::GetLastChild>(isolate, tpl, "lastChild");

                                V8Helpers::SetAccessor<alt::IRmlElement, int, &alt::IRmlElement::GetChildCount>(isolate, tpl, "childCount");
                                V8Helpers::SetAccessor<alt::IRmlElement, bool, &alt::IRmlElement::HasChildren>(isolate, tpl, "hasChildren");

                                V8Helpers::SetAccessor<alt::IRmlElement, std::string, &alt::IRmlElement::GetInnerRML, const std::string&, &alt::IRmlElement::SetInnerRML>(
                                  isolate, tpl, "innerRML");

                                V8Helpers::SetAccessor<alt::IRmlElement, IRmlDocument*, &alt::IRmlElement::GetOwnerDocument>(isolate, tpl, "ownerDocument");

                                V8Helpers::SetAccessor(isolate, tpl, "childNodes", &ChildrenGetter);

                                V8Helpers::SetMethod(isolate, tpl, "appendChild", &AppendChild);
                                V8Helpers::SetMethod(isolate, tpl, "insertBefore", &InsertBefore);
                                V8Helpers::SetMethod(isolate, tpl, "replaceChild", &ReplaceChild);
                                V8Helpers::SetMethod(isolate, tpl, "removeChild", &RemoveChild);

                                V8Helpers::SetMethod(isolate, tpl, "addClass", &AddClass);
                                V8Helpers::SetMethod(isolate, tpl, "removeClass", &RemoveClass);
                                V8Helpers::SetMethod(isolate, tpl, "hasClass", &HasClass);
                                V8Helpers::SetMethod(isolate, tpl, "getClassList", &GetClassList);

                                V8Helpers::SetMethod(isolate, tpl, "addPseudoClass", &AddPseudoClass);
                                V8Helpers::SetMethod(isolate, tpl, "removePseudoClass", &RemovePseudoClass);
                                V8Helpers::SetMethod(isolate, tpl, "hasPseudoClass", &HasPseudoClass);
                                V8Helpers::SetMethod(isolate, tpl, "getPseudoClassList", &GetPseudoClassList);

                                V8Helpers::SetMethod(isolate, tpl, "setOffset", &SetOffset);

                                V8Helpers::SetMethod(isolate, tpl, "isPointWithinElement", &IsPointWithinElement);

                                V8Helpers::SetAccessor(isolate, tpl, "style", &StyleGetter);
                                V8Helpers::SetMethod(isolate, tpl, "setProperty", &SetProperty);
                                V8Helpers::SetMethod(isolate, tpl, "removeProperty", &RemoveProperty);
                                V8Helpers::SetMethod(isolate, tpl, "hasProperty", &HasProperty);
                                V8Helpers::SetMethod(isolate, tpl, "hasLocalProperty", &HasLocalProperty);
                                V8Helpers::SetMethod(isolate, tpl, "getProperty", &GetProperty);
                                V8Helpers::SetMethod(isolate, tpl, "getLocalProperty", &GetLocalProperty);
                                V8Helpers::SetMethod(isolate, tpl, "getPropertyAbsoluteValue", &GetPropertyAbsoluteValue);

                                V8Helpers::SetMethod(isolate, tpl, "setAttribute", &SetAttribute);
                                V8Helpers::SetMethod(isolate, tpl, "removeAttribute", &RemoveAttribute);
                                V8Helpers::SetMethod(isolate, tpl, "hasAttribute", &HasAttribute);
                                V8Helpers::SetMethod(isolate, tpl, "getAttribute", &GetAttribute);
                                V8Helpers::SetMethod(isolate, tpl, "getAttributes", &GetAttributes);

                                V8Helpers::SetMethod(isolate, tpl, "closest", &GetClosest);

                                V8Helpers::SetMethod(isolate, tpl, "getElementByID", &GetElementByID);
                                V8Helpers::SetMethod(isolate, tpl, "getElementsByTagName", &GetElementsByTagName);
                                V8Helpers::SetMethod(isolate, tpl, "getElementsByClassName", &GetElementsByClassName);
                                V8Helpers::SetMethod(isolate, tpl, "querySelector", &QuerySelector);
                                V8Helpers::SetMethod(isolate, tpl, "querySelectorAll", &QuerySelectorAll);

                                V8Helpers::SetMethod(isolate, tpl, "focus", &Focus);
                                V8Helpers::SetMethod<alt::IRmlElement, &alt::IRmlElement::Blur>(isolate, tpl, "blur");
                                V8Helpers::SetMethod<alt::IRmlElement, &alt::IRmlElement::Click>(isolate, tpl, "click");
                                V8Helpers::SetMethod(isolate, tpl, "scrollIntoView", &ScrollIntoView);
                            });
