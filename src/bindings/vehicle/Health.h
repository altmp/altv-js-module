#pragma once

namespace V8::Vehicle
{
	void EngineHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INT32(vehicle->GetEngineHealth());
	}

	void EngineHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, health);

		vehicle->SetEngineHealth(health);
	}

	void PetrolTankHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INT32(vehicle->GetPetrolTankHealth());
	}

	void PetrolTankHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, health);

		vehicle->SetPetrolTankHealth(health);
	}

	void BodyHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INT32(vehicle->GetBodyHealth());
	}

	void BodyHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, health);

		vehicle->SetBodyHealth(health);
	}

	void BodyAdditionalHealthGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INT32(vehicle->GetBodyAdditionalHealth());
	}

	void BodyAdditionalHealthSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, health);

		vehicle->SetBodyAdditionalHealth(health);
	}

	void WheelsCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_UINT32(vehicle->GetWheelsCount());
	}

	void RepairsCountGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_UINT32(vehicle->GetRepairsCount());
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
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);
		
		V8_ARG_TO_INTEGER(1, wheel);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetWheelBurst(wheel, state);
	}

	void GetWheelBurst(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);
		
		V8_ARG_TO_INTEGER(1, wheel);

		V8_RETURN_BOOLEAN(vehicle->IsWheelBurst(wheel));
	}

	void SetWheelHasTire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);
		
		V8_ARG_TO_INTEGER(1, wheel);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetWheelHasTire(wheel, state);
	}

	void GetWheelHasTire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);
		
		V8_ARG_TO_INTEGER(1, wheel);

		V8_RETURN_BOOLEAN(vehicle->DoesWheelHasTire(wheel));
	}

	void SetWheelDetached(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);
		
		V8_ARG_TO_INTEGER(1, wheel);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetWheelDetached(wheel, state);
	}

	void GetIsWheelDetached(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);
		
		V8_ARG_TO_INTEGER(1, wheel);

		V8_RETURN_BOOLEAN(vehicle->IsWheelDetached(wheel));
	}

	void SetWheelOnFire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);
		
		V8_ARG_TO_INTEGER(1, wheel);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetWheelOnFire(wheel, state);
	}

	void GetIsWheelOnFire(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);
		
		V8_ARG_TO_INTEGER(1, wheel);

		V8_RETURN_BOOLEAN(vehicle->IsWheelOnFire(wheel));
	}

	void SetWheelHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);
		
		V8_ARG_TO_INTEGER(1, wheel);
		V8_ARG_TO_NUMBER(2, health);

		vehicle->SetWheelHealth(wheel, health);
	}

	void GetWheelHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);
		
		V8_ARG_TO_INTEGER(1, wheel);

		V8_RETURN_NUMBER(vehicle->GetWheelHealth(wheel));
	}

	void SetHealthData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);
		
		V8_ARG_TO_STRING(1, data);

		vehicle->LoadHealthDataFromBase64(data);
	}

	void GetHealthData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_STRING(vehicle->GetHealthDataBase64().CStr());
	}
}
