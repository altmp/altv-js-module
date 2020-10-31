
#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void ToString(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	v8::Local<v8::Number> r = info.This()->Get(ctx, V8::RGBA_RKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> g = info.This()->Get(ctx, V8::RGBA_GKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> b = info.This()->Get(ctx, V8::RGBA_BKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> a = info.This()->Get(ctx, V8::RGBA_AKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();

	std::ostringstream ss;
	ss << "RGBA{ r: " << r->Value() << ", g: " << r->Value() << ", b: " << b->Value() << ", a: " << a->Value() << " }";

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, ss.str().c_str(), v8::NewStringType::kNormal).ToLocalChecked());
}

static V8Class v8RGBA(
	"RGBA", "", [](const v8::FunctionCallbackInfo<v8::Value> &info) {
	v8::Isolate* isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 4, "4 args expected");

	v8::Local<v8::Object> _this = info.This();

	v8::Local<v8::Number> r = info[0]->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> g = info[1]->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> b = info[2]->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> a = info[3]->ToNumber(ctx).ToLocalChecked();

	V8::DefineOwnProperty(isolate, ctx, _this, V8::RGBA_RKey(isolate), r, v8::PropertyAttribute::ReadOnly);
	V8::DefineOwnProperty(isolate, ctx, _this, V8::RGBA_GKey(isolate), g, v8::PropertyAttribute::ReadOnly);
	V8::DefineOwnProperty(isolate, ctx, _this, V8::RGBA_BKey(isolate), b, v8::PropertyAttribute::ReadOnly);
	V8::DefineOwnProperty(isolate, ctx, _this, V8::RGBA_AKey(isolate), a, v8::PropertyAttribute::ReadOnly); }, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetMethod(isolate, tpl, "toString", ToString); }, false);
