#pragma once

#include "v8.h"
#include "cpp-sdk/ICore.h"

#define V8_GET_ISOLATE() v8::Isolate* isolate = info.GetIsolate()
#define V8_GET_CONTEXT() v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext()
#define V8_GET_ISOLATE_CONTEXT() \
    V8_GET_ISOLATE();            \
    V8_GET_CONTEXT()

#define V8_GET_RESOURCE()                                                                    \
    V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredOrMicrotaskContext()); \
    V8_CHECK(resource, "invalid resource");

#define V8_GET_IRESOURCE()                                                                           \
    alt::IResource* resource = V8ResourceImpl::GetResource(isolate->GetEnteredOrMicrotaskContext()); \
    V8_CHECK(resource, "invalid resource");

#define V8_GET_ISOLATE_CONTEXT_RESOURCE() \
    V8_GET_ISOLATE_CONTEXT();             \
    V8_GET_RESOURCE()

#define V8_GET_ISOLATE_CONTEXT_IRESOURCE() \
    V8_GET_ISOLATE_CONTEXT();              \
    V8_GET_IRESOURCE()

#define V8_CHECK_NORETN(a, b)           \
    if(!(a))                            \
    {                                   \
        V8Helpers::Throw(isolate, (b)); \
    }

#define V8_CHECK_RETN(a, b, c)          \
    if(!(a))                            \
    {                                   \
        V8Helpers::Throw(isolate, (b)); \
        return c;                       \
    }
#define V8_CHECK(a, b) V8_CHECK_RETN(a, b, )

