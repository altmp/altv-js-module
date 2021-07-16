#pragma once

namespace V8::Vehicle
{
	void SetLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, light);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetLightDamaged(light, state);
	}

	void IsLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, light);

		V8_RETURN_BOOLEAN(vehicle->IsLightDamaged(light));
	}

	void SetWindowDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, window);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetWindowDamaged(window, state);
	}

	void IsWindowDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, window);

		V8_RETURN_BOOLEAN(vehicle->IsWindowDamaged(window));
	}

	void SetSpecialLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, light);
		V8_ARG_TO_BOOLEAN(2, state);

		vehicle->SetSpecialLightDamaged(light, state);
	}

	void IsSpecialLightDamaged(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, light);

		V8_RETURN_BOOLEAN(vehicle->IsSpecialLightDamaged(light));
	}

	void SetBumperDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, bumper);
		V8_ARG_TO_INT(2, damage);

		vehicle->SetBumperDamageLevel(bumper, damage);
	}

	void GetBumperDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, bumper);

		V8_RETURN_INT(vehicle->GetBumperDamageLevel(bumper));
	}

	void HasArmoredWindowsGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_BOOLEAN(vehicle->HasArmoredWindows());
	}

	void SetPartDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, part);
		V8_ARG_TO_INT(2, damage);

		vehicle->SetPartDamageLevel(part, damage);
	}

	void GetPartDamageLevel(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, part);

		V8_RETURN_INT(vehicle->GetPartDamageLevel(part));
	}

	void SetPartBulletHoles(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, part);
		V8_ARG_TO_INT(2, holes);

		vehicle->SetPartBulletHoles(part, holes);
	}

	void GetPartBulletHoles(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, part);

		V8_RETURN_INT(vehicle->GetPartBulletHoles(part));
	}

	void SetArmoredWindowHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, window);
		V8_ARG_TO_NUMBER(2, damage);

		vehicle->SetArmoredWindowHealth(window, damage);
	}

	void GetArmoredWindowHealth(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, window);

		V8_RETURN_NUMBER(vehicle->GetArmoredWindowHealth(window));
	}

	void SetArmoredWindowShootCount(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(2);

		V8_ARG_TO_INT(1, window);
		V8_ARG_TO_INT(2, count);

		vehicle->SetArmoredWindowShootCount(window, count);
	}

	void GetArmoredWindowShootCount(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, window);

		V8_RETURN_INT(vehicle->GetArmoredWindowShootCount(window));
	}

	void SetDamageStatus(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_STRING(1, data);

		vehicle->LoadDamageDataFromBase64(data);
	}

	void GetDamageStatus(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		V8_RETURN_STRING(vehicle->GetDamageDataBase64().CStr());
	}

	void SetFixed(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

		vehicle->SetFixed();
	}

	void SetWheelFixed(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_INT(1, wheelId);

		vehicle->SetWheelFixed(wheelId);
	}
}
