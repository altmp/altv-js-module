#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void PointConstructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	V8_GET_ISOLATE_CONTEXT_RESOURCE();
	V8_CHECK_CONSTRUCTOR();
	V8_CHECK_ARGS_LEN(4);
	
	V8_ARG_TO_INTEGER(1, type);
	V8_ARG_TO_NUMBER(2, x);
	V8_ARG_TO_NUMBER(3, y);
	V8_ARG_TO_NUMBER(4, z);

	alt::Position pos(x, y, z);

	Ref<IBlip> blip = ICore::Instance().CreateBlip(nullptr, (alt::IBlip::BlipType)type, pos);

	V8_CHECK(blip, "Failed to create blip");
	resource->BindEntity(info.This(), blip.Get());
}

extern V8Class v8WorldObject;
extern V8Class v8Blip("Blip", v8WorldObject, nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
});

extern V8Class v8PointBlip("PointBlip", v8Blip, PointConstructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
});

/*static V8Class v8EntityBlip("EntityBlip", "Blip", nullptr, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
});*/