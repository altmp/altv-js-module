#pragma once

namespace V8::Vehicle
{
	void SetLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetLightDamaged(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void IsLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> lightId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsLightDamaged(lightId->Value())));
	}

	void SetWindowDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWindowDamaged(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void IsWindowDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> windowId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsWindowDamaged(windowId->Value())));
	}

	void SetSpecialLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetSpecialLightDamaged(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void IsSpecialLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> specialLightId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsSpecialLightDamaged(specialLightId->Value())));
	}

	void SetBumperDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetBumperDamageLevel(info[0]->ToInteger(isolate)->Value(), info[1]->ToInteger(isolate)->Value());
	}

	void GetBumperDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> part = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetBumperDamageLevel(part->Value())));
	}

	void HasArmoredWindowsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->HasArmoredWindows()));
	}

	void SetPartDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetPartDamageLevel(info[0]->ToInteger(isolate)->Value(), info[1]->ToInteger(isolate)->Value());
	}

	void GetPartDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> part = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetPartDamageLevel(part->Value())));
	}

	void SetPartBulletHoles(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetPartBulletHoles(info[0]->ToInteger(isolate)->Value(), info[1]->ToInteger(isolate)->Value());
	}

	void GetPartBulletHoles(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> part = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetPartBulletHoles(part->Value())));
	}

	void SetArmoredWindowHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetArmoredWindowHealth(info[0]->ToInteger(isolate)->Value(), info[1]->ToNumber(isolate)->Value());
	}

	void GetArmoredWindowHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> windowId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Number::New(isolate, vehicle->GetArmoredWindowHealth(windowId->Value())));
	}

	void SetArmoredWindowShootCount(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetArmoredWindowShootCount(info[0]->ToInteger(isolate)->Value(), info[1]->ToInteger(isolate)->Value());
	}

	void GetArmoredWindowShootCount(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> windowId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetArmoredWindowShootCount(windowId->Value())));
	}

	void SetDamageStatus(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::String::Utf8Value damageStatus(isolate, info[0]->ToString(isolate));

		vehicle->LoadDamageDataFromBase64(*damageStatus);
	}

	void SetFixed(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetFixed();
	}

	void GetDamageStatus(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::String::NewFromUtf8(isolate, vehicle->GetDamageDataBase64().CStr()));
	}
}
