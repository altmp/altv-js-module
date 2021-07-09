#pragma once

namespace V8::Vehicle
{
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

		V8_RETURN_INT(vehicle->GetDoorState(door));
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
		V8_GET_ISOLATE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_STRING(vehicle->GetGameStateBase64().CStr());
	}
}
