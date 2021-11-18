#pragma once

#include <vector>
#include <functional>

#include <v8.h>
#include <limits>

#include "cpp-sdk/objects/IEntity.h"
#include "cpp-sdk/types/MValue.h"
#include "V8Entity.h"

namespace V8Helpers
{
    inline void Throw(v8::Isolate* isolate, const std::string& msg)
    {
        isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, msg.data(), v8::NewStringType::kNormal, msg.size()).ToLocalChecked()));
    }

    bool TryCatch(const std::function<bool()>& fn);

    struct HashFunc
    {
        size_t operator()(v8::Local<v8::Function> fn) const
        {
            return fn->GetIdentityHash();
        }
    };

    struct EqFunc
    {
        size_t operator()(v8::Local<v8::Function> lhs, v8::Local<v8::Function> rhs) const
        {
            return lhs->StrictEquals(rhs);
        }
    };

    class Binding
    {
    public:
        using Callback = std::function<void(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)>;

        Binding(Callback&& fn)
        {
            All().push_back(std::move(fn));
        }

        static std::vector<Callback>& All()
        {
            static std::vector<Callback> _All;
            return _All;
        }

        static void RegisterAll(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)
        {
            for(auto& binding : All()) binding(ctx, exports);
        }
    };

    void RegisterFunc(v8::Local<v8::Object> exports, const std::string& _name, v8::FunctionCallback cb, void* data = nullptr);

    void FunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

    alt::MValue V8ToMValue(v8::Local<v8::Value> val, bool allowFunction = true);

    v8::Local<v8::Value> MValueToV8(alt::MValueConst val);

    void MValueArgsToV8(alt::MValueArgs args, std::vector<v8::Local<v8::Value>>& v8Args);

    void SetAccessor(v8::Local<v8::Template> tpl, v8::Isolate* isolate, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

    alt::MValueByteArray V8ToRawBytes(v8::Local<v8::Value> val);
    v8::MaybeLocal<v8::Value> RawBytesToV8(alt::MValueByteArrayConst bytes);

};  // namespace V8Helpers

class V8ResourceImpl;

namespace V8
{
    template<typename T>
    using CPersistent = v8::Persistent<T, v8::CopyablePersistentTraits<T>>;

    class SourceLocation
    {
    public:
        SourceLocation(std::string&& fileName, int line, v8::Local<v8::Context> ctx);

        const std::string& GetFileName() const
        {
            return fileName;
        }
        int GetLineNumber() const
        {
            return line;
        }

        std::string ToString();

        static SourceLocation GetCurrent(v8::Isolate* isolate);

    private:
        CPersistent<v8::Context> context;
        std::string fileName;
        int line = 0;
    };

    struct EventCallback
    {
        v8::UniquePersistent<v8::Function> fn;
        SourceLocation location;
        bool removed = false;
        bool once;

        EventCallback(v8::Isolate* isolate, v8::Local<v8::Function> _fn, SourceLocation&& location, bool once = false) : fn(isolate, _fn), location(std::move(location)), once(once) {}
    };

    class EventHandler
    {
    public:
        using CallbacksGetter = std::function<std::vector<EventCallback*>(V8ResourceImpl* resource, const alt::CEvent*)>;
        using ArgsGetter = std::function<void(V8ResourceImpl* resource, const alt::CEvent*, std::vector<v8::Local<v8::Value>>& args)>;

        EventHandler(alt::CEvent::Type type, CallbacksGetter&& _handlersGetter, ArgsGetter&& _argsGetter);

        // Temp issue fix for https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
        void Reference();

        std::vector<V8::EventCallback*> GetCallbacks(V8ResourceImpl* impl, const alt::CEvent* e);
        std::vector<v8::Local<v8::Value>> GetArgs(V8ResourceImpl* impl, const alt::CEvent* e);

        static EventHandler* Get(const alt::CEvent* e);

    private:
        alt::CEvent::Type type;
        CallbacksGetter callbacksGetter;
        ArgsGetter argsGetter;

        static std::unordered_map<alt::CEvent::Type, EventHandler*>& all()
        {
            static std::unordered_map<alt::CEvent::Type, EventHandler*> _all;
            return _all;
        }

