#include "Serialization.h"
#include "V8ResourceImpl.h"

// Magic bytes to identify raw JS value buffers
static uint8_t magicBytes[] = { 'J', 'S', 'V', 'a', 'l' };

enum class RawValueType : uint8_t
{
    GENERIC,
    ENTITY,
    VECTOR3,
    VECTOR2,
    RGBA,
    INVALID
};

extern V8Class v8Entity;
static inline RawValueType GetValueType(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val)
{
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
    bool result;
    if(val->InstanceOf(ctx, v8Entity.JSValue(ctx->GetIsolate(), ctx)).To(&result) && result) return RawValueType::ENTITY;
    if(resource->IsVector3(val)) return RawValueType::VECTOR3;
    if(resource->IsVector2(val)) return RawValueType::VECTOR2;
    if(resource->IsRGBA(val)) return RawValueType::RGBA;
    if(val->IsSharedArrayBuffer() || val->IsFunction()) return RawValueType::INVALID;
    else
        return RawValueType::GENERIC;
}

// Converts a JS value to a MValue byte array
alt::MValueByteArray V8Helpers::V8ToRawBytes(v8::Local<v8::Value> val)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();
    std::vector<uint8_t> bytes;

    RawValueType type = GetValueType(ctx, val);
    if(type == RawValueType::INVALID) return alt::MValueByteArray();

    v8::ValueSerializer serializer(isolate);
    serializer.WriteHeader();
    serializer.WriteRawBytes(&type, sizeof(uint8_t));

    switch(type)
    {
        case RawValueType::GENERIC:
        {
            bool result;
            if(!serializer.WriteValue(ctx, val).To(&result) || !result) return alt::MValueByteArray();
            break;
        }
        case RawValueType::ENTITY:
        {
            V8Entity* entity = V8Entity::Get(val);
            if(!entity) return alt::MValueByteArray();
            uint16_t id = entity->GetHandle().As<alt::IEntity>()->GetID();
            serializer.WriteRawBytes(&id, sizeof(id));
            break;
        }
        case RawValueType::VECTOR3:
        {
            alt::Vector3f vec;
            if(!V8::SafeToVector3(val, ctx, vec)) return alt::MValueByteArray();
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
            if(!V8::SafeToVector2(val, ctx, vec)) return alt::MValueByteArray();
            float x = vec[0];
            float y = vec[1];
            serializer.WriteRawBytes(&x, sizeof(float));
            serializer.WriteRawBytes(&y, sizeof(float));
            break;
        }
        case RawValueType::RGBA:
        {
            alt::RGBA rgba;
            if(!V8::SafeToRGBA(val, ctx, rgba)) return alt::MValueByteArray();
            serializer.WriteRawBytes(&rgba.r, sizeof(uint8_t));
            serializer.WriteRawBytes(&rgba.g, sizeof(uint8_t));
            serializer.WriteRawBytes(&rgba.b, sizeof(uint8_t));
            serializer.WriteRawBytes(&rgba.a, sizeof(uint8_t));
            break;
        }
    }

    std::pair<uint8_t*, size_t> serialized = serializer.Release();

    // Write the serialized value to the buffer
    bytes.assign(serialized.first, serialized.first + serialized.second);

    // Reserve size for the magic bytes
    bytes.reserve(bytes.size() + sizeof(magicBytes));

    // Write the magic bytes to the front of the buffer
    for(size_t i = 0; i < sizeof(magicBytes); ++i) bytes.insert(bytes.begin() + i, magicBytes[i]);

    // Copy the data, because it gets freed by the std::vector when this scope ends,
    // and the MValue byte array does not copy the data
    uint8_t* data = new uint8_t[bytes.size()];
    std::memcpy(data, bytes.data(), bytes.size());

    return alt::ICore::Instance().CreateMValueByteArray(data, bytes.size());
}

// Converts a MValue byte array to a JS value
v8::MaybeLocal<v8::Value> V8Helpers::RawBytesToV8(alt::MValueByteArrayConst rawBytes)
{
    // We copy the data here, because the std::vector frees the data when it goes out of scope
    uint8_t* data = new uint8_t[rawBytes->GetSize()];
    std::memcpy(data, rawBytes->GetData(), rawBytes->GetSize());
    std::vector<uint8_t> bytes(data, data + rawBytes->GetSize());

    // Check for magic bytes
    if(bytes.size() < sizeof(magicBytes)) return v8::MaybeLocal<v8::Value>();
    for(size_t i = 0; i < sizeof(magicBytes); ++i)
        if(bytes[i] != magicBytes[i]) return v8::MaybeLocal<v8::Value>();

    // Remove the magic bytes from the byte array
    bytes.erase(bytes.begin(), bytes.begin() + sizeof(magicBytes));

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

    v8::ValueDeserializer deserializer(isolate, bytes.data(), bytes.size());
    bool headerValid;
    if(!deserializer.ReadHeader(ctx).To(&headerValid) || !headerValid) return v8::MaybeLocal<v8::Value>();
    RawValueType* typePtr;
    if(!deserializer.ReadRawBytes(sizeof(uint8_t), (const void**)&typePtr)) return v8::MaybeLocal<v8::Value>();
    RawValueType type = *typePtr;

    // Deserialize the value
    V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);
    v8::MaybeLocal<v8::Value> result;
    switch(type)
    {
        case RawValueType::GENERIC:
        {
            result = deserializer.ReadValue(ctx);
            break;
        }
        case RawValueType::ENTITY:
        {
            uint16_t* id;
            if(!deserializer.ReadRawBytes(sizeof(uint16_t), (const void**)&id)) return v8::MaybeLocal<v8::Value>();
            alt::Ref<alt::IEntity> entity = alt::ICore::Instance().GetEntityByID(*id);
            if(!entity) return v8::MaybeLocal<v8::Value>();
            result = V8ResourceImpl::Get(ctx)->GetOrCreateEntity(entity.Get(), "Entity")->GetJSVal(isolate);
            break;
        }
        case RawValueType::VECTOR3:
        {
            float* x;
            float* y;
            float* z;
            if(!deserializer.ReadRawBytes(sizeof(float), (const void**)&x) || !deserializer.ReadRawBytes(sizeof(float), (const void**)&y) ||
               !deserializer.ReadRawBytes(sizeof(float), (const void**)&z))
                return v8::MaybeLocal<v8::Value>();
            result = resource->CreateVector3({ *x, *y, *z });
            break;
        }
        case RawValueType::VECTOR2:
        {
            float* x;
            float* y;
            if(!deserializer.ReadRawBytes(sizeof(float), (const void**)&x) || !deserializer.ReadRawBytes(sizeof(float), (const void**)&y)) return v8::MaybeLocal<v8::Value>();
            result = resource->CreateVector2({ *x, *y });
            break;
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
            result = resource->CreateRGBA({ *r, *g, *b, *a });
            break;
        }
    }

    return result;
}
