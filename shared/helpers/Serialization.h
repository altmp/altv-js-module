#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"

namespace V8Helpers
{
    alt::MValue V8ToMValue(v8::Local<v8::Value> val, bool allowFunction = true);
    v8::Local<v8::Value> MValueToV8(alt::MValueConst val);
    void MValueArgsToV8(alt::MValueArgs args, std::vector<v8::Local<v8::Value>>& v8Args);

    alt::MValueByteArray V8ToRawBytes(v8::Local<v8::Value> val);
    v8::MaybeLocal<v8::Value> RawBytesToV8(alt::MValueByteArrayConst bytes);

    namespace Serialization
    {
        // A serialized JavaScript value
        struct Value
        {
            uint8_t* data;
            size_t size;
            bool ownPtr;  // If true, the data pointer is owned by this object and must be freed when this object is destroyed

            bool Valid() const
            {
                return data != nullptr && size > 0;
            }
            void Reset()
            {
                data = nullptr;
                size = 0;
                ownPtr = false;
            }

            Value() : data(nullptr), size(0), ownPtr(false) {}
            Value(uint8_t* data, size_t size, bool ownPtr = true) : data(data), size(size), ownPtr(ownPtr) {}
            Value(Value&& otherValue)
            {
                data = otherValue.data;
                size = otherValue.size;
                ownPtr = otherValue.ownPtr;
                otherValue.Reset();
            }
            Value(const Value& otherValue)
            {
                data = otherValue.data;
                size = otherValue.size;
                ownPtr = otherValue.ownPtr;
                const_cast<Value&>(otherValue).Reset();
            }
            ~Value()
            {
                if(ownPtr) free(data);
            }
        };

        // Serializes a JS value to a binary format
        // Make sure the context is entered before calling this function
        inline Value Serialize(v8::Local<v8::Context> context, v8::Local<v8::Value> value, bool ownPtr = true)
        {
            v8::ValueSerializer serializer(context->GetIsolate());
            serializer.WriteHeader();
            if(serializer.WriteValue(context, value).IsNothing()) return Value{};
            std::pair<uint8_t*, size_t> data = serializer.Release();
            return Value{ data.first, data.second, ownPtr };
        }

        // Deserializes a JS value from a binary format
        // Make sure the context is entered before calling this function
        inline v8::MaybeLocal<v8::Value> Deserialize(v8::Local<v8::Context> context, const Value& value)
        {
            if(!value.Valid()) return v8::MaybeLocal<v8::Value>();
            v8::ValueDeserializer deserializer(context->GetIsolate(), value.data, value.size);
            v8::Maybe<bool> header = deserializer.ReadHeader(context);
            if(header.IsNothing() || header.FromJust() == false) return v8::MaybeLocal<v8::Value>();
            v8::MaybeLocal<v8::Value> result = deserializer.ReadValue(context);
            return result;
        }
    }  // namespace Serialization
}  // namespace V8Helpers
