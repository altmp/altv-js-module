
#include "../helpers/V8Helpers.h"
#include "../helpers/V8BindHelpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IWebView.h"

using namespace alt;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	auto webview = info.This();
	V8_OBJECT_GET_STRING(webview, "url", url);

	std::ostringstream ss;
	ss << "WebView{ url: " << url.CStr() << " }";

	V8_RETURN_STRING(ss.str().c_str());
}

static void On(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, fun);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	static_cast<CV8ResourceImpl *>(resource)->SubscribeWebView(view, evName.ToString(), fun, V8::SourceLocation::GetCurrent(isolate));
}

static void Once(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, fun);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	static_cast<CV8ResourceImpl *>(resource)->SubscribeWebView(view, evName.ToString(), fun, V8::SourceLocation::GetCurrent(isolate), true);
}

static void Off(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, fun);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	static_cast<CV8ResourceImpl *>(resource)->UnsubscribeWebView(view, evName.ToString(), fun);
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN_MIN(1);
	V8_ARG_TO_STRING(1, evName);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	alt::MValueArgs mvArgs;

	for (int i = 1; i < info.Length(); ++i)
		mvArgs.Push(V8Helpers::V8ToMValue(info[i]));

	view->Trigger(evName, mvArgs);
}

static void GetEventListeners(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_ARGS_LEN(1);
	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_ARG_TO_STRING(1, eventName);

	std::vector<V8::EventCallback *> handlers = static_cast<CV8ResourceImpl*>(resource)->GetWebViewHandlers(view, eventName.ToString());

	auto array = v8::Array::New(isolate, handlers.size());
	for(int i = 0; i < handlers.size(); i++)
	{
		array->Set(ctx, i, handlers[i]->fn.Get(isolate));
	}

	V8_RETURN(array);
}

static void FocusedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE();

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_RETURN_BOOLEAN(view->IsFocused());
}

static void FocusedSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_TO_BOOLEAN(value, shouldBeFocused);

	if(shouldBeFocused) view->Focus();
	else view->Unfocus();
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN_MIN_MAX(1, 4);

	V8_ARG_TO_STRING(1, url);

	alt::IResource* altres = V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext());
	V8_CHECK(altres, "invalid resource");

	alt::Ref<IWebView> view = nullptr;

	if (info.Length() == 4)
	{
		V8_ARG_TO_BOOLEAN(2, isOverlayBool);
		V8_ARG_TO_OBJECT(3, pos);
		V8_ARG_TO_OBJECT(4, size);

		V8_OBJECT_GET_INT(pos, "x", posX);
		V8_OBJECT_GET_INT(pos, "y", posY);

		V8_OBJECT_GET_INT(size, "x", sizeX);
		V8_OBJECT_GET_INT(size, "y", sizeY);

		view = alt::ICore::Instance().CreateWebView(altres, url, { posX, posY }, { sizeX, sizeY }, true, isOverlayBool);
	}
	else if (info.Length() == 3 && info[2]->IsObject())
	{
		V8_ARG_TO_OBJECT(2, pos);
		V8_ARG_TO_OBJECT(3, size);

		V8_OBJECT_GET_INT(pos, "x", posX);
		V8_OBJECT_GET_INT(pos, "y", posY);

		V8_OBJECT_GET_INT(size, "x", sizeX);
		V8_OBJECT_GET_INT(size, "y", sizeY);

		view = alt::ICore::Instance().CreateWebView(altres, url, { posX, posY }, { sizeX, sizeY }, true, false);
	}
	else if (info.Length() == 3)
	{
		V8_ARG_TO_INT(2, drawableHash);
		V8_ARG_TO_STRING(3, targetTextureStr);

		auto texture = alt::ICore::Instance().GetTextureFromDrawable(drawableHash, targetTextureStr);
		V8_CHECK(texture != nullptr, "Texture not found");

		view = alt::ICore::Instance().CreateWebView(altres, url, (uint32_t)drawableHash, targetTextureStr);
		V8_CHECK(!view.IsEmpty(), "Interactive WebView cannot be created");
	}
	else if (info.Length() == 2 && info[1]->IsObject())
	{
		V8_ARG_TO_OBJECT(2, pos);

		V8_OBJECT_GET_INT(pos, "x", posX);
		V8_OBJECT_GET_INT(pos, "y", posY);

		view = alt::ICore::Instance().CreateWebView(altres, url, { posX, posY }, { 0, 0 }, true, false);
	}
	else if (info.Length() == 2)
	{
		V8_ARG_TO_BOOLEAN(2, isOverlayBool);

		view = alt::ICore::Instance().CreateWebView(altres, url, { 0, 0 }, { 0, 0 }, true, isOverlayBool);
	}
	else
	{
		view = alt::ICore::Instance().CreateWebView(altres, url, { 0, 0 }, { 0, 0 }, true, false);
	}

	V8_BIND_BASE_OBJECT(view, "Failed to create WebView");
}

extern V8Class v8BaseObject;
extern V8Class v8WebView("WebView", v8BaseObject, &Constructor,	[](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetMethod(isolate, tpl, "toString", ToString);

	V8::SetAccessor<IWebView, bool, &IWebView::IsVisible, &IWebView::SetVisible>(isolate, tpl, "isVisible");
	V8::SetAccessor<IWebView, StringView, &IWebView::GetUrl, &IWebView::SetUrl>(isolate, tpl, "url");
	V8::SetAccessor<IWebView, bool, &IWebView::IsOverlay>(isolate, tpl, "isOverlay");
	V8::SetAccessor<IWebView, bool, &IWebView::IsReady>(isolate, tpl, "isReady");
	V8::SetAccessor(isolate, tpl, "focused", &FocusedGetter, &FocusedSetter);

	V8::SetMethod(isolate, tpl, "on", &On);
	V8::SetMethod(isolate, tpl, "once", &Once);
	V8::SetMethod(isolate, tpl, "off", &Off);
	V8::SetMethod(isolate, tpl, "getEventListeners", GetEventListeners);
	V8::SetMethod(isolate, tpl, "emit", &Emit);
	V8::SetMethod<IWebView, &IWebView::Focus>(isolate, tpl, "focus");
	V8::SetMethod<IWebView, &IWebView::Unfocus>(isolate, tpl, "unfocus");
});
