#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void PointConstructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 4, "4 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Integer> type = info[0]->ToInteger(isolate);
	v8::Local<v8::Number> x = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[3]->ToNumber(isolate);

	alt::Position pos(x->Value(), y->Value(), z->Value());

	Ref<IBlip> blip = ICore::Instance().CreateBlip(nullptr, (alt::IBlip::BlipType)type->Value(), pos);

	if (blip)
		resource->BindEntity(info.This(), blip.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create Blip");
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