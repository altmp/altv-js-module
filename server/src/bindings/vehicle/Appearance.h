#pragma once

#include "V8Helpers.h"

namespace V8Helpers::Vehicle
{
    namespace detail
    {
        void NeonActiveGetterHandler(v8::Local<v8::Name> property, const v8::PropertyCallbackInfo<v8::Value>& info)
        {
            V8_GET_ISOLATE_CONTEXT();
            V8_GET_DATA(alt::IVehicle, vehicle);

            bool left, right, front, back;
            vehicle->GetNeonActive(&left, &right, &front, &back);

            std::string prop = V8Helpers::CppValue(property);
            if(prop == "left") V8_RETURN(left);
            else if(prop == "right")
                V8_RETURN(right);
            else if(prop == "front")
                V8_RETURN(front);
            else if(prop == "back")
                V8_RETURN(back);
            else
                V8_RETURN(v8::Undefined(isolate));
        }
        void NeonActiveSetterHandler(v8::Local<v8::Name> property, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<v8::Value>& info)
        {
            V8_GET_ISOLATE_CONTEXT();
            V8_GET_DATA(alt::IVehicle, vehicle);
            V8_TO_BOOLEAN(value, neonActive);
            std::string prop = V8Helpers::CppValue(property);

            bool left, right, front, back;
            vehicle->GetNeonActive(&left, &right, &front, &back);

            if(prop == "left") left = neonActive;
            else if(prop == "right")
                right = neonActive;
            else if(prop == "front")
                front = neonActive;
            else if(prop == "back")
                back = neonActive;

            vehicle->SetNeonActive(left, right, front, back);
            V8_RETURN(neonActive);
        }
        void NeonActiveEnumeratorHandler(const v8::PropertyCallbackInfo<v8::Array>& info)
        {
            V8_GET_ISOLATE_CONTEXT();

            v8::Local<v8::Array> arr = v8::Array::New(isolate, 4);
            arr->Set(ctx, 0, V8Helpers::JSValue("left"));
            arr->Set(ctx, 1, V8Helpers::JSValue("right"));
            arr->Set(ctx, 2, V8Helpers::JSValue("front"));
            arr->Set(ctx, 3, V8Helpers::JSValue("back"));

            V8_RETURN(arr);
        }
    }  // namespace detail

    void ModKitGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE();
        V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
        V8_RETURN_INT(_this->GetModKit());
    }

    void ModKitSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(_this, IVehicle);
        V8_TO_INTEGER(value, _val);
        V8_CHECK(_this->SetModKit(_val), "wrong modkit");
    }

    void NeonActiveGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

        V8_RETURN(V8Helpers::CreateCustomObject(isolate, _this.Get(), &detail::NeonActiveGetterHandler, &detail::NeonActiveSetterHandler, nullptr, &detail::NeonActiveEnumeratorHandler));
    }

    void NeonActiveSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(_this, IVehicle);

        V8_TO_OBJECT(value, neonActive);
        V8_OBJECT_GET_BOOLEAN(neonActive, "left", left);
        V8_OBJECT_GET_BOOLEAN(neonActive, "right", right);
        V8_OBJECT_GET_BOOLEAN(neonActive, "front", front);
        V8_OBJECT_GET_BOOLEAN(neonActive, "back", back);

        _this->SetNeonActive(left, right, front, back);
    }

    void SetWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(2);

        V8_ARG_TO_INT(1, type);
        V8_ARG_TO_INT(2, variation);

        vehicle->SetWheels(type, variation);
    }

    void SetRearWheels(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(1);

        V8_ARG_TO_INT(1, variation);

        vehicle->SetRearWheels(variation);
    }

    void GetModsCount(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(1);

        V8_ARG_TO_INT(1, category);

        V8_RETURN_INT(vehicle->GetModsCount(category));
    }

    void SetMod(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(2);

        V8_ARG_TO_INT(1, category);
        V8_ARG_TO_INT(2, id);

        if(!vehicle->SetMod(category, id))
        {
            if(vehicle->GetModKit() == 0) V8Helpers::Throw(isolate, "modkit is not set");
            else
                V8Helpers::Throw(isolate, "mod doesn't exist");
        }
    }

    void GetMod(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(1);

        V8_ARG_TO_INT(1, category);

        V8_RETURN_INT(vehicle->GetMod(category));
    }

    void SetExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(2);

        V8_ARG_TO_INT(1, category);
        V8_ARG_TO_BOOLEAN(2, state);

        vehicle->ToggleExtra(category, state);
    }

    void GetExtra(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(1);

        V8_ARG_TO_INT(1, category);

        V8_RETURN_BOOLEAN(vehicle->IsExtraOn(category));
    }

    void SetAppearanceData(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE_CONTEXT();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);
        V8_CHECK_ARGS_LEN(1);

        V8_ARG_TO_STRING(1, data);

        vehicle->LoadAppearanceDataFromBase64(data);
    }

    void GetAppearanceData(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        V8_GET_ISOLATE();
        V8_GET_THIS_BASE_OBJECT(vehicle, IVehicle);

        V8_RETURN_STRING(vehicle->GetAppearanceDataBase64());
    }
}  // namespace V8Helpers::Vehicle
