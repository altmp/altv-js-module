#include "Serialization.h"
#include "V8ResourceImpl.h"
#include "Bindings.h"

alt::MValue V8Helpers::V8ToMValue(v8::Local<v8::Value> val, bool allowFunction)
{
    auto& core = alt::ICore::Instance();

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    if(val.IsEmpty()) return core.CreateMValueNone();

    if(val->IsUndefined()) return core.CreateMValueNone();

    if(val->IsNull()) return core.CreateMValueNil();

    if(val->IsBoolean()) return core.CreateMValueBool(val->BooleanValue(isolate));

    if(val->IsInt32()) return core.CreateMValueInt(val->Int32Value(ctx).ToChecked());

    if(val->IsUint32()) return core.CreateMValueUInt(val->Uint32Value(ctx).ToChecked());

    if(val->IsBigInt()) return core.CreateMValueInt(val.As<v8::BigInt>()->Int64Value());

    if(val->IsNumber()) return core.CreateMValueDouble(val->NumberValue(ctx).ToChecked());

    if(val->IsString()) return core.CreateMValueString(*v8::String::Utf8Value(isolate, val));

    if(val->IsObject())
    {
        if(val->IsArray())
        {
            v8::Local<v8::Array> v8Arr = val.As<v8::Array>();
            alt::MValueList list = core.CreateMValueList(v8Arr->Length());

            for(uint32_t i = 0; i < v8Arr->Length(); ++i)
            {
                v8::Local<v8::Value> value;
                if(!v8Arr->Get(ctx, i).ToLocal(&value)) continue;
                list->Set(i, V8ToMValue(value, allowFunction));
            }

            return list;
        }
        else if(val->IsFunction())
        {
            if(!allowFunction)
            {
                Log::Error << V8Helpers::SourceLocation::GetCurrent(isolate).ToString() << " "
                           << "Cannot convert function to MValue" << Log::Endl;
                return core.CreateMValueNone();
            }
            v8::Local<v8::Function> v8Func = val.As<v8::Function>();
            return V8ResourceImpl::Get(ctx)->GetFunction(v8Func);
        }
        else if(val->IsArrayBuffer())
        {
            auto v8Buffer = val.As<v8::ArrayBuffer>()->GetBackingStore();
            return core.CreateMValueByteArray((uint8_t*)v8Buffer->Data(), v8Buffer->ByteLength());
        }
        else if(val->IsMap())
        {
            v8::Local<v8::Map> map = val.As<v8::Map>();
            v8::Local<v8::Array> mapArr = map->AsArray();
            uint32_t size = mapArr->Length();

            alt::MValueDict dict = alt::ICore::Instance().CreateMValueDict();
            for(uint32_t i = 0; i < size; i += 2)
            {
                auto maybeKey = mapArr->Get(ctx, i);
                auto maybeValue = mapArr->Get(ctx, i + 1);
                v8::Local<v8::Value> key;
                v8::Local<v8::Value> value;

                if(!maybeKey.ToLocal(&key)) continue;
                if(!maybeValue.ToLocal(&value)) continue;
                alt::String keyString = V8Helpers::Stringify(ctx, val);
                if(keyString.IsEmpty()) continue;
                dict->Set(keyString, V8ToMValue(value, false));
            }
            return dict;
        }
        else
        {
            V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
            v8::Local<v8::Object> v8Obj = val.As<v8::Object>();

            // if (v8Obj->InstanceOf(ctx, v8Vector3->JSValue(isolate, ctx)).ToChecked())
            if(resource->IsVector3(v8Obj))
            {
                v8::Local<v8::Value> x, y, z;
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocal(&x), "Failed to convert Vector3 to MValue", core.CreateMValueNil());
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocal(&y), "Failed to convert Vector3 to MValue", core.CreateMValueNil());
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocal(&z), "Failed to convert Vector3 to MValue", core.CreateMValueNil());

                return core.CreateMValueVector3(alt::Vector3f{ x.As<v8::Number>()->Value(), y.As<v8::Number>()->Value(), z.As<v8::Number>()->Value() });
            }
            else if(resource->IsVector2(v8Obj))
            {
                v8::Local<v8::Value> x, y;
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocal(&x), "Failed to convert Vector2 to MValue", core.CreateMValueNil());
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocal(&y), "Failed to convert Vector2 to MValue", core.CreateMValueNil());

                return core.CreateMValueVector2(alt::Vector2f{ x.As<v8::Number>()->Value(), y.As<v8::Number>()->Value() });
            }
            else if(resource->IsRGBA(v8Obj))
            {
                v8::Local<v8::Value> r, g, b, a;
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::RGBA_RKey(isolate)).ToLocal(&r), "Failed to convert RGBA to MValue", core.CreateMValueNil());
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::RGBA_GKey(isolate)).ToLocal(&g), "Failed to convert RGBA to MValue", core.CreateMValueNil());
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::RGBA_BKey(isolate)).ToLocal(&b), "Failed to convert RGBA to MValue", core.CreateMValueNil());
                V8_CHECK_RETN(v8Obj->Get(ctx, V8Helpers::RGBA_AKey(isolate)).ToLocal(&a), "Failed to convert RGBA to MValue", core.CreateMValueNil());

                return core.CreateMValueRGBA(
                  alt::RGBA{ (uint8_t)r.As<v8::Number>()->Value(), (uint8_t)g.As<v8::Number>()->Value(), (uint8_t)b.As<v8::Number>()->Value(), (uint8_t)a.As<v8::Number>()->Value() });
            }
            else if(resource->IsBaseObject(v8Obj))
            {
                V8Entity* ent = V8Entity::Get(v8Obj);
                Log::Debug << "Instanceof BaseObject" << Log::Endl;

                V8_CHECK_RETN(ent, "Unable to convert base object to MValue because it was destroyed and is now invalid", core.CreateMValueNil());
                return core.CreateMValueBaseObject(ent->GetHandle());
            }
            else
            {
                alt::MValueDict dict = core.CreateMValueDict();
                v8::Local<v8::Array> keys;

                V8_CHECK_RETN(v8Obj->GetOwnPropertyNames(ctx).ToLocal(&keys), "Failed to convert object to MValue", core.CreateMValueNil());
                for(uint32_t i = 0; i < keys->Length(); ++i)
                {
                    v8::Local<v8::Value> v8Key;
                    V8_CHECK_RETN(keys->Get(ctx, i).ToLocal(&v8Key), "Failed to convert object to MValue", core.CreateMValueNil());
                    v8::Local<v8::Value> value;
                    V8_CHECK_RETN(v8Obj->Get(ctx, v8Key).ToLocal(&value), "Failed to convert object to MValue", core.CreateMValueNil());

                    if(value->IsUndefined()) continue;
                    std::string key = *v8::String::Utf8Value(isolate, v8Key);
                    dict->Set(key, V8ToMValue(value, allowFunction));
                }

                return dict;
            }
        }
    }

    return core.CreateMValueNone();
}

v8::Local<v8::Value> V8Helpers::MValueToV8(alt::MValueConst val)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    switch(val->GetType())
    {
        case alt::IMValue::Type::NONE: return v8::Undefined(isolate);
        case alt::IMValue::Type::NIL: return V8Helpers::JSValue(nullptr);
        case alt::IMValue::Type::BOOL: return V8Helpers::JSValue(val.As<alt::IMValueBool>()->Value());
        case alt::IMValue::Type::INT:
        {
            int64_t _val = val.As<alt::IMValueInt>()->Value();

            if(_val >= INT_MIN && _val <= INT_MAX) return V8Helpers::JSValue((int32_t)_val);

            return V8Helpers::JSValue(_val);
        }
        case alt::IMValue::Type::UINT:
        {
            uint64_t _val = val.As<alt::IMValueUInt>()->Value();

            if(_val <= UINT_MAX) return V8Helpers::JSValue((uint32_t)_val);

            return V8Helpers::JSValue(_val);
        }
        case alt::IMValue::Type::DOUBLE: return V8Helpers::JSValue(val.As<alt::IMValueDouble>()->Value());
        case alt::IMValue::Type::STRING: return V8Helpers::JSValue(val.As<alt::IMValueString>()->Value());
        case alt::IMValue::Type::LIST:
        {
            alt::MValueListConst list = val.As<alt::IMValueList>();
            v8::Local<v8::Array> v8Arr = v8::Array::New(isolate, list->GetSize());

            for(uint32_t i = 0; i < list->GetSize(); ++i) v8Arr->Set(ctx, i, MValueToV8(list->Get(i)));

            return v8Arr;
        }
        case alt::IMValue::Type::DICT:
        {
            alt::MValueDictConst dict = val.As<alt::IMValueDict>();
            v8::Local<v8::Object> v8Obj = v8::Object::New(isolate);

            for(auto it = dict->Begin(); it; it = dict->Next())
            {
                v8Obj->Set(ctx, V8Helpers::JSValue(it->GetKey()), MValueToV8(it->GetValue()));
            }

            return v8Obj;
        }
        case alt::IMValue::Type::BASE_OBJECT:
        {
            alt::Ref<alt::IBaseObject> ref = val.As<alt::IMValueBaseObject>()->Value();
            return V8ResourceImpl::Get(ctx)->GetBaseObjectOrNull(ref);
        }
        case alt::IMValue::Type::FUNCTION:
        {
            alt::MValueFunctionConst fn = val.As<alt::IMValueFunction>();
            v8::Local<v8::External> extFn = v8::External::New(isolate, new alt::MValueFunctionConst(fn));

            v8::Local<v8::Function> func;
            V8_CHECK_RETN(v8::Function::New(ctx, V8Helpers::FunctionCallback, extFn).ToLocal(&func), "Failed to convert MValue to function", v8::Undefined(isolate));
            return func;
        }
        case alt::IMValue::Type::VECTOR3: return V8ResourceImpl::Get(ctx)->CreateVector3(val.As<alt::IMValueVector3>()->Value());
        case alt::IMValue::Type::VECTOR2: return V8ResourceImpl::Get(ctx)->CreateVector2(val.As<alt::IMValueVector2>()->Value());
        case alt::IMValue::Type::RGBA: return V8ResourceImpl::Get(ctx)->CreateRGBA(val.As<alt::IMValueRGBA>()->Value());
        case alt::IMValue::Type::BYTE_ARRAY:
        {
            alt::MValueByteArrayConst buffer = val.As<alt::IMValueByteArray>();
            // Check if the buffer is a raw JS value buffer
            v8::MaybeLocal<v8::Value> jsVal = RawBytesToV8(buffer);
            if(!jsVal.IsEmpty()) return jsVal.ToLocalChecked();

            v8::Local<v8::ArrayBuffer> v8Buffer = v8::ArrayBuffer::New(isolate, buffer->GetSize());
            std::memcpy(v8Buffer->GetBackingStore()->Data(), buffer->GetData(), buffer->GetSize());
            return v8Buffer;
        }
        default: Log::Warning << "V8Helpers::MValueToV8 Unknown MValue type " << (int)val->GetType() << Log::Endl;
    }

    return v8::Undefined(isolate);
}

