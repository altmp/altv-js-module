#pragma once

namespace V8::Vehicle
{
	void ManualEngineControlGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsManualEngineControl()));
	}

	void ManualEngineControlSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetManualEngineControl(value->ToBoolean(info.GetIsolate())->Value());
	}

	void SetScriptData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::String::Utf8Value scriptData(isolate, info[0]->ToString(isolate));

		vehicle->LoadScriptDataFromBase64(*scriptData);
	}

	void GetScriptData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), vehicle->GetScriptDataBase64().CStr()));
	}
}
