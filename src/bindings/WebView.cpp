
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "cpp-sdk/script-objects/IWebView.h"

using namespace alt;

static void On(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");
	V8_CHECK(info[1]->IsFunction(), "callback must be a function");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();
	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	static_cast<CV8ResourceImpl *>(resource)->SubscribeWebView(view, evName, info[1].As<v8::Function>(), V8::SourceLocation::GetCurrent(isolate));
}

static void Off(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 2, "2 args expected");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");
	V8_CHECK(info[1]->IsFunction(), "callback must be a function");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();
	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	static_cast<CV8ResourceImpl *>(resource)->UnsubscribeWebView(view, evName, info[1].As<v8::Function>());
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() >= 1, "WebView.emit expects atleast 1 arg");
	V8_CHECK(info[0]->IsString(), "eventName must be a string");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "Invalid resource");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();
	std::string evName = *v8::String::Utf8Value(info.GetIsolate(), info[0].As<v8::String>());

	alt::MValueArgs mvArgs;

	for (int i = 1; i < info.Length(); ++i)
		mvArgs.Push(V8Helpers::V8ToMValue(info[i]));

	view->Trigger(evName, mvArgs);
}

static void Focus(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	view->Focus();
}

static void Unfocus(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	view->Unfocus();
}

static void SetURL(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "string expected");

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	v8::String::Utf8Value url(info.GetIsolate(), info[0].As<v8::String>());

	view->SetUrl(*url);
}

static void IsVisibleGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	info.GetReturnValue().Set(v8::Boolean::New(isolate, view->IsVisible()));
}

static void IsVisibleSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	view->SetVisible(value->BooleanValue(isolate));
}

static void URLGetter(v8::Local<v8::String> property, const v8::PropertyCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, view->GetUrl().CStr()).ToLocalChecked());
}

static void URLSetter(v8::Local<v8::String> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

	V8Entity *_this = V8Entity::Get(info.This());
	V8_CHECK(_this, "entity is invalid");

	alt::Ref<IWebView> view = _this->GetHandle().As<IWebView>();

	v8::String::Utf8Value url{isolate, value->ToString(ctx).ToLocalChecked()};
	view->SetUrl(*url);
}

extern V8Class v8BaseObject;
extern V8Class v8WebView(
	"WebView", v8BaseObject,
	[](const v8::FunctionCallbackInfo<v8::Value> &info) {
		v8::Isolate *isolate = info.GetIsolate();
		v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

		V8_CHECK(info.IsConstructCall(), "WebView is not a function");
		V8_CHECK(info.Length() > 0 && info.Length() <= 3, "new WebView(...) expects 1, 2 or 3 args");
		V8_CHECK(info[0]->IsString(), "url must be a string");

		alt::IResource *resource = V8ResourceImpl::GetResource(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		v8::Local<v8::String> url = info[0].As<v8::String>();

		alt::Ref<IWebView> view = nullptr;

		if (info.Length() == 3)
		{
			v8::Local<v8::Integer> modelHash = info[1]->ToInteger(ctx).ToLocalChecked();
			v8::Local<v8::String> targetTexture = info[2]->ToString(ctx).ToLocalChecked();

			uint32_t drawableHash = modelHash->Value();
			std::string targetTextureStr = *v8::String::Utf8Value(info.GetIsolate(), targetTexture);

			auto texture = alt::ICore::Instance().GetTextureFromDrawable(drawableHash, targetTextureStr);
			V8_CHECK(texture != nullptr, "Texture not found");

			view = resource->CreateWebView(*v8::String::Utf8Value(info.GetIsolate(), url), (uint32_t)drawableHash, targetTextureStr);
			V8_CHECK(!view.IsEmpty(), "Interactive WebView cannot be created");
		}
		else if (info.Length() == 2)
		{
			v8::Local<v8::Boolean> isOverlay = info[1]->ToBoolean(isolate);
			bool isOverlayBool = isOverlay->Value();

			view = resource->CreateWebView(*v8::String::Utf8Value(info.GetIsolate(), url), {0, 0}, {0, 0}, true, isOverlayBool);
		}
		else
		{
			view = resource->CreateWebView(*v8::String::Utf8Value(info.GetIsolate(), url), {0, 0}, {0, 0}, true, false);
		}

		// static_cast<CV8ResourceImpl *>(resource)->AddOwned(view);
		// resource->BindEntity(info.This(), view.Get());
	},
	[](v8::Local<v8::FunctionTemplate> tpl) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();

		V8::SetAccessor(isolate, tpl, "isVisible", &IsVisibleGetter, &IsVisibleSetter);
		V8::SetAccessor(isolate, tpl, "url", &URLGetter, &URLSetter);

		v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

		proto->Set(isolate, "on", v8::FunctionTemplate::New(isolate, &On));
		proto->Set(isolate, "off", v8::FunctionTemplate::New(isolate, &Off));
		proto->Set(isolate, "emit", v8::FunctionTemplate::New(isolate, &Emit));
		proto->Set(isolate, "focus", v8::FunctionTemplate::New(isolate, &Focus));
		proto->Set(isolate, "unfocus", v8::FunctionTemplate::New(isolate, &Unfocus));
	});