void V8Helpers::MValueArgsToV8(alt::MValueArgs args, std::vector<v8::Local<v8::Value>>& v8Args)
{
    for(uint64_t i = 0; i < args.GetSize(); ++i) v8Args.push_back(MValueToV8(args[i]));
}

// Magic bytes to identify raw JS value buffers
static uint8_t magicBytes[] = { 'J', 'S', 'V', 'a', 'l' };

enum class RawValueType : uint8_t
{
    GENERIC,
    ENTITY,
    VECTOR3,
    VECTOR2,
    RGBA,
    ARRAY,
    INVALID
};

extern V8Class v8Entity;
static inline RawValueType GetValueType(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val)
{
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
    bool result;
    if(val->IsSharedArrayBuffer() || val->IsFunction() || val->IsProxy() || val->IsPromise()) return RawValueType::INVALID;
    if(val->InstanceOf(ctx, v8Entity.JSValue(ctx->GetIsolate(), ctx)).To(&result) && result) return RawValueType::ENTITY;
    if(resource->IsVector3(val)) return RawValueType::VECTOR3;
    if(resource->IsVector2(val)) return RawValueType::VECTOR2;
    if(resource->IsRGBA(val)) return RawValueType::RGBA;
    if(val->IsArray()) return RawValueType::ARRAY;
    else
        return RawValueType::GENERIC;
}

