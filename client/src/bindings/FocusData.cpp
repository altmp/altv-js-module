#include "cpp-sdk/ICore.h"
#include "V8Class.h"
#include "V8Helpers.h"
#include "V8ResourceImpl.h"

static void IsFocusOverriden(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsFocusOverriden());
}

static void GetFocusOverridePos(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_RETURN_VECTOR3(alt::ICore::Instance().GetFocusOverridePos());
}

static void GetFocusOverrideOffset(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_RETURN_VECTOR3(alt::ICore::Instance().GetFocusOverrideOffset());
}

static void GetFocusOverrideEntity(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetFocusOverrideEntity());
}

static void OverrideFocus(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(1, 2);

    if(resource->IsVector3(info[0]))
    {
        V8_ARG_TO_VECTOR3(1, pos);
        alt::Vector3f offset = { 0, 0, 0 };
        if(info.Length() == 2)
        {
            V8_ARG_TO_VECTOR3(2, focusOffset);
            offset = focusOffset;
        }
        alt::ICore::Instance().OverrideFocusPosition(pos, offset);
    }
    else if(resource->IsBaseObject(info[0]))
    {
        V8_ARG_TO_BASE_OBJECT(1, entity, alt::IEntity, "Entity");
        alt::ICore::Instance().OverrideFocusEntity(entity);
    }
    else
        V8_CHECK(false, "Invalid arg type, only Vector3 and Entity allowed")
}

static void ClearFocus(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    alt::ICore::Instance().ClearFocusOverride();
}

extern V8Class v8FocusData("FocusData", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "isFocusOverriden", &IsFocusOverriden);
    V8Helpers::SetStaticAccessor(isolate, tpl, "focusOverridePos", &GetFocusOverridePos);
    V8Helpers::SetStaticAccessor(isolate, tpl, "focusOverrideOffset", &GetFocusOverrideOffset);
    V8Helpers::SetStaticAccessor(isolate, tpl, "focusOverrideEntity", &GetFocusOverrideEntity);

    V8Helpers::SetStaticMethod(isolate, tpl, "overrideFocus", &OverrideFocus);
    V8Helpers::SetStaticMethod(isolate, tpl, "clearFocus", &ClearFocus);
});
