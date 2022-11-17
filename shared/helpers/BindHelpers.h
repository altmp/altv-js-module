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

#define V8_CALL_SETTER_EXPLICIT(type, req, convert)                                                                                          \
    template<class T>                                                                                                                        \
    static inline void CallSetter(const v8::PropertyCallbackInfo<void>& info, v8::Local<v8::Value> value, T* _this, void (T::*setter)(type)) \
    {                                                                                                                                        \
        req();                                                                                                                               \
        convert(value, _val);                                                                                                                \
        (_this->*setter)((_val));                                                                                                            \
    }

namespace V8Helpers
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
        V8_CALL_GETTER(std::string, V8_GET_ISOLATE, V8_RETURN_STRING);
        V8_CALL_GETTER(std::string&, V8_GET_ISOLATE, V8_RETURN_STRING);
        V8_CALL_GETTER(const std::string&, V8_GET_ISOLATE, V8_RETURN_STRING);
        V8_CALL_GETTER(alt::Position, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Rotation, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Vector3f, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Vector2f, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR2);
        V8_CALL_GETTER(alt::Vector3i, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR3);
        V8_CALL_GETTER(alt::Vector2i, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_VECTOR2);
        V8_CALL_GETTER(alt::RGBA, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_RGBA);
        V8_CALL_GETTER(alt::IColShape::ColShapeType, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_ENUM);
        V8_CALL_GETTER(alt::IBaseObject::Type, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_ENUM);
        V8_CALL_GETTER(alt::IBaseObject*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::IEntity*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::IPlayer*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::IVehicle*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::IRmlElement*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::IRmlDocument*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);
        V8_CALL_GETTER(alt::IObject*, V8_GET_ISOLATE_CONTEXT_RESOURCE, V8_RETURN_BASE_OBJECT);

        V8_CALL_SETTER(bool, V8_GET_ISOLATE, V8_TO_BOOLEAN);
        V8_CALL_SETTER(uint8_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(uint16_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(uint32_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(int8_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(int16_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(int32_t, V8_GET_ISOLATE_CONTEXT, V8_TO_INTEGER);
        V8_CALL_SETTER(float, V8_GET_ISOLATE_CONTEXT, V8_TO_NUMBER);
        V8_CALL_SETTER(double, V8_GET_ISOLATE_CONTEXT, V8_TO_NUMBER);
        V8_CALL_SETTER(std::string, V8_GET_ISOLATE_CONTEXT, V8_TO_STRING);
        V8_CALL_SETTER_EXPLICIT(std::string&, V8_GET_ISOLATE_CONTEXT, V8_TO_STRING);
        V8_CALL_SETTER_EXPLICIT(const std::string&, V8_GET_ISOLATE_CONTEXT, V8_TO_STRING);
        V8_CALL_SETTER(alt::Position, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3);
        V8_CALL_SETTER(alt::Rotation, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3);
        V8_CALL_SETTER(alt::Vector3f, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3);
        V8_CALL_SETTER(alt::Vector2f, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR2);
        V8_CALL_SETTER(alt::Vector3i, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR3_INT);
        V8_CALL_SETTER(alt::Vector2i, V8_GET_ISOLATE_CONTEXT, V8_TO_VECTOR2_INT);
        V8_CALL_SETTER(alt::RGBA, V8_GET_ISOLATE_CONTEXT, V8_TO_RGBA);
        // V8_CALL_SETTER(alt::IEntity*, V8_GET_ISOLATE_CONTEXT, V8_TO_ENTITY);

        template<class T, class U, U (T::*Getter)() const>
        static void WrapGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
        {
            V8_GET_ISOLATE();
            V8_GET_THIS_BASE_OBJECT(_this, T);
            CallGetter<T>(info, _this, Getter);
        }

        template<class T, class U, void (T::*Setter)(U)>
        static void WrapSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
        {
            V8_GET_ISOLATE();
            V8_GET_THIS_BASE_OBJECT(_this, T);
            CallSetter<T>(info, value, _this, Setter);
        }

        template<class T, void (T::*Method)()>
        static void WrapMethod(const v8::FunctionCallbackInfo<v8::Value>& info)
        {
            V8_GET_ISOLATE();
            V8_GET_THIS_BASE_OBJECT(_this, T);
            (_this->*Method)();
        }
    }  // namespace detail

    template<class T, class U, U (T::*Getter)() const>
    inline void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8Helpers::SetAccessor(isolate, tpl, name, &V8Helpers::detail::WrapGetter<T, U, Getter>, nullptr);
    }

    template<class T, class U, U (T::*Getter)() const, void (T::*Setter)(U)>
    inline void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8Helpers::SetAccessor(isolate, tpl, name, V8Helpers::detail::WrapGetter<T, U, Getter>, V8Helpers::detail::WrapSetter<T, U, Setter>);
    }

    template<class T, class U, U (T::*Getter)() const, class OtherU, void (T::*Setter)(OtherU)>
    inline void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8Helpers::SetAccessor(isolate, tpl, name, V8Helpers::detail::WrapGetter<T, U, Getter>, V8Helpers::detail::WrapSetter<T, OtherU, Setter>);
    }

    template<class T, class U, U (T::*Getter)() const>
    inline void SetLazyAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8Helpers::SetLazyAccessor(isolate, tpl, name, V8Helpers::detail::WrapGetter<T, U, Getter>);
    }

    template<class T, void (T::*Method)()>
    inline void SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name)
    {
        V8Helpers::SetMethod(isolate, tpl, name, V8Helpers::detail::WrapMethod<T, Method>);
    }
}  // namespace V8Helpers
