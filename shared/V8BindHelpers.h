#pragma once

#include "V8Helpers.h"
#include "V8ResourceImpl.h"

#define V8_CALL_GETTER(type, req, retn)                                                                                 \
    template<class T>                                                                                                   \
    static inline void CallGetter(const v8::PropertyCallbackInfo<v8::Value>& info, T* _this, type (T::*getter)() const) \
    {                                                                                                                   \
        req();                                                                                                          \
        retn((_this->*getter)());                                                                                       \
    }

#define V8_CALL_SETTER(type, req, convert)                                                                                                   \
    template<class T>                                                                                                                        \
    static inline void CallSetter(const v8::PropertyCallbackInfo<void>& info, v8::Local<v8::Value> value, T* _this, void (T::*setter)(type)) \
    {                                                                                                                                        \
        req();                                                                                                                               \
        convert(value, _val);                                                                                                                \
        (_this->*setter)(type(_val));                                                                                                        \
    }

namespace V8
{
    namespace detail
    {
        V8_CALL_GETTER(bool, V8_GET_ISOLATE, V8_RETURN_BOOLEAN);
        V8_CALL_GETTER(uint8_t, V8_GET_ISOLATE, V8_RETURN_UINT);
        V8_CALL_GETTER(uint16_t, V8_GET_ISOLATE, V8_RETURN_UINT);
        V8_CALL_GETTER(uint32_t, V8_GET_ISOLATE, V8_RETURN_UINT);
        V8_CALL_GETTER(uint64_t, V8_GET_ISOLATE, V8_RETURN_UINT64);
        V8_CALL_GETTER(int8_t, V8_GET_ISOLATE, V8_RETURN_INT);
        V8_CALL_GETTER(int16_t, V8_GET_ISOLATE, V8_RETURN_INT);
        V8_CALL_GETTER(int32_t, V8_GET_ISOLATE, V8_RETURN_INT);
        V8_CALL_GETTER(int64_t, V8_GET_ISOLATE, V8_RETURN_INT64);
        V8_CALL_GETTER(float, V8_GET_ISOLATE, V8_RETURN_NUMBER);
        V8_CALL_GETTER(double, V8_GET_ISOLATE, V8_RETURN_NUMBER);
        V8_CALL_GETTER(alt::StringView, V8_GET_ISOLATE, V8_RETURN_ALT_STRING);
        V8_CALL_GETTER(alt::String, V8_GET_ISOLATE, V8_RETURN_ALT_STRING);
        V8_CALL_GETTER(alt::Position, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Rotation, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Vector3f, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Vector2f, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR2);
        V8_CALL_GETTER(alt::RGBA, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_RGBA);
        V8_CALL_GETTER(alt::IColShape::ColShapeType, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_ENUM);
        V8_CALL_GETTER(alt::IBaseObject::Type, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_ENUM);
        V8_CALL_GETTER(alt::Ref<alt::IBaseObject>, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::Ref<alt::IEntity>, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::Ref<alt::IPlayer>, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::Ref<alt::IVehicle>, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);

        V8_CALL_SETTER(bool, V8_GET_ISOLATE, V8_TO_BOOLEAN);
        V8_CALL_SETTER(uint8_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(uint16_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(uint32_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(int8_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(int16_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(int32_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(float, V8_GET_ISOLATE_CONTEXT, V8_TO_NUMBER);
        V8_CALL_SETTER(double, V8_GET_ISOLATE_CONTEXT, V8_TO_NUMBER);
        V8_CALL_SETTER(alt::StringView, V8_GET_ISOLATE_CONTEXT, V8_TO_STRING);
        V8_CALL_SETTER(alt::String, V8_GET_ISOLATE_CONTEXT, V8_TO_STRING);
        V8_CALL_SETTER(alt::Position, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3);
        V8_CALL_SETTER(alt::Rotation, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3);
        V8_CALL_SETTER(alt::Vector3f, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3);
        V8_CALL_SETTER(alt::Vector2f, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR2);
        V8_CALL_SETTER(alt::RGBA, V8_GET_ISOLATE_CONTEXT, V8_TO_RGBA);
        //V8_CALL_SETTER(alt::Ref<alt::IEntity>, V8_GET_ISOLATE_CONTEXT, V8_TO_ENTITY);

        template<class T, class U, U (T::*Getter)() const>
        static void WrapGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
        {
            V8_GET_ISOLATE();
            V8_GET_THIS_BASE_OBJECT(_this, T);
            CallGetter<T>(info, _this.Get(), Getter);
        }

        template<class T, class U, void (T::*Setter)(U)>
        static void WrapSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
        {
            V8_GET_ISOLATE();
            V8_GET_THIS_BASE_OBJECT(_this, T);
            CallSetter<T>(info, value, _this.Get(), Setter);
        }

        template<class T, void (T::*Method)()>
        static void WrapMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            V8_GET_ISOLATE();
            V8_GET_THIS_BASE_OBJECT(_this, T);
            (_this.Get()->*Method)();
        }
    }  // namespace detail

    template<class T, class U, U (T::*Getter)() const>
    inline void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8::SetAccessor(isolate, tpl, name, &V8::detail::WrapGetter<T, U, Getter>, nullptr);
    }

    template<class T, class U, U (T::*Getter)() const, void (T::*Setter)(U)>
    inline void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8::SetAccessor(isolate, tpl, name, V8::detail::WrapGetter<T, U, Getter>, V8::detail::WrapSetter<T, U, Setter>);
    }

    template<class T, void (T::*Method)()>
    inline void SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8::SetMethod(isolate, tpl, name, V8::detail::WrapMethod<T, Method>);
    }
}  // namespace V8
