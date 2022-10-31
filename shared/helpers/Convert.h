#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"

#include "V8Entity.h"

#include "Serialization.h"

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
    bool SafeToArrayBuffer(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBuffer>& out);
    bool SafeToArrayBufferView(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::ArrayBufferView>& out);
    bool SafeToArray(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Array>& out);

    bool SafeToUInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint64_t& out);
    bool SafeToInt64(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out);
    bool SafeToUInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, uint32_t& out);
    bool SafeToInt32(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int32_t& out);

    template<typename T>
    bool SafeToBaseObject(v8::Local<v8::Value> val, v8::Isolate* isolate, alt::Ref<T>& out)
    {
        V8Entity* v8BaseObject = V8Entity::Get(val);
        if(!v8BaseObject) return false;

        out = v8BaseObject->GetHandle().As<T>();
        if(out.IsEmpty()) return false;

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

    class JSVal;
    class JSArray
    {
        std::vector<std::shared_ptr<JSVal>> values;
        v8::Local<v8::Context> ctx;

    public:
        JSArray() = default;
        JSArray(v8::Local<v8::Array> array, v8::Local<v8::Context> _ctx) : ctx(_ctx)
        {
            uint32_t size = array->Length();
            values.reserve(size);
            for(uint32_t i = 0; i < size; i++) values.push_back(std::make_shared<JSVal>(array->Get(ctx, i).ToLocalChecked(), ctx));
        }
        JSArray(const std::vector<v8::Local<v8::Value>>& array, v8::Local<v8::Context> _ctx) : ctx(_ctx)
        {
            uint32_t size = array.size();
            values.reserve(size);
            for(uint32_t i = 0; i < size; i++) values.push_back(std::make_shared<JSVal>(array[i], ctx));
        }

        std::shared_ptr<JSVal> Get(size_t index)
        {
            if(index > values.size()) return {};
            return values.at(index);
        }
        size_t GetSize()
        {
            return values.size();
        }

        std::shared_ptr<JSVal> operator[](size_t index)
        {
            return Get(index);
        }

        decltype(values)::iterator begin()
        {
            return values.begin();
        }
        decltype(values)::const_iterator begin() const
        {
            return values.begin();
        }
        decltype(values)::iterator end()
        {
            return values.end();
        }
        decltype(values)::const_iterator end() const
        {
            return values.end();
        }
    };

    class JSObject
    {
        std::unordered_map<std::string, std::shared_ptr<JSVal>> values;
        v8::Local<v8::Context> ctx;

    public:
        JSObject() = default;
        JSObject(v8::Local<v8::Object> object, v8::Local<v8::Context> _ctx) : ctx(_ctx)
        {
            v8::Isolate* isolate = ctx->GetIsolate();
            v8::Local<v8::Array> keys = object->GetOwnPropertyNames(ctx, v8::PropertyFilter::SKIP_SYMBOLS, v8::KeyConversionMode::kConvertToString).ToLocalChecked();
            uint32_t size = keys->Length();
            for(uint32_t i = 0; i < size; i++)
            {
                v8::Local<v8::String> jsKey = keys->Get(ctx, i).ToLocalChecked().As<v8::String>();
                std::string key = *v8::String::Utf8Value(isolate, jsKey);
                values.insert({ key, std::make_shared<JSVal>(object->Get(ctx, jsKey).ToLocalChecked(), ctx) });
            }
        }

        std::shared_ptr<JSVal> Get(const std::string& key)
        {
            auto result = values.find(key);
            if(result == values.end()) return {};
            return result->second;
        }
        bool Has(const std::string& key)
        {
            return values.contains(key);
        }

        std::shared_ptr<JSVal> operator[](const std::string& key)
        {
            return Get(key);
        }
        std::shared_ptr<JSVal> operator[](const char* key)
        {
            return Get(key);
        }

        decltype(values)::iterator begin()
        {
            return values.begin();
        }
        decltype(values)::const_iterator begin() const
        {
            return values.begin();
        }
        decltype(values)::iterator end()
        {
            return values.end();
        }
        decltype(values)::const_iterator end() const
        {
            return values.end();
        }
    };

    class JSVal
    {
    public:
        enum class Type : uint8_t
        {
            INVALID,
            NUMBER,
            BIGINT,
            STRING,
            BOOLEAN,
            ARRAY,
            OBJECT
        };

    private:
        v8::Local<v8::Value> value;
        v8::Local<v8::Context> ctx;
        Type type;

    public:
        JSVal() = default;
        JSVal(v8::Local<v8::Value> _value, v8::Local<v8::Context> _ctx) : value(_value), ctx(_ctx)
        {
            if(value->IsNumber()) type = Type::NUMBER;
            else if(value->IsBigInt())
                type = Type::BIGINT;
            else if(value->IsString())
                type = Type::STRING;
            else if(value->IsBoolean())
                type = Type::BOOLEAN;
            else if(value->IsArray())
                type = Type::ARRAY;
            else if(value->IsObject())
                type = Type::OBJECT;
            else
                type = Type::INVALID;
        }

        template<typename T>
        T Get(const T& fallback = T())
        {
            if(value.IsEmpty() || type == Type::INVALID) return fallback;

            v8::Isolate* isolate = ctx->GetIsolate();
            // Number
            if constexpr((std::is_unsigned_v<T> && !std::is_same_v<T, uint64_t>) || (std::is_signed_v<T> && !std::is_same_v<T, int64_t>))
            {
                if(type != Type::NUMBER && type != Type::STRING) return fallback;
                double val;
                if(!value->NumberValue(ctx).To(&val)) return fallback;
                return (T)val;
            }
            // BigInt
            if constexpr(std::is_same_v<T, uint64_t> || std::is_same_v<T, int64_t>)
            {
                if(type != Type::BIGINT && type != Type::NUMBER && type != Type::STRING) return fallback;
                auto maybe = value->ToBigInt(ctx);
                v8::Local<v8::BigInt> val;
                if(!value->ToBigInt(ctx).ToLocal(&val)) return fallback;
                if constexpr(std::is_same_v<T, uint64_t>) return val->Uint64Value();
                else
                    return val->Int64Value();
            }
            // String
            if constexpr(std::is_same_v<T, std::string>)
            {
                if(type != Type::STRING) return fallback;
                return std::string(*v8::String::Utf8Value{ isolate, value.As<v8::String>() });
            }
            // Boolean
            if constexpr(std::is_same_v<T, bool>)
            {
                if(type != Type::BOOLEAN) return fallback;
                return value.As<v8::Boolean>()->Value();
            }
            // Array
            if constexpr(std::is_same_v<T, JSArray>)
            {
                if(type != Type::ARRAY) return fallback;
                return JSArray{ value.As<v8::Array>(), ctx };
            }
            // Object
            if constexpr(std::is_same_v<T, JSObject>)
            {
                if(type != Type::OBJECT) return fallback;
                return JSObject{ value.As<v8::Object>(), ctx };
            }

            // *** Special cases
            if constexpr(std::is_same_v<T, alt::MValue>)
            {
                return V8Helpers::V8ToMValue(value);
            }

            return fallback;
        }

        Type GetType()
        {
            return type;
        }
    };

    inline JSArray GetFunctionParams(const v8::FunctionCallbackInfo<v8::Value>& info)
    {
        int size = info.Length();
        std::vector<v8::Local<v8::Value>> params;
        params.reserve(size);
        v8::Local<v8::Context> ctx = info.GetIsolate()->GetEnteredOrMicrotaskContext();
        for(int i = 0; i < size; i++) params.push_back(info[i]);
        return JSArray{ params, ctx };
    }

    v8::Local<v8::Value> ConfigNodeToV8(Config::Value::ValuePtr node);

}  // namespace V8Helpers
