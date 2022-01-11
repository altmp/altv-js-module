#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8ResourceImpl.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "cpp-sdk/script-objects/IRml.h"

extern V8Class v8BaseObject;
extern V8Class v8RmlElement("RmlElement", v8BaseObject, nullptr, [](v8::Local<v8::FunctionTemplate> tpl)
{
    using namespace alt;
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    //V8Helpers::SetAccessor(isolate, tpl, "sourceUrl", &SourceUrlGetter);

    V8Helpers::SetAccessor<alt::IRmlElement, Vector2f, &alt::IRmlElement::GetRelativeOffset>(isolate, tpl, "relativeOffset");
    V8Helpers::SetAccessor<alt::IRmlElement, Vector2f, &alt::IRmlElement::GetAbsoluteOffset>(isolate, tpl, "absoluteOffset");
    V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetBaseline>(isolate, tpl, "baseline");
    V8Helpers::SetAccessor<alt::IRmlElement, float, &alt::IRmlElement::GetZIndex>(isolate, tpl, "zIndex");
    V8Helpers::SetAccessor<alt::IRmlElement, Vector2f, &alt::IRmlElement::GetContainingBlock>(isolate, tpl, "containingBlock");

    V8Helpers::SetAccessor<alt::IRmlElement, Ref<IRmlElement>, &alt::IRmlElement::GetFocusedElement>(isolate, tpl, "focusedElement");

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

    V8Helpers::SetAccessor<alt::IRmlElement, Ref<IRmlElement>, &alt::IRmlElement::GetParent>(isolate, tpl, "parent");
    V8Helpers::SetAccessor<alt::IRmlElement, Ref<IRmlElement>, &alt::IRmlElement::GetNextSibling>(isolate, tpl, "nextSibling");
    V8Helpers::SetAccessor<alt::IRmlElement, Ref<IRmlElement>, &alt::IRmlElement::GetPreviousSibling>(isolate, tpl, "previousSibling");
    V8Helpers::SetAccessor<alt::IRmlElement, Ref<IRmlElement>, &alt::IRmlElement::GetFirstChild>(isolate, tpl, "firstChild");
    V8Helpers::SetAccessor<alt::IRmlElement, Ref<IRmlElement>, &alt::IRmlElement::GetLastChild>(isolate, tpl, "lastChild");

    V8Helpers::SetAccessor<alt::IRmlElement, int, &alt::IRmlElement::GetChildCount>(isolate, tpl, "childCount");
    V8Helpers::SetAccessor<alt::IRmlElement, bool, &alt::IRmlElement::HasChildren>(isolate, tpl, "hasChildren");

    V8Helpers::SetAccessor<alt::IRmlElement, const std::string&, &alt::IRmlElement::GetInnerRML, &alt::IRmlElement::SetInnerRML>(isolate, tpl, "innerRML");
    /*
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

    V8Helpers::SetMethod(isolate, tpl, "setProperty", &SetProperty);
    V8Helpers::SetMethod(isolate, tpl, "removeProperty", &RemoveProperty);
    V8Helpers::SetMethod(isolate, tpl, "hasProperty", &HasProperty);
    V8Helpers::SetMethod(isolate, tpl, "hasLocalProperty", &HasLocalProperty);
    V8Helpers::SetMethod(isolate, tpl, "getProperty", &GetProperty);
    V8Helpers::SetMethod(isolate, tpl, "getLocalProperty", &GetLocalProperty);
    V8Helpers::SetMethod(isolate, tpl, "getPropertyAbsoluteValue", &GetPropertyAbsoluteValue);

    V8Helpers::SetMethod(isolate, tpl, "getClosest", &GetClosest);
    V8Helpers::SetMethod(isolate, tpl, "getChild", &GetChild);

    V8Helpers::SetMethod(isolate, tpl, "getElementByID", &GetElementByID);
    V8Helpers::SetMethod(isolate, tpl, "getElementsByTagName", &GetElementsByTagName);
    V8Helpers::SetMethod(isolate, tpl, "getElementsByClassName", &GetElementsByClassName);
    V8Helpers::SetMethod(isolate, tpl, "querySelector", &QuerySelector);
    V8Helpers::SetMethod(isolate, tpl, "querySelectorAll", &QuerySelectorAll);

    V8Helpers::SetMethod(isolate, tpl, "focus", &Focus);
    V8Helpers::SetMethod<alt::IRmlElement, &alt::IRmlElement::Blur>(isolate, tpl, "blur");
    V8Helpers::SetMethod<alt::IRmlElement, &alt::IRmlElement::Click>(isolate, tpl, "click");
    V8Helpers::SetMethod(isolate, tpl, "scrollIntoView", &ScrollIntoView);
    */
});