static inline bool WriteRawValue(v8::Local<v8::Context> ctx, v8::ValueSerializer& serializer, RawValueType type, v8::Local<v8::Value> val)
{
    serializer.WriteRawBytes(&type, sizeof(uint8_t));
    switch(type)
    {
        case RawValueType::GENERIC:
        {
            bool result;
            if(!serializer.WriteValue(ctx, val).To(&result) || !result) return false;
            break;
        }
        case RawValueType::ENTITY:
        {
            V8Entity* entity = V8Entity::Get(val);
            if(!entity) return false;
            uint16_t id = entity->GetHandle().As<alt::IEntity>()->GetID();
            serializer.WriteRawBytes(&id, sizeof(id));
            break;
        }
        case RawValueType::VECTOR3:
        {
            alt::Vector3f vec;
            if(!V8Helpers::SafeToVector3(val, ctx, vec)) return false;
            float x = vec[0];
            float y = vec[1];
            float z = vec[2];
            serializer.WriteRawBytes(&x, sizeof(float));
            serializer.WriteRawBytes(&y, sizeof(float));
            serializer.WriteRawBytes(&z, sizeof(float));
            break;
        }
        case RawValueType::VECTOR2:
        {
            alt::Vector2f vec;
            if(!V8Helpers::SafeToVector2(val, ctx, vec)) return false;
            float x = vec[0];
            float y = vec[1];
            serializer.WriteRawBytes(&x, sizeof(float));
            serializer.WriteRawBytes(&y, sizeof(float));
            break;
        }
        case RawValueType::RGBA:
        {
            alt::RGBA rgba;
            if(!V8Helpers::SafeToRGBA(val, ctx, rgba)) return false;
            serializer.WriteRawBytes(&rgba.r, sizeof(uint8_t));
            serializer.WriteRawBytes(&rgba.g, sizeof(uint8_t));
            serializer.WriteRawBytes(&rgba.b, sizeof(uint8_t));
            serializer.WriteRawBytes(&rgba.a, sizeof(uint8_t));
            break;
        }
        case RawValueType::ARRAY:
        {
            v8::Local<v8::Array> array = val.As<v8::Array>();
            uint32_t length = array->Length();
            serializer.WriteRawBytes(&length, sizeof(uint32_t));
            for(uint32_t i = 0; i < length; ++i)
            {
                v8::Local<v8::Value> element;
                if(!array->Get(ctx, i).ToLocal(&element)) return false;
                if(!WriteRawValue(ctx, serializer, GetValueType(ctx, element), element)) return false;
            }
            break;
        }
    }
    return true;
}

