
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IWebView.h"

using namespace alt;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    auto webview = info.This();
    V8_OBJECT_GET_STRING(webview, "url", url);

    std::ostringstream ss;
    ss << "WebView{ url: " << url << " }";

    V8_RETURN_STRING(ss.str());
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    static_cast<CV8ResourceImpl*>(resource)->SubscribeWebView(view, evName, fun, V8Helpers::SourceLocation::GetCurrent(isolate, resource));
}

static void Once(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    static_cast<CV8ResourceImpl*>(resource)->SubscribeWebView(view, evName, fun, V8Helpers::SourceLocation::GetCurrent(isolate, resource), true);
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, evName);
    V8_ARG_TO_FUNCTION(2, fun);

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    static_cast<CV8ResourceImpl*>(resource)->UnsubscribeWebView(view, evName, fun);
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN_MIN(1);
    V8_ARG_TO_STRING(1, evName);

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    alt::MValueArgs mvArgs;

    for(int i = 1; i < info.Length(); ++i) mvArgs.emplace_back(V8Helpers::V8ToMValue(info[i], false));

    view->Trigger(evName, mvArgs);
}

static void GetEventListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    V8_ARG_TO_STRING(1, eventName);

    std::vector<V8Helpers::EventCallback*> handlers = static_cast<CV8ResourceImpl*>(resource)->GetWebViewHandlers(view, eventName);

    auto array = v8::Array::New(isolate, handlers.size());
    for(int i = 0; i < handlers.size(); i++)
    {
        array->Set(ctx, i, handlers[i]->fn.Get(isolate));
    }

    V8_RETURN(array);
}

static void FocusedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    V8_RETURN_BOOLEAN(view->IsFocused());
}

static void FocusedSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    V8_TO_BOOLEAN(value, shouldBeFocused);

    if(shouldBeFocused) view->Focus();
    else
        view->Unfocus();
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(1, 4);

    V8_ARG_TO_STRING(1, url);

    alt::IResource* altres = V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext());
    V8_CHECK(altres, "invalid resource");

    alt::IWebView* view = nullptr;

    if(info.Length() == 4)
    {
        V8_ARG_TO_BOOLEAN(2, isOverlayBool);
        V8_ARG_TO_OBJECT(3, pos);
        V8_ARG_TO_OBJECT(4, size);

        V8_OBJECT_GET_INT(pos, "x", posX);
        V8_OBJECT_GET_INT(pos, "y", posY);

        V8_OBJECT_GET_INT(size, "x", sizeX);
        V8_OBJECT_GET_INT(size, "y", sizeY);

        view = alt::ICore::Instance().CreateWebView(url, { posX, posY }, { sizeX, sizeY }, true, isOverlayBool, altres);
    }
    else if(info.Length() == 3 && info[2]->IsObject())
    {
        V8_ARG_TO_OBJECT(2, pos);
        V8_ARG_TO_OBJECT(3, size);

        V8_OBJECT_GET_INT(pos, "x", posX);
        V8_OBJECT_GET_INT(pos, "y", posY);

        V8_OBJECT_GET_INT(size, "x", sizeX);
        V8_OBJECT_GET_INT(size, "y", sizeY);

        view = alt::ICore::Instance().CreateWebView(url, { posX, posY }, { sizeX, sizeY }, true, false, altres);
    }
    else if(info.Length() == 3)
    {
        V8_ARG_TO_INT(2, drawableHash);
        V8_ARG_TO_STRING(3, targetTextureStr);

        auto texture = alt::ICore::Instance().GetTextureFromDrawable(drawableHash, targetTextureStr);
        V8_CHECK(texture != nullptr, "Texture not found");

        view = alt::ICore::Instance().CreateWebView(url, (uint32_t)drawableHash, targetTextureStr, altres);
        V8_CHECK(view, "Interactive WebView cannot be created");
    }
    else if(info.Length() == 2 && info[1]->IsObject())
    {
        V8_ARG_TO_OBJECT(2, pos);

        V8_OBJECT_GET_INT(pos, "x", posX);
        V8_OBJECT_GET_INT(pos, "y", posY);

        view = alt::ICore::Instance().CreateWebView(url, { posX, posY }, { 0, 0 }, true, false, altres);
    }
    else if(info.Length() == 2)
    {
        V8_ARG_TO_BOOLEAN(2, isOverlayBool);

        view = alt::ICore::Instance().CreateWebView(url, { 0, 0 }, { 0, 0 }, true, isOverlayBool, altres);
    }
    else
    {
        view = alt::ICore::Instance().CreateWebView(url, { 0, 0 }, { 0, 0 }, true, false, altres);
    }

    V8_BIND_BASE_OBJECT(view, "Failed to create WebView");
}

static void SetExtraHeader(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, name);
    V8_ARG_TO_STRING(2, value);

    view->SetExtraHeader(name, value);
}

