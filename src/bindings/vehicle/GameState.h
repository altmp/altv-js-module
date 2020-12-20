#pragma once

namespace V8::Vehicle
{
	void EngineOnGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->IsEngineOn());
	}

	void EngineOnSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_BOOLEAN(value, state);

		vehicle->SetEngineOn(state);
	}

	void HandbrakeActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->IsHandbrakeActive());
	}

	void HeadlightColorGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetHeadlightColor());
	}

	void HeadlightColorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, color);

		vehicle->SetHeadlightColor(color);
	}

	void ActiveRadioStationGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetRadioStationIndex());
	}

	void ActiveRadioStationSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, index);

		vehicle->SetRadioStationIndex(index);
	}

	void SirenActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->IsSirenActive());
	}

	void SirenActiveSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_BOOLEAN(value, state);

		vehicle->SetSirenActive(state);
	}

	void LockStateGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_INTEGER(vehicle->GetLockState());
	}

	void LockStateSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_INTEGER(value, state);

		vehicle->SetLockState(state);
	}

	void DaylightOnGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->IsDaylightOn());
	}

	void NightlightOnGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->IsNightlightOn());
	}

	void RoofStateGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->GetRoofState());
	}

	void RoofStateSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_BOOLEAN(value, state);

		vehicle->SetRoofState(state);
	}

	void FlamethrowerActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->IsFlamethrowerActive());
	}

	void LightsMultiplierGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_NUMBER(vehicle->GetLightsMultiplier());
	}

	void LightsMultiplierSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_TO_NUMBER(value, multiplier);

		vehicle->SetLightsMultiplier(multiplier);
	}

	void SetDoorState(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INTEGER(1, door);
		V8_ARG_TO_INTEGER(2, state);

		vehicle->SetDoorState(door, state);
	}

	void GetDoorState(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INTEGER(1, door);
	
		V8_RETURN_INTEGER(vehicle->GetDoorState(door));
	}

	void SetWindowOpened(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INTEGER(1, window);
		V8_ARG_TO_BOOLEAN(2, state);
		
		vehicle->SetWindowOpened(window, state);
	}

	void GetWindowOpened(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INTEGER(1, window);
	
		V8_RETURN_BOOLEAN(vehicle->IsWindowOpened(window));
	}

	void SetGamestateData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_STRING(1, data);

		vehicle->LoadGameStateFromBase64(data);
	}

	void GetGamestateData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_STRING(vehicle->GetGameStateBase64().CStr());
	}
}