        static void Register(alt::CEvent::Type type, EventHandler* handler);
    };

    class LocalEventHandler : public EventHandler
    {
    public:
        LocalEventHandler(alt::CEvent::Type type, const std::string& name, ArgsGetter&& argsGetter) : EventHandler(type, std::move(GetCallbacksGetter(name)), std::move(argsGetter)) {}

    private:
        static CallbacksGetter GetCallbacksGetter(const std::string& name);
    };

    void DefineOwnProperty(v8::Isolate* isolate,
                           v8::Local<v8::Context> ctx,
                           v8::Local<v8::Object> val,
                           const char* name,
                           v8::Local<v8::Value> value,
                           v8::PropertyAttribute attributes = v8::PropertyAttribute::None);

    void DefineOwnProperty(v8::Isolate* isolate,
                           v8::Local<v8::Context> ctx,
                           v8::Local<v8::Object> val,
                           v8::Local<v8::String> name,
                           v8::Local<v8::Value> value,
                           v8::PropertyAttribute attributes = v8::PropertyAttribute::None);

    void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

    void SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback);

    void SetStaticAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

    void SetStaticMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name, v8::FunctionCallback callback);

    void SetFunction(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> target, const char* name, v8::FunctionCallback cb, void* userData = nullptr);

    v8::Local<v8::Value> Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, const char* name);
    v8::Local<v8::Value> Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, v8::Local<v8::Name> name);

    v8::Local<v8::Value> New(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Function> constructor, std::vector<v8::Local<v8::Value>>& args);

    // TODO: create c++ classes for v8 classes and move there
    v8::Local<v8::String> Vector3_XKey(v8::Isolate* isolate);
    v8::Local<v8::String> Vector3_YKey(v8::Isolate* isolate);
    v8::Local<v8::String> Vector3_ZKey(v8::Isolate* isolate);

    v8::Local<v8::String> RGBA_RKey(v8::Isolate* isolate);
    v8::Local<v8::String> RGBA_GKey(v8::Isolate* isolate);
    v8::Local<v8::String> RGBA_BKey(v8::Isolate* isolate);
    v8::Local<v8::String> RGBA_AKey(v8::Isolate* isolate);

    v8::Local<v8::String> Fire_PosKey(v8::Isolate* isolate);
    v8::Local<v8::String> Fire_WeaponKey(v8::Isolate* isolate);

    bool SafeToBoolean(v8::Local<v8::Value> val, v8::Isolate* isolate, bool& out);
    bool SafeToInteger(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, int64_t& out);
    bool SafeToNumber(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, double& out);
    bool SafeToString(v8::Local<v8::Value> val, v8::Isolate* isolate, v8::Local<v8::Context> ctx, alt::String& out);
    bool SafeToFunction(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Function>& out);
    bool SafeToObject(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, v8::Local<v8::Object>& out);
    bool SafeToRGBA(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::RGBA& out);
    bool SafeToVector3(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector3f& out);
    bool SafeToVector2(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, alt::Vector2f& out);
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

    std::string Stringify(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val);
    alt::String GetJSValueTypeName(v8::Local<v8::Value> val);

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
    inline v8::Local<v8::String> JSValue(alt::String val)
    {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), val.GetData(), v8::NewStringType::kNormal, (int)val.GetSize()).ToLocalChecked();
    }
    inline v8::Local<v8::String> JSValue(alt::StringView val)
    {
        return v8::String::NewFromUtf8(v8::Isolate::GetCurrent(), val.GetData(), v8::NewStringType::kNormal, (int)val.GetSize()).ToLocalChecked();
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

    inline std::string GetStackTrace(const std::string errorMsg)
    {
        auto isolate = v8::Isolate::GetCurrent();
        auto ctx = isolate->GetEnteredOrMicrotaskContext();

        auto exception = v8::Exception::Error(JSValue(errorMsg));
        auto stackTrace = v8::TryCatch::StackTrace(ctx, exception).ToLocalChecked();
        return *v8::String::Utf8Value(isolate, stackTrace);
    }

    inline std::string GetCurrentSourceOrigin(v8::Isolate* isolate)
    {
        auto stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 1);
        if(stackTrace->GetFrameCount() == 0) return "";
        return *v8::String::Utf8Value(isolate, stackTrace->GetFrame(isolate, 0)->GetScriptName());
    }

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

            Value() : data(nullptr), size(0), ownPtr(false) {}
            Value(uint8_t* data, size_t size, bool ownPtr = true) : data(data), size(size), ownPtr(ownPtr) {}
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

}  // namespace V8

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