static void SetZoomLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_NUMBER(1, zoomLevel);

    view->SetZoomLevel(zoomLevel);
}

static void Reload(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);
    V8_CHECK_ARGS_LEN2(0, 1);

    bool ignoreCache = false;
    if(info.Length() > 0)
    {
        V8_ARG_TO_BOOLEAN(1, _ignoreCache);
        ignoreCache = _ignoreCache;
    }

    view->Reload(ignoreCache);
}

static void AllWebviewGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto objects = alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::WEBVIEW);
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, objects.size());
    for(size_t i = 0; i < objects.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(objects[i]));
    V8_RETURN(jsArr);
}

static void WebviewCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(alt::ICore::Instance().GetBaseObjects(alt::IBaseObject::Type::WEBVIEW).size());
}

static void WebviewGpuAccelerationActive(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_BOOLEAN(alt::ICore::Instance().IsWebViewGpuAccelerationActive());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::WEBVIEW, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::WEBVIEW)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
}

static void AddOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IWebView);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, entity, alt::IAudioOutput, "AudioOutput");
    audio->AddOutput(entity);
}

static void RemoveOutput(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IWebView);
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_BASE_OBJECT(1, entity, alt::IAudioOutput, "AudioOutput");
    audio->RemoveOutput(entity);
}

static void GetOutputs(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_BASE_OBJECT(audio, alt::IWebView);

    auto list = audio->GetOutputs();
    auto arr = v8::Array::New(isolate, list->GetSize());
    for (int i = 0; i < list->GetSize(); i++)
    {
        auto val = list->Get(i);
        if (val->GetType() == alt::IMValue::Type::BASE_OBJECT)
        {
            auto baseObj = resource->GetBaseObjectOrNull(std::dynamic_pointer_cast<alt::IMValueBaseObject>(val)->RawValue());
            arr->Set(ctx, i, baseObj);
        }
        else if (val->GetType() == alt::IMValue::Type::UINT)
            arr->Set(ctx, i, v8::Integer::NewFromUnsigned(isolate, std::dynamic_pointer_cast<alt::IMValueUInt>(val)->Value()));
    }

    V8_RETURN(arr);
}

extern V8Class v8BaseObject;
extern V8Class v8WebView("WebView",
                         v8BaseObject,
                         &Constructor,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                             v8::Isolate* isolate = v8::Isolate::GetCurrent();

                             V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllWebviewGetter);
                             V8Helpers::SetStaticAccessor(isolate, tpl, "count", &WebviewCountGetter);
                             V8Helpers::SetStaticAccessor(isolate, tpl, "gpuAccelerationActive", &WebviewGpuAccelerationActive);

                             V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
                             V8Helpers::SetMethod(isolate, tpl, "toString", ToString);

                             V8Helpers::SetAccessor<IWebView, bool, &IWebView::IsVisible, &IWebView::SetVisible>(isolate, tpl, "isVisible");
                             V8Helpers::SetAccessor<IWebView, const std::string&, &IWebView::GetUrl, &IWebView::SetUrl>(isolate, tpl, "url");
                             V8Helpers::SetAccessor<IWebView, bool, &IWebView::IsOverlay>(isolate, tpl, "isOverlay");
                             V8Helpers::SetAccessor<IWebView, bool, &IWebView::IsReady>(isolate, tpl, "isReady");
                             V8Helpers::SetAccessor(isolate, tpl, "focused", &FocusedGetter, &FocusedSetter);
                             V8Helpers::SetAccessor<IWebView, Vector2i, &IWebView::GetSize, &IWebView::SetSize>(isolate, tpl, "size");
                             V8Helpers::SetAccessor<IWebView, Vector2i, &IWebView::GetPosition>(isolate, tpl, "pos");

                             V8Helpers::SetMethod(isolate, tpl, "on", &On);
                             V8Helpers::SetMethod(isolate, tpl, "once", &Once);
                             V8Helpers::SetMethod(isolate, tpl, "off", &Off);
                             V8Helpers::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);
                             V8Helpers::SetMethod(isolate, tpl, "emit", &Emit);
                             V8Helpers::SetMethod<IWebView, &IWebView::Focus>(isolate, tpl, "focus");
                             V8Helpers::SetMethod<IWebView, &IWebView::Unfocus>(isolate, tpl, "unfocus");

                             V8Helpers::SetMethod(isolate, tpl, "setExtraHeader", &SetExtraHeader);
                             V8Helpers::SetMethod(isolate, tpl, "setZoomLevel", &SetZoomLevel);
                             V8Helpers::SetMethod(isolate, tpl, "reload", &Reload);

                             V8Helpers::SetMethod(isolate, tpl, "addOutput", &AddOutput);
                             V8Helpers::SetMethod(isolate, tpl, "removeOutput", &RemoveOutput);
                             V8Helpers::SetMethod(isolate, tpl, "getOutputs", &GetOutputs);
                         });
