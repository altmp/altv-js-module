#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"

#include "V8Entity.h"

#include <optional>

namespace V8Helpers
{
    bool SafeToBoolean(v8::Local<v8::Value> val, v8::Isolate* isolate, bool& out);
    bool SafeToInteger(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out);
    bool SafeToNumber(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, double& out);
    bool SafeToString(v8::Local<v8::Value> val, v8::Isolate* isolate, v8::Local<v8::Context> ctx, std::string& out);
    bool SafeToFunction(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Function>& out);
    bool SafeToObject(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Object>& out);
    bool SafeToRGBA(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::RGBA& out);
    bool SafeToVector3(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector3f& out);
    bool SafeToVector3Int(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector3i& out);
    bool SafeToVector2(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector2f& out);
    bool SafeToVector2Int(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector2i& out);
    bool SafeToQuaternion(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Quaternion& out);
    bool SafeToArrayBuffer(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBuffer>& out);
    bool SafeToArrayBufferView(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBufferView>& out);
    bool SafeToArray(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Array>& out);

    bool SafeToUInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint64_t& out);
    bool SafeToInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out);
    bool SafeToUInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint32_t& out);
    bool SafeToInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int32_t& out);

    template<typename T>
    bool SafeToBaseObject(v8::Local<v8::Value> val, v8::Isolate* isolate, T*& out)
    {
        V8Entity* v8BaseObject = V8Entity::Get(val);
        if(!v8BaseObject) return false;

        out = dynamic_cast<T*>(v8BaseObject->GetHandle());
        if(!out) return false;

        return true;
    }

    // * Function utilizing overloads to quickly convert a C++ value to a JS value
    // * These functions are defined in the header directly to utilize the 'inline' modifier

    inline v8::Local<v8::String> JSValue(const char* val)
    {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), val).ToLocalChecked();
    }
    inline v8::Local<v8::String> JSValue(const std::string& val)
    {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), val.c_str(), v8::NewStringType::kNormal, (int)val.size()).ToLocalChecked();
    }
    inline v8::Local<v8::String> JSValue(const uint16_t* val)
    {
        return v8::String::NewFromTwoByte(v8::Isolate::GetCurrent(), val).ToLocalChecked();
    }
    inline v8::Local<v8::Boolean> JSValue(bool val)
    {
        return v8::Boolean::New(v8::Isolate::GetCurrent(), val);
    }
    inline v8::Local<v8::Number> JSValue(double val)
    {
        return v8::Number::New(v8::Isolate::GetCurrent(), val);
    }
    inline v8::Local<v8::Integer> JSValue(int32_t val)
    {
        return v8::Integer::New(v8::Isolate::GetCurrent(), val);
    }
    inline v8::Local<v8::Integer> JSValue(uint32_t val)
    {
        return v8::Integer::NewFromUnsigned(v8::Isolate::GetCurrent(), val);
    }
    inline v8::Local<v8::BigInt> JSValue(int64_t val)
    {
        return v8::BigInt::New(v8::Isolate::GetCurrent(), val);
    }
    inline v8::Local<v8::BigInt> JSValue(uint64_t val)
    {
        return v8::BigInt::NewFromUnsigned(v8::Isolate::GetCurrent(), val);
    }
    template<class T>
    inline v8::Local<v8::Array> JSValue(alt::Array<T>& arr)
    {
        auto jsArr = v8::Array::New(v8::Isolate::GetCurrent(), arr.GetSize());
        for(int i = 0; i < arr.GetSize(); i++)
        {
            jsArr->Set(v8::Isolate::GetCurrent()->GetEnteredOrMicrotaskContext(), i, JSValue(arr[i]));
        }
        return jsArr;
    }
    template<class T>
    inline v8::Local<v8::Array> JSValue(std::vector<T>& arr)
    {
        auto jsArr = v8::Array::New(v8::Isolate::GetCurrent(), arr.size());
        for(int i = 0; i < arr.size(); i++)
        {
            jsArr->Set(v8::Isolate::GetCurrent()->GetEnteredOrMicrotaskContext(), i, JSValue(arr[i]));
        }
        return jsArr;
    }
    // Returns null
    inline v8::Local<v8::Primitive> JSValue(std::nullptr_t val)
    {
        return v8::Null(v8::Isolate::GetCurrent());
    }

    // Converts a JS value to a C++ value
    inline std::string CppValue(v8::Local<v8::Name> val)
    {
        return *v8::String::Utf8Value(v8::Isolate::GetCurrent(), val);
    }
    inline std::string CppValue(v8::Local<v8::Symbol> val)
    {
        return *v8::String::Utf8Value(v8::Isolate::GetCurrent(), val);
    }
    inline std::string CppValue(v8::Local<v8::String> val)
    {
        return *v8::String::Utf8Value(v8::Isolate::GetCurrent(), val);
    }
    inline double CppValue(v8::Local<v8::Number> val)
    {
        return val->Value();
    }
    inline int64_t CppValue(v8::Local<v8::Integer> val)
    {
        return val->Value();
    }
    inline uint32_t CppValue(v8::Local<v8::Uint32> val)
    {
        return val->Value();
    }
    inline int32_t CppValue(v8::Local<v8::Int32> val)
    {
        return val->Value();
    }
    inline uint64_t CppValue(v8::Local<v8::BigInt> val)
    {
        return val->Uint64Value();
    }
    inline bool CppValue(v8::Local<v8::Boolean> val)
    {
        return val->Value();
    }
    template<typename T, int Size = 0>
    std::optional<std::vector<T>> CppValue(v8::Local<v8::Array> arr)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        uint32_t size = arr->Length();
        if(Size != 0 && size != Size) return std::nullopt;

        std::vector<T> result;
        result.reserve(size);
        for(uint32_t i = 0; i < size; i++)
        {
            v8::MaybeLocal<v8::Value> maybeVal = arr->Get(ctx, i);
            v8::Local<v8::Value> val;
            if(!maybeVal.ToLocal(&val)) return std::nullopt;

            if constexpr(std::is_same_v<T, v8::Local<v8::Value>>) result.push_back(val);
            else if constexpr(std::is_integral_v<T> || std::is_floating_point_v<T>)
            {
                if(!val->IsNumber()) return std::nullopt;
                result.push_back((T)val->ToNumber(ctx).ToLocalChecked()->Value());
            }
            else if constexpr(std::is_same_v<T, std::string>)
            {
                if(!val->IsString()) return std::nullopt;
                result.push_back(*v8::String::Utf8Value(isolate, val->ToString(ctx).ToLocalChecked()));
            }
            else if constexpr(std::is_same_v<T, bool>)
            {
                if(!val->IsBoolean()) return std::nullopt;
                result.push_back(val->ToBoolean(isolate)->Value());
            }
            else if constexpr(std::is_same_v<T, std::vector<typename T::value_type>>)
            {
                using VecType = typename T::value_type;
                if(!val->IsArray()) return std::nullopt;
                std::optional<std::vector<VecType>> vec = CppValue<VecType>(val.As<v8::Array>());
                if(!vec.has_value()) return std::nullopt;
                result.push_back(vec.value());
            }
            else if constexpr(std::is_same_v<T, std::unordered_map<typename T::value_type::first_type, typename T::value_type::second_type>>)
            {
                using MapType = typename T::value_type;
                if constexpr(!std::is_same_v<std::string, typename MapType::first_type>) return std::nullopt;
                if(!val->IsObject()) return std::nullopt;
                std::optional<std::unordered_map<std::string, typename MapType::second_type>> vec = CppValue<typename MapType::second_type>(val.As<v8::Object>());
                if(!vec.has_value()) return std::nullopt;
                result.push_back(vec.value());
            }
            else
                static_assert("Invalid type specified to CppValue<v8::Array>");
        }
        return result;
    }
    template<typename T>
    std::optional<std::unordered_map<std::string, T>> CppValue(v8::Local<v8::Object> obj)
    {
        v8::Isolate* isolate = v8::Isolate::GetCurrent();
        v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
        std::optional<std::vector<std::string>> keys = CppValue<std::string>(obj->GetOwnPropertyNames(ctx).ToLocalChecked());
        if(!keys.has_value()) return std::nullopt;

        std::unordered_map<std::string, T> result;
        for(uint32_t i = 0; i < keys->size(); i++)
        {
            const std::string& key = keys->at(i);
            v8::MaybeLocal<v8::Value> maybeVal = obj->Get(ctx, JSValue(key));
            v8::Local<v8::Value> val;
            if(!maybeVal.ToLocal(&val)) return std::nullopt;

            if constexpr(std::is_same_v<T, v8::Local<v8::Value>>) result.insert({ key, val });
            else if constexpr(std::is_integral_v<T> || std::is_floating_point_v<T>)
            {
                if(!val->IsNumber()) return std::nullopt;
                result.insert({ key, (T)val->ToNumber(ctx).ToLocalChecked()->Value() });
            }
            else if constexpr(std::is_same_v<T, std::string>)
            {
                if(!val->IsString()) return std::nullopt;
                result.insert({ key, *v8::String::Utf8Value(isolate, val->ToString(ctx).ToLocalChecked()) });
            }
            else if constexpr(std::is_same_v<T, bool>)
            {
                if(!val->IsBoolean()) return std::nullopt;
                result.insert({ key, val->ToBoolean(isolate)->Value() });
            }
            else if constexpr(std::is_same_v<T, std::vector<typename T::value_type>>)
            {
                using VecType = typename T::value_type;
                if(!val->IsArray()) return std::nullopt;
                std::optional<std::vector<VecType>> vec = CppValue<VecType>(val.As<v8::Array>());
                if(!vec.has_value()) return std::nullopt;
                result.insert({ key, vec.value() });
            }
            else if constexpr(std::is_same_v<T, std::unordered_map<typename T::value_type::first_type, typename T::value_type::second_type>>)
            {
                using MapType = typename T::value_type;
                if constexpr(!std::is_same_v<std::string, typename MapType::first_type>) return std::nullopt;
                if(!val->IsObject()) return std::nullopt;
                std::optional<std::unordered_map<std::string, typename MapType::second_type>> vec = CppValue<typename MapType::second_type>(val.As<v8::Object>());
                if(!vec.has_value()) return std::nullopt;
                result.insert({ key, vec.value() });
            }
            else
                static_assert("Invalid type specified to CppValue<v8::Object>");
        }
        return result;
    }

    v8::Local<v8::Value> ConfigNodeToV8(Config::Value::ValuePtr node);

}  // namespace V8Helpers