#define V8_CHECK_RETN(a, b, c)          \
    if(!(a))                            \
    {                                   \
        V8Helpers::Throw(isolate, (b)); \
        return c;                       \
    }
#define V8_CHECK(a, b) V8_CHECK_RETN(a, b, )

#define V8_GET_THIS_BASE_OBJECT(val, type)                 \
    ::alt::Ref<type> val;                                  \
    {                                                      \
        V8Entity* __val = V8Entity::Get(info.This());      \
        V8_CHECK(__val, "baseobject is invalid");          \
        val = __val->GetHandle().As<type>();               \
        V8_CHECK(val, "baseobject is not of type " #type); \
    }

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_OBJECT(idx, val) auto val = info.This()->GetInternalField((idx)-1)->ToObject(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_V8ENTITY(idx, val) auto val = V8Entity::Get(info.This()->GetInternalField((idx)-1)->ToObject(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked());

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_ENTITY(idx, val, type) \
    auto val = V8Entity::Get(info.This()->GetInternalField((idx)-1)->ToObject(isolate->GetEnteredOrMicrotaskContext()).ToLocalChecked())->GetHandle().As<type>();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_INTEGER(idx, val) auto val = info.This()->GetInternalField((idx)-1)->IntegerValue(ctx).ToChecked();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_UINT32(idx, val) auto val = info.This()->GetInternalField((idx)-1)->Uint32Value(ctx).ToChecked();

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(idx, val, type) auto val = static_cast<type*>(info.This()->GetInternalField((idx)-1).As<v8::External>()->Value());

// idx starts with 1
#define V8_GET_THIS_INTERNAL_FIELD_PTR(idx, val, type) auto val = static_cast<type*>(info.This()->GetAlignedPointerFromInternalField((idx)-1));

#define V8_CHECK_CONSTRUCTOR() V8_CHECK(info.IsConstructCall(), "function can't be called without new")

#define V8_CHECK_ARGS_LEN(count)                  V8_CHECK(info.Length() == (count), #count " arguments expected")
#define V8_CHECK_ARGS_LEN2(count1, count2)        V8_CHECK(info.Length() == (count1) || info.Length() == (count2), #count1 " or " #count2 " arguments expected")
#define V8_CHECK_ARGS_LEN_MIN_MAX(count1, count2) V8_CHECK(info.Length() >= (count1) && info.Length() <= (count2), "Minimum " #count1 ", maximum " #count2 " arguments expected")
#define V8_CHECK_ARGS_LEN_MIN(count)              V8_CHECK(info.Length() >= (count), "Minimum " #count " arguments expected")

#define V8_TO_BOOLEAN(v8Val, val) \
    bool val;                     \
    V8_CHECK(V8::SafeToBoolean((v8Val), isolate, val), "Failed to convert value to boolean")

#define V8_TO_NUMBER(v8Val, val) \
    double val;                  \
    V8_CHECK(V8::SafeToNumber((v8Val), ctx, val), "Failed to convert value to number")

#define V8_TO_INTEGER(v8Val, val) \
    int64_t val;                  \
    V8_CHECK(V8::SafeToInteger((v8Val), ctx, val), "Failed to convert value to integer")

#define V8_TO_INT32(v8Val, val) \
    int32_t val;                \
    V8_CHECK(V8::SafeToInt32((v8Val), ctx, val), "Failed to convert value to integer")

#define V8_TO_STRING(v8Val, val) \
    alt::String val;             \
    V8_CHECK(V8::SafeToString((v8Val), isolate, ctx, val), "Failed to convert value to string")

#define V8_TO_OBJECT(v8Val, val) \
    v8::Local<v8::Object> val;   \
    V8_CHECK(V8::SafeToObject((v8Val), ctx, val), "Failed to convert value to object")

#define V8_TO_VECTOR3(v8Val, val) \
    alt::Vector3f val;            \
    V8_CHECK(V8::SafeToVector3((v8Val), ctx, val), "Failed to convert value to Vector3")

#define V8_TO_VECTOR2(v8Val, val) \
    alt::Vector2f val;            \
    V8_CHECK(V8::SafeToVector2((v8Val), ctx, val), "Failed to convert value to Vector2")

#define V8_TO_RGBA(v8Val, val) \
    alt::RGBA val;             \
    V8_CHECK(V8::SafeToRGBA((v8Val), ctx, val), "Failed to convert value to RGBA")

#define V8_TO_ENTITY(v8Val, val) \
    alt::Ref<IEntity> val;       \
    V8_CHECK(V8::SafeToBaseObject<IEntity>(v8Val, isolate, val), "Failed to convert to BaseObject")

#define V8_OBJECT_GET_NUMBER(v8Val, prop, val) V8_TO_NUMBER((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_NUMBER(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Number::New(isolate, val));

#define V8_OBJECT_GET_INT(v8Val, prop, val) V8_TO_INTEGER((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_INT(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Integer::New(isolate, val));

#define V8_OBJECT_SET_UINT(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Integer::NewFromUnsigned(isolate, val));

#define V8_OBJECT_GET_BOOLEAN(v8Val, prop, val) V8_TO_BOOLEAN((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_BOOLEAN(v8Val, prop, val) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::Boolean::New(isolate, val));

#define V8_OBJECT_GET_STRING(v8Val, prop, val) V8_TO_STRING((v8Val)->Get(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked()).ToLocalChecked(), val)

#define V8_OBJECT_SET_STRING(v8Val, prop, val) \
    if(!val.IsEmpty()) (v8Val)->Set(ctx, v8::String::NewFromUtf8(isolate, prop).ToLocalChecked(), v8::String::NewFromUtf8(isolate, val.CStr()).ToLocalChecked());

#define V8_NEW_STRING(val) v8::String::NewFromUtf8(isolate, val).ToLocalChecked()

#define V8_NEW_OBJECT(val) v8::Local<v8::Object> val = v8::Object::New(isolate);

#define V8_NEW_ARGS(val) std::vector<v8::Local<v8::Value>> val;

#define V8_ADD_ARG(args, val) (args).push_back(val);

// idx starts with 1
#define V8_ARG_CHECK_NUMBER(idx) V8_CHECK(info[(idx)-1]->IsNumber(), "Argument " #idx " must be a number")

// idx starts with 1
#define V8_ARG_TO_BOOLEAN(idx, val) \
    bool val;                       \
    V8_CHECK(V8::SafeToBoolean(info[(idx)-1], isolate, val), "Failed to convert argument " #idx " to boolean")

// idx starts with 1
#define V8_ARG_TO_BOOLEAN_OPT(idx, val, defaultVal)                                                                 \
    bool val;                                                                                                       \
    if(info.Length() >= (idx))                                                                                      \
    {                                                                                                               \
        V8_CHECK(V8::SafeToBoolean(info[(idx)-1], isolate, val), "Failed to convert argument " #idx " to boolean"); \
    }                                                                                                               \
    else                                                                                                            \
    {                                                                                                               \
        val = defaultVal;                                                                                           \
    }

// idx starts with 1
#define V8_ARG_TO_MVALUE(idx, val) alt::MValue val = V8Helpers::V8ToMValue(info[(idx)-1]);

// idx starts with 1
#define V8_ARG_TO_INT(idx, val) \
    int64_t val;                \
    V8_CHECK(V8::SafeToInteger(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to integer")

// idx starts with 1
#define V8_ARG_TO_NUMBER(idx, val) \
    double val;                    \
    V8_CHECK(V8::SafeToNumber(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to number")

// idx starts with 1
#define V8_ARG_TO_STRING(idx, val) \
    alt::String val;               \
    V8_CHECK(V8::SafeToString(info[(idx)-1], isolate, ctx, val), "Failed to convert argument " #idx " to string")

// idx starts with 1
#define V8_ARG_TO_FUNCTION(idx, val) \
    v8::Local<v8::Function> val;     \
    V8_CHECK(V8::SafeToFunction(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to function")

// idx starts with 1
#define V8_ARG_TO_OBJECT(idx, val) \
    v8::Local<v8::Object> val;     \
    V8_CHECK(V8::SafeToObject(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to object")

// idx starts with 1
#define V8_ARG_TO_BASE_OBJECT(idx, val, type, jsClassName) \
    alt::Ref<type> val;                                    \
    V8_CHECK(V8::SafeToBaseObject<type>(info[(idx)-1], isolate, val), "Argument " #idx " must be a " jsClassName)

// idx starts with 1
#define V8_ARG_TO_ARRAY_BUFFER(idx, val) \
    v8::Local<v8::ArrayBuffer> val;      \
    V8_CHECK(V8::SafeToArrayBuffer(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to ArrayBuffer")

// idx starts with 1
#define V8_ARG_TO_ARRAY_BUFFER_VIEW(idx, val) \
    v8::Local<v8::ArrayBufferView> val;       \
    V8_CHECK(V8::SafeToArrayBufferView(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to ArrayBufferView")

#define V8_ARG_TO_ARRAY(idx, val) \
    v8::Local<v8::Array> val;     \
    V8_CHECK(V8::SafeToArray(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to Array")

// idx starts with 1
#define V8_ARG_TO_UINT64(idx, val) \
    uint64_t val;                  \
    V8_CHECK(V8::SafeToUInt64(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to uint64")

// idx starts with 1
#define V8_ARG_TO_INT64(idx, val) \
    int64_t val;                  \
    V8_CHECK(V8::SafeToInt64(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to int64")

// idx starts with 1
#define V8_ARG_TO_UINT(idx, val) \
    uint32_t val;                \
    V8_CHECK(V8::SafeToUInt32(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to uint32")

// idx starts with 1
#define V8_ARG_TO_INT32(idx, val) \
    int32_t val;                  \
    V8_CHECK(V8::SafeToInt32(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to int32")

// idx starts with 1
#define V8_ARG_TO_VECTOR3(idx, val) \
    alt::Vector3f val;              \
    V8_CHECK(V8::SafeToVector3(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to vector3")

// idx starts with 1
#define V8_ARG_TO_VECTOR2(idx, val) \
    alt::Vector2f val;              \
    V8_CHECK(V8::SafeToVector2(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to vector2")

// idx starts with 1
#define V8_ARG_TO_RGBA(idx, val) \
    alt::RGBA val;               \
    V8_CHECK(V8::SafeToRGBA(info[(idx)-1], ctx, val), "Failed to convert argument " #idx " to rgba")

#define V8_RETURN(val)            info.GetReturnValue().Set(val)
#define V8_RETURN_NULL()          info.GetReturnValue().SetNull()
#define V8_RETURN_BOOLEAN(val)    V8_RETURN(val)
#define V8_RETURN_INT(val)        V8_RETURN(static_cast<int32_t>(val))
#define V8_RETURN_UINT(val)       V8_RETURN(static_cast<uint32_t>(val))
#define V8_RETURN_NUMBER(val)     V8_RETURN(static_cast<double>(val))
#define V8_RETURN_STRING(val)     V8_RETURN(v8::String::NewFromUtf8(isolate, (val), v8::NewStringType::kNormal).ToLocalChecked())
#define V8_RETURN_ALT_STRING(val) V8_RETURN(v8::String::NewFromUtf8(isolate, (val).CStr(), v8::NewStringType::kNormal).ToLocalChecked())
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
        V8_CHECK(!baseObjectRef.IsEmpty(), reason);       \
        resource->BindEntity(info.This(), baseObjectRef); \
    }

#define V8_EVENT_HANDLER       extern V8::EventHandler
#define V8_LOCAL_EVENT_HANDLER extern V8::LocalEventHandler
#define V8_REFERENCE_EVENT_HANDLER(name) \
    V8_EVENT_HANDLER name;               \
    name.Reference();
#define V8_REFERENCE_LOCAL_EVENT_HANDLER(name) \
    V8_LOCAL_EVENT_HANDLER name;               \
    name.Reference();

#define V8_DEPRECATE(oldName, newName) Log::Warning << oldName << " is deprecated and will be removed in future versions. Consider using " << newName << Log::Endl;
