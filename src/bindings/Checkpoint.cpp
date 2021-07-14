#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();

	V8_CHECK(info.IsConstructCall(), "Checkpoint constructor is not a function");
	V8_CHECK_ARGS_LEN(10);

	V8_ARG_TO_INT(1, type);
	V8_ARG_TO_NUMBER(2, x);
	V8_ARG_TO_NUMBER(3, y);
	V8_ARG_TO_NUMBER(4, z);
	V8_ARG_TO_NUMBER(5, radius);
	V8_ARG_TO_NUMBER(6, height);
	V8_ARG_TO_INT(7, r);
	V8_ARG_TO_INT(8, g);
	V8_ARG_TO_INT(9, b);
	V8_ARG_TO_INT(10, a);

	alt::Position pos(x, y, z);
	alt::RGBA color(r, g, b, a);

	Ref<ICheckpoint> cp = ICore::Instance().CreateCheckpoint(type, pos, radius, height, color);

	if (cp)
		resource->BindEntity(info.This(), cp.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create Checkpoint");
}

extern V8Class v8Colshape;
extern V8Class v8Checkpoint("Checkpoint", v8Colshape, Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
});
