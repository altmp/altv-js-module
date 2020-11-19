
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IWebView.h"

using namespace alt;

static void On(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK_ARGS_LEN(2);
	V8_ARG_TO_STRING(1, evName);
	V8_ARG_TO_FUNCTION(2, fun);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	static_cast<CV8ResourceImpl *>(resource)->SubscribeWebView(view, evName.ToString(), fun, V8::SourceLocation::GetCurrent(isolate));
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

static void Focus(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	view->Focus();
}

static void Unfocus(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	view->Unfocus();
}

static void SetURL(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_ARGS_LEN(1);
	V8_ARG_TO_STRING(1, url);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	view->SetUrl(url);
}

static void IsVisibleGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_RETURN_BOOLEAN(view->IsVisible());
}

static void IsVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_TO_BOOLEAN(value, state);
	view->SetVisible(state);
}

static void URLGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	V8_GET_ISOLATE(info);

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_RETURN_STRING(view->GetUrl().CStr());
}

static void URLSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_GET_THIS_BASE_OBJECT(view, alt::IWebView);

	V8_TO_STRING(value, url);

	view->SetUrl(url);
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN_MIN_MAX(1, 3);

	V8_ARG_TO_STRING(1, url);

	alt::IResource* altres = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
	V8_CHECK(altres, "invalid resource");

	alt::Ref<IWebView> view = nullptr;

	if (info.Length() == 3)
	{
		V8_ARG_TO_INTEGER(2, drawableHash);
		V8_ARG_TO_STRING(3, targetTextureStr);

		auto texture = alt::ICore::Instance().GetTextureFromDrawable(drawableHash, targetTextureStr);
		V8_CHECK(texture != nullptr, "Texture not found");

		view = altres->CreateWebView(url, (uint32_t)drawableHash, targetTextureStr);
		V8_CHECK(!view.IsEmpty(), "Interactive WebView cannot be created");
	}
	else if (info.Length() == 2)
	{
		V8_ARG_TO_BOOLEAN(2, isOverlayBool);

		view = altres->CreateWebView(url, { 0, 0 }, { 0, 0 }, true, isOverlayBool);
	}
	else
	{
		view = altres->CreateWebView(url, { 0, 0 }, { 0, 0 }, true, false);
	}

	V8_BIND_BASE_OBJECT(view);
}

extern V8Class v8BaseObject;
extern V8Class v8WebView("WebView", v8BaseObject, &Constructor,	[](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

	V8::SetAccessor(isolate, tpl, "isVisible", &IsVisibleGetter, &IsVisibleSetter);
	V8::SetAccessor(isolate, tpl, "url", &URLGetter, &URLSetter);

	V8::SetMethod(isolate, tpl, "on", &On);
	V8::SetMethod(isolate, tpl, "off", &Off);
	V8::SetMethod(isolate, tpl, "emit", &Emit);
	V8::SetMethod(isolate, tpl, "focus", &Focus);
	V8::SetMethod(isolate, tpl, "unfocus", &Unfocus);
});
