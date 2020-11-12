#include "stdafx.h"

#include "helpers/V8Helpers.h"
#include "helpers/V8ResourceImpl.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
	v8::Isolate* isolate = info.GetIsolate();

	V8_CHECK(info.IsConstructCall(), "Checkpoint constructor is not a function");
	V8_CHECK(info.Length() == 10, "10 args expected");

	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	v8::Local<v8::Integer> type = info[0]->ToInteger(isolate);
	v8::Local<v8::Number> x = info[1]->ToNumber(isolate);
	v8::Local<v8::Number> y = info[2]->ToNumber(isolate);
	v8::Local<v8::Number> z = info[3]->ToNumber(isolate);
	v8::Local<v8::Number> radius = info[4]->ToNumber(isolate);
	v8::Local<v8::Number> height = info[5]->ToNumber(isolate);
	v8::Local<v8::Integer> r = info[6]->ToInteger(isolate);
	v8::Local<v8::Integer> g = info[7]->ToInteger(isolate);
	v8::Local<v8::Integer> b = info[8]->ToInteger(isolate);
	v8::Local<v8::Integer> a = info[9]->ToInteger(isolate);

	alt::Position pos(x->Value(), y->Value(), z->Value());
	alt::RGBA color(r->Value(), g->Value(), b->Value(), a->Value());

	Ref<ICheckpoint> cp = ICore::Instance().CreateCheckpoint(type->Value(), pos, radius->Value(), height->Value(), color);

	if (cp)
		resource->BindEntity(info.This(), cp.Get());
	else
		V8Helpers::Throw(isolate, "Failed to create Checkpoint");
}

extern V8Class v8Colshape;
extern V8Class v8Checkpoint("Checkpoint", v8Colshape, Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
});
