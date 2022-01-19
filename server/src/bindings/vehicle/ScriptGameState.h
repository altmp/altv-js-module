#pragma once

namespace V8Helpers::Vehicle
{
    void SetScriptData(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(1);

        V8_ARG_TO_STD_STRING(1, scriptData);

        vehicle->LoadScriptDataFromBase64(scriptData);
    }

    void GetScriptData(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

        V8_RETURN_STD_STRING(vehicle->GetScriptDataBase64());
    }
}  // namespace V8Helpers::Vehicle
