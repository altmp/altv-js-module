#pragma once

namespace V8::Vehicle
{
	void EngineHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetEngineHealth()));
	}

	void EngineHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetEngineHealth(value->ToInteger(isolate)->Value());
	}

	void PetrolTankHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetPetrolTankHealth()));
	}

	void PetrolTankHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetPetrolTankHealth(value->ToInteger(isolate)->Value());
	}

	void BodyHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, vehicle->GetBodyHealth()));
	}

	void BodyHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetBodyHealth(value->ToInteger(isolate)->Value());
	}

	void BodyAdditionalHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, vehicle->GetBodyAdditionalHealth()));
	}

	void BodyAdditionalHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetBodyAdditionalHealth(value->ToInteger(info.GetIsolate())->Value());
	}

	void WheelsCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, vehicle->GetWheelsCount()));
	}

	void RepairsCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::NewFromUnsigned(isolate, vehicle->GetRepairsCount()));
	}

	//TODO:
	/*void LastAttackerGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8Resource* resource = V8Resource::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		alt::IEntity* damager = vehicle->GetLastAttacker();

		if (damager)
			info.GetReturnValue().Set(resource->GetOrCreateEntity(damager)->GetJSVal());
		else
			info.GetReturnValue().Set(v8::Null(isolate));
	}

	void LastDamagedWithGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8Resource* resource = V8Resource::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetLastDamagedWith()));
	}*/

	void SetWheelBurst(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWheelBurst(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void GetWheelBurst(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> wheelId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsWheelBurst(wheelId->Value())));
	}

	void SetWheelHasTire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWheelHasTire(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void GetWheelHasTire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> wheelId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->DoesWheelHasTire(wheelId->Value())));
	}

	void SetWheelDetached(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWheelDetached(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void GetIsWheelDetached(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> wheelId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsWheelDetached(wheelId->Value())));
	}

	void SetWheelOnFire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		vehicle->SetWheelOnFire(info[0]->ToInteger(isolate)->Value(), info[1]->ToBoolean(isolate)->Value());
	}

	void GetIsWheelOnFire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> wheelId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Boolean::New(isolate, vehicle->IsWheelOnFire(wheelId->Value())));
	}

	void SetWheelHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 2, "2 args expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> wheelId = info[0]->ToInteger(isolate);
		v8::Local<v8::Number> health = info[1]->ToNumber(isolate);

		vehicle->SetWheelHealth(wheelId->Value(), health->Value());
	}

	void GetWheelHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::Local<v8::Integer> wheelId = info[0]->ToInteger(isolate);

		info.GetReturnValue().Set(v8::Number::New(isolate, vehicle->GetWheelHealth(wheelId->Value())));
	}

	void SetHealthData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8_CHECK(info.Length() == 1, "1 arg expected");

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		v8::String::Utf8Value healthData(isolate, info[0]->ToString(isolate));

		vehicle->LoadHealthDataFromBase64(*healthData);
	}

	void GetHealthData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		v8::Isolate* isolate = info.GetIsolate();

		V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
		V8_CHECK(resource, "invalid resource");

		V8Entity* _this = V8Entity::Get(info.This());
		V8_CHECK(_this, "entity is invalid");

		Ref<IVehicle> vehicle = _this->GetHandle().As<IVehicle>();

		info.GetReturnValue().Set(v8::String::NewFromUtf8(info.GetIsolate(), vehicle->GetHealthDataBase64().CStr()));
	}
}
