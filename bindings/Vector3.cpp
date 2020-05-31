#include "stdafx.h"

#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetCurrentContext();

	v8::Local<v8::Number> x = info.This()->Get(ctx, V8::Vector3_XKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> y = info.This()->Get(ctx, V8::Vector3_YKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> z = info.This()->Get(ctx, V8::Vector3_ZKey(isolate)).ToLocalChecked()->ToNumber(ctx).ToLocalChecked();

	std::ostringstream ss;
	ss << "Vector3{ x: " << x->Value() << ", y: " << y->Value() << ", z: " << z->Value() << " }";

	info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, ss.str().c_str(), v8::NewStringType::kNormal).ToLocalChecked());
}

static V8Class v8Vector3("Vector3", "", [] (const v8::FunctionCallbackInfo<v8::Value>& info) {
	v8::Isolate* isolate = info.GetIsolate();
	v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

	V8_CHECK(info.Length() == 3, "3 args expected");

	v8::Local<v8::Object> _this = info.This();
	
	v8::Local<v8::Number> x = info[0]->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> y = info[1]->ToNumber(ctx).ToLocalChecked();
	v8::Local<v8::Number> z = info[2]->ToNumber(ctx).ToLocalChecked();

	V8::DefineOwnProperty(isolate, ctx, _this, V8::Vector3_XKey(isolate), x, v8::PropertyAttribute::ReadOnly);
	V8::DefineOwnProperty(isolate, ctx, _this, V8::Vector3_YKey(isolate), y, v8::PropertyAttribute::ReadOnly);
	V8::DefineOwnProperty(isolate, ctx, _this, V8::Vector3_ZKey(isolate), z, v8::PropertyAttribute::ReadOnly);
}, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();

	V8::SetMethod(isolate, tpl, "toString", ToString);
}, false);
