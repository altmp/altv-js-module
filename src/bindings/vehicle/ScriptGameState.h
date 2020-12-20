#pragma once

namespace V8::Vehicle
{
	void ManualEngineControlGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_BOOLEAN(vehicle->IsManualEngineControl());
	}

	void ManualEngineControlSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_TO_BOOLEAN(value, toggle);

		vehicle->SetManualEngineControl(toggle);
	}

	void SetScriptData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE_CONTEXT();
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		V8_CHECK_ARGS_LEN(1);

		V8_ARG_TO_STRING(1, scriptData);

		vehicle->LoadScriptDataFromBase64(scriptData);
	}

	void GetScriptData(const v8::FunctionCallbackInfo<v8::Value>& info)
	{
		V8_GET_ISOLATE(info);
		V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
		
		V8_RETURN_STRING(vehicle->GetScriptDataBase64().CStr());
	}
}