#define V8_GET_THIS_BASE_OBJECT(val, type)                 \
    type* val;                                             \
    {                                                      \
        V8Entity* __val = V8Entity::Get(info.This());      \
        V8_CHECK(__val, "baseobject is invalid");          \
        val = dynamic_cast<type*>(__val->GetHandle());     \
        V8_CHECK(val, "baseobject is not of type " #type); \
    }

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_OBJECT(idx, val)                                                                         \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = info.This()->GetInternalField((idx)-1)->ToObject(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_V8ENTITY(idx, val)                                                                       \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = V8Entity::Get(info.This()->GetInternalField((idx)-1)->ToObject(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked());

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_ENTITY(idx, val, type)                                                                   \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = dynamic_cast<type*>(V8Entity::Get(info.This()->GetInternalField((idx)-1)->ToObject(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked())->GetHandle());

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_INTEGER(idx, val)                                                                        \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = info.This()->GetInternalField((idx)-1)->IntegerValue(ctx).ToChecked();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_UINT32(idx, val)                                                                         \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = info.This()->GetInternalField((idx)-1)->Uint32Value(ctx).ToChecked();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(idx, val, type)                                                                 \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = static_cast<type*>(info.This()->GetInternalField((idx)-1).As<v8::External>()->Value());

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_PTR(idx, val, type)                                                                      \
    V8_CHECK(info.This()->InternalFieldCount() > idx - 1, "Invalid internal field count (is the 'this' context correct?)"); \
    auto val = static_cast<type*>(info.This()->GetAlignedPointerFromInternalField((idx)-1));

#define V8_CHECK_CONSTRUCTOR() V8_CHECK(info.IsConstructCall(), "function can't be called without new")

#define V8_CHECK_ARGS_LEN(count)                  V8_CHECK(info.Length() == (count), #count " arguments expected")
#define V8_CHECK_ARGS_LEN2(count1, count2)        V8_CHECK(info.Length() == (count1) || info.Length() == (count2), #count1 " or " #count2 " arguments expected")
#define V8_CHECK_ARGS_LEN_MIN_MAX(count1, count2) V8_CHECK(info.Length() >= (count1) && info.Length() <= (count2), "Minimum " #count1 ", maximum " #count2 " arguments expected")
#define V8_CHECK_ARGS_LEN_MIN(count)              V8_CHECK(info.Length() >= (count), "Minimum " #count " arguments expected")

#define V8_TO_BOOLEAN(v8Val, val) \
    bool val;                     \
    V8_CHECK(V8Helpers::SafeToBoolean((v8Val), isolate, val), "Failed to convert value to boolean")

#define V8_TO_NUMBER(v8Val, val) \
    double val;                  \
    V8_CHECK(V8Helpers::SafeToNumber((v8Val), ctx, val), "Failed to convert value to number")

#define V8_TO_INTEGER(v8Val, val) \
    int64_t val;                  \
    V8_CHECK(V8Helpers::SafeToInteger((v8Val), ctx, val), "Failed to convert value to integer")

#define V8_TO_INT32(v8Val, val) \
    int32_t val;                \
    V8_CHECK(V8Helpers::SafeToInt32((v8Val), ctx, val), "Failed to convert value to integer")

#define V8_TO_UINT(v8Val, val) \
    uint32_t val;              \
    V8_CHECK(V8Helpers::SafeToUInt32((v8Val), ctx, val), "Failed to convert value to uint")

#define V8_TO_STRING(v8Val, val) \
    std::string val;             \
    V8_CHECK(V8Helpers::SafeToString((v8Val), isolate, ctx, val), "Failed to convert value to string")

#define V8_TO_OBJECT(v8Val, val) \
    v8::Local<v8::Object> val;   \
    V8_CHECK(V8Helpers::SafeToObject((v8Val), ctx, val), "Failed to convert value to object")

#define V8_TO_VECTOR3(v8Val, val) \
    alt::Vector3f val;            \
    V8_CHECK(V8Helpers::SafeToVector3((v8Val), ctx, val), "Failed to convert value to Vector3")

#define V8_TO_VECTOR3_INT(v8Val, val) \
    alt::Vector3i val;                \
    V8_CHECK(V8Helpers::SafeToVector3Int((v8Val), ctx, val), "Failed to convert value to Vector3")

#define V8_TO_VECTOR2(v8Val, val) \
    alt::Vector2f val;            \
    V8_CHECK(V8Helpers::SafeToVector2((v8Val), ctx, val), "Failed to convert value to Vector2")

#define V8_TO_VECTOR2_INT(v8Val, val) \
    alt::Vector2i val;                \
    V8_CHECK(V8Helpers::SafeToVector2Int((v8Val), ctx, val), "Failed to convert value to Vector2")

#define V8_TO_RGBA(v8Val, val) \
    alt::RGBA val;             \
    V8_CHECK(V8Helpers::SafeToRGBA((v8Val), ctx, val), "Failed to convert value to RGBA")

#define V8_TO_ENTITY(v8Val, val) \
    alt::IEntity* val;           \
    V8_CHECK(V8Helpers::SafeToBaseObject<IEntity>(v8Val, isolate, val), "Failed to convert to BaseObject")

#define V8_OBJECT_GET_NUMBER(v8Val, prop, val) V8_TO_NUMBER((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_NUMBER(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Number::New(isolate, val));

#define V8_OBJECT_GET_INT(v8Val, prop, val) V8_TO_INTEGER((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_INT(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Integer::New(isolate, val));

#define V8_OBJECT_SET_UINT(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, val));

#define V8_OBJECT_GET_BOOLEAN(v8Val, prop, val) V8_TO_BOOLEAN((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_BOOLEAN(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Boolean::New(isolate, val));

#define V8_OBJECT_GET_STRING(v8Val, prop, val) V8_TO_STRING((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_BIGINT(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::BigInt::New(isolate, val));

#define V8_OBJECT_SET_BIGUINT(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::BigInt::NewFromUnsigned(isolate, val));

#define V8_OBJECT_SET_STRING(v8Val, prop, val) \
    if(!val.empty()) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::String::NewFromUtf8(isolate, val.c_str()).ToLocalChecked());

#define V8_OBJECT_SET_RAW_STRING(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::String::NewFromUtf8(isolate, val).ToLocalChecked());

#define V8_NEW_OBJECT(val) v8::Local<v8::Object> val = v8::Object::New(isolate);

#define V8_NEW_ARGS(val) std::vector<v8::Local<v8::Value>> val;

#define V8_ADD_ARG(args, val) (args).push_back(val);

// idx starts with 1
#define V8_ARG_CHECK_NUMBER(idx) V8_CHECK(info[(idx)-1]->IsNumber(), "Argument " #idx " must be a number")

// idx starts with 1
#define V8_ARG_TO_BOOLEAN(idx, val) \
    bool val;                       \
    V8_CHECK(V8Helpers::SafeToBoolean(info[(idx)-1], isolate, val), "Failed to convert argument " #idx " to boolean")

// idx starts with 1
#define V8_ARG_TO_BOOLEAN_OPT(idx, val, defaultVal)                                                                        \
    bool val;                                                                                                              \
    if(info.Length() >= (idx))                                                                                             \
    {                                                                                                                      \
        V8_CHECK(V8Helpers::SafeToBoolean(info[(idx)-1], isolate, val), "Failed to convert argument " #idx " to boolean"); \
    }                                                                                                                      \
    else                                                                                                                   \
    {                                                                                                                      \
        val = defaultVal;                                                                                                  \
    }

// idx starts with 1
#define V8_ARG_TO_MVALUE(idx, val) alt::MValue val = V8Helpers::V8ToMValue(info[(idx)-1]);

// idx starts with 1
#define V8_ARG_TO_INT(idx, val) \
    int64_t val;                \
    V8_CHECK(V8Helpers::SafeToInteger(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to integer")

// idx starts with 1
#define V8_ARG_TO_NUMBER(idx, val) \
    double val;                    \
    V8_CHECK(V8Helpers::SafeToNumber(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to number")

// idx starts with 1
#define V8_ARG_TO_STRING(idx, val) \
    std::string val;               \
    V8_CHECK(V8Helpers::SafeToString(info[(idx)-1], isolate, ctx, val), "Failed to convert argument " #idx " to string")

// idx starts with 1
#define V8_ARG_TO_FUNCTION(idx, val) \
    v8::Local<v8::Function> val;     \
    V8_CHECK(V8Helpers::SafeToFunction(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to function")

// idx starts with 1
#define V8_ARG_TO_OBJECT(idx, val) \
    v8::Local<v8::Object> val;     \
    V8_CHECK(V8Helpers::SafeToObject(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to object")

// idx starts with 1
#define V8_ARG_TO_BASE_OBJECT(idx, val, type, jsClassName) \
    type* val;                                             \
    V8_CHECK(V8Helpers::SafeToBaseObject<type>(info[(idx)-1], isolate, val), "Argument " #idx " must be a " jsClassName)

// idx starts with 1
#define V8_ARG_TO_ARRAY_BUFFER(idx, val) \
    v8::Local<v8::ArrayBuffer> val;      \
    V8_CHECK(V8Helpers::SafeToArrayBuffer(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to ArrayBuffer")

// idx starts with 1
#define V8_ARG_TO_ARRAY_BUFFER_VIEW(idx, val) \
    v8::Local<v8::ArrayBufferView> val;       \
    V8_CHECK(V8Helpers::SafeToArrayBufferView(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to ArrayBufferView")

#define V8_ARG_TO_ARRAY(idx, val) \
    v8::Local<v8::Array> val;     \
    V8_CHECK(V8Helpers::SafeToArray(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to Array")

// idx starts with 1
#define V8_ARG_TO_UINT64(idx, val) \
    uint64_t val;                  \
    V8_CHECK(V8Helpers::SafeToUInt64(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to uint64")

// idx starts with 1
#define V8_ARG_TO_INT64(idx, val) \
    int64_t val;                  \
    V8_CHECK(V8Helpers::SafeToInt64(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to int64")

// idx starts with 1
#define V8_ARG_TO_UINT(idx, val) \
    uint32_t val;                \
    V8_CHECK(V8Helpers::SafeToUInt32(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to uint32")

// idx starts with 1
#define V8_ARG_TO_INT32(idx, val) \
    int32_t val;                  \
    V8_CHECK(V8Helpers::SafeToInt32(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to int32")

// idx starts with 1
#define V8_ARG_TO_VECTOR3(idx, val) \
    alt::Vector3f val;              \
    V8_CHECK(V8Helpers::SafeToVector3(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to vector3")

// idx starts with 1
#define V8_ARG_TO_VECTOR2(idx, val) \
    alt::Vector2f val;              \
    V8_CHECK(V8Helpers::SafeToVector2(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to vector2")

// idx starts with 1
#define V8_ARG_TO_RGBA(idx, val) \
    alt::RGBA val;               \
    V8_CHECK(V8Helpers::SafeToRGBA(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to rgba")

#define V8_GET_DATA(type, val) auto val = static_cast<type*>(info.Data().As<v8::External>()->Value());

#define V8_RETURN(val)            info.GetReturnValue().Set(val)
#define V8_RETURN_NULL()          info.GetReturnValue().SetNull()
#define V8_RETURN_BOOLEAN(val)    V8_RETURN(val)
#define V8_RETURN_INT(val)        V8_RETURN(static_cast<int32_t>(val))
#define V8_RETURN_UINT(val)       V8_RETURN(static_cast<uint32_t>(val))
#define V8_RETURN_NUMBER(val)     V8_RETURN(static_cast<double>(val))
#define V8_RETURN_STRING(val)     V8_RETURN(v8::String::NewFromUtf8(isolate, (val).c_str(), v8::NewStringType::kNormal).ToLocalChecked())
#define V8_RETURN_RAW_STRING(val) V8_RETURN(v8::String::NewFromUtf8(isolate, (val), v8::NewStringType::kNormal).ToLocalChecked())
#define V8_RETURN_MVALUE(val)     V8_RETURN(V8Helpers::MValueToV8(val))
#define V8_RETURN_UINT64(val)     V8_RETURN(v8::BigInt::NewFromUnsigned(isolate, static_cast<uint64_t>(val)))
#define V8_RETURN_INT64(val)      V8_RETURN(v8::BigInt::New(isolate, static_cast<int64_t>(val)))
#define V8_RETURN_VECTOR3(val)    V8_RETURN(resource->CreateVector3(val))
#define V8_RETURN_VECTOR2(val)    V8_RETURN(resource->CreateVector2(val))
#define V8_RETURN_RGBA(val)       V8_RETURN(resource->CreateRGBA(val))
#define V8_RETURN_ENUM(val)       V8_RETURN(uint32_t(val))

#define V8_RETURN_BASE_OBJECT(baseObjectRef) V8_RETURN(resource->GetBaseObjectOrNull(baseObjectRef))

#define V8_BIND_BASE_OBJECT(baseObjectRef, reason)        \
    {                                                     \
        V8_CHECK(baseObjectRef, reason);                  \
        resource->BindEntity(info.This(), baseObjectRef); \
    }

#define V8_EVENT_HANDLER       extern V8Helpers::EventHandler
#define V8_LOCAL_EVENT_HANDLER extern V8Helpers::LocalEventHandler
#define V8_REFERENCE_EVENT_HANDLER(name) \
    V8_EVENT_HANDLER name;               \
    name.Reference();
#define V8_REFERENCE_LOCAL_EVENT_HANDLER(name) \
    V8_LOCAL_EVENT_HANDLER name;               \
    name.Reference();

#define V8_DEPRECATE(oldName, newName) Log::Warning << oldName << " is deprecated and will be removed in future versions. Consider using " << newName << " instead" << Log::Endl;
