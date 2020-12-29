#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT();

	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN2(1, 2);

	v8::Local<v8::Object> _this = info.This();

	v8::Local<v8::Value> x, y;

	if (info.Length() == 2)
	{
		V8_ARG_CHECK_NUMBER(1);
		V8_ARG_CHECK_NUMBER(2);

		x = info[0];
		y = info[1];
	}
	else
	{
		v8::Local<v8::Value> val = info[0];

		if (val->IsArray())
		{
			v8::Local<v8::Array> arr = val.As<v8::Array>();
			V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

			x = arr->Get(ctx, 0).ToLocalChecked();
			y = arr->Get(ctx, 1).ToLocalChecked();

			V8_CHECK(x->IsNumber(), "Argument must be an array of 2 numbers");
			V8_CHECK(y->IsNumber(), "Argument must be an array of 2 numbers");
		}
		else if (val->IsObject())
		{
			v8::Local<v8::Object> obj = val.As<v8::Object>();

			x = obj->Get(ctx, V8::Vector3_XKey(isolate)).ToLocalChecked();
			y = obj->Get(ctx, V8::Vector3_YKey(isolate)).ToLocalChecked();

			V8_CHECK(x->IsNumber(), "x must be a number");
			V8_CHECK(y->IsNumber(), "y must be a number");
		}
		else
		{
			V8Helpers::Throw(isolate, "Argument must be an array of 2 numbers or IVector2");
			return;
		}
	}

	V8::DefineOwnProperty(isolate, ctx, _this, V8::Vector3_XKey(isolate), x, v8::PropertyAttribute::ReadOnly);
	V8::DefineOwnProperty(isolate, ctx, _this, V8::Vector3_YKey(isolate), y, v8::PropertyAttribute::ReadOnly);
}

extern V8Class v8Vector2("Vector2", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate *isolate = v8::Isolate::GetCurrent();

});