static inline v8::MaybeLocal<v8::Value> ReadRawValue(v8::Local<v8::Context> ctx, v8::ValueDeserializer& deserializer)
{
    v8::Isolate* isolate = ctx->GetIsolate();
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);

    RawValueType* typePtr;
    if(!deserializer.ReadRawBytes(sizeof(uint8_t), (const void**)&typePtr)) return v8::MaybeLocal<v8::Value>();
    RawValueType type = *typePtr;

    switch(type)
    {
        case RawValueType::GENERIC:
        {
            return deserializer.ReadValue(ctx);
        }
        case RawValueType::ENTITY:
        {
            uint16_t* id;
            if(!deserializer.ReadRawBytes(sizeof(uint16_t), (const void**)&id)) return v8::MaybeLocal<v8::Value>();
            alt::Ref<alt::IEntity> entity = alt::ICore::Instance().GetEntityByID(*id);
            if(!entity) return v8::MaybeLocal<v8::Value>();
            return V8ResourceImpl::Get(ctx)->GetOrCreateEntity(entity.Get(), "Entity")->GetJSVal(isolate);
        }
        case RawValueType::VECTOR3:
        {
            float* x;
            float* y;
            float* z;
            if(!deserializer.ReadRawBytes(sizeof(float), (const void**)&x) || !deserializer.ReadRawBytes(sizeof(float), (const void**)&y) ||
               !deserializer.ReadRawBytes(sizeof(float), (const void**)&z))
                return v8::MaybeLocal<v8::Value>();
            return resource->CreateVector3({ *x, *y, *z });
        }
        case RawValueType::VECTOR2:
        {
            float* x;
            float* y;
            if(!deserializer.ReadRawBytes(sizeof(float), (const void**)&x) || !deserializer.ReadRawBytes(sizeof(float), (const void**)&y)) return v8::MaybeLocal<v8::Value>();
            return resource->CreateVector2({ *x, *y });
        }
        case RawValueType::RGBA:
        {
            uint8_t* r;
            uint8_t* g;
            uint8_t* b;
            uint8_t* a;
            if(!deserializer.ReadRawBytes(sizeof(uint8_t), (const void**)&r) || !deserializer.ReadRawBytes(sizeof(uint8_t), (const void**)&g) ||
               !deserializer.ReadRawBytes(sizeof(uint8_t), (const void**)&b) || !deserializer.ReadRawBytes(sizeof(uint8_t), (const void**)&a))
                return v8::MaybeLocal<v8::Value>();
            return resource->CreateRGBA({ *r, *g, *b, *a });
        }
        case RawValueType::ARRAY:
        {
            uint32_t* lengthPtr;
            if(!deserializer.ReadRawBytes(sizeof(uint32_t), (const void**)&lengthPtr)) return v8::MaybeLocal<v8::Value>();
            uint32_t length = *lengthPtr;
            v8::Local<v8::Array> array = v8::Array::New(isolate, length);
            for(uint32_t i = 0; i < length; ++i)
            {
                v8::MaybeLocal<v8::Value> element = ReadRawValue(ctx, deserializer);
                if(element.IsEmpty()) return v8::MaybeLocal<v8::Value>();
                bool result;
                if(!array->Set(ctx, i, element.ToLocalChecked()).To(&result) && !result) return v8::MaybeLocal<v8::Value>();
            }
            return array;
        }
        default:
        {
            Log::Error << "Reading unknown raw value type: " << (int)type << Log::Endl;
            break;
        }
    }
    return v8::MaybeLocal<v8::Value>();
}

// Converts a JS value to a MValue byte array
alt::MValueByteArray V8Helpers::V8ToRawBytes(v8::Local<v8::Value> val)
{
    auto start = std::chrono::high_resolution_clock::now();
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    RawValueType type = GetValueType(ctx, val);
    if(type == RawValueType::INVALID) return alt::MValueByteArray();

    v8::ValueSerializer serializer(isolate);
    serializer.WriteHeader();

    // Write the magic bytes to the buffer
    serializer.WriteRawBytes(magicBytes, sizeof(magicBytes));

    // Write the serialized value to the buffer
    bool result = WriteRawValue(ctx, serializer, type, val);
    if(!result) return alt::MValueByteArray();

    std::pair<uint8_t*, size_t> serialized = serializer.Release();
    return alt::ICore::Instance().CreateMValueByteArray(serialized.first, serialized.second);
}

// Converts a MValue byte array to a JS value
v8::MaybeLocal<v8::Value> V8Helpers::RawBytesToV8(alt::MValueByteArrayConst rawBytes)
{
    const uint8_t* data = rawBytes->GetData();
    size_t size = rawBytes->GetSize();
    if(size < sizeof(magicBytes)) return v8::MaybeLocal<v8::Value>();

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    v8::ValueDeserializer deserializer(isolate, data, size);

    bool headerValid;
    if(!deserializer.ReadHeader(ctx).To(&headerValid) || !headerValid) return v8::MaybeLocal<v8::Value>();

    // Check for magic bytes
    uint8_t* magicBytesPtr;
    if(!deserializer.ReadRawBytes(sizeof(magicBytes), (const void**)&magicBytesPtr)) return v8::MaybeLocal<v8::Value>();
    if(memcmp(magicBytesPtr, magicBytes, sizeof(magicBytes)) != 0) return v8::MaybeLocal<v8::Value>();

    // Deserialize the value
    v8::MaybeLocal<v8::Value> result = ReadRawValue(ctx, deserializer);

    return result;
}
