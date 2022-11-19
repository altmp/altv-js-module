
#include "../V8Helpers.h"
#include "../helpers/BindHelpers.h"
#include "../V8ResourceImpl.h"
#include "../V8Class.h"
#include "../V8Entity.h"

using namespace alt;

static void TypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    V8_RETURN_INT((uint32_t)obj->GetType());
}

static void ValidGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8Entity* ent = V8Entity::Get(info.This());

    V8_RETURN_BOOLEAN(ent ? true : false);
}

static void HasMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    V8_RETURN_BOOLEAN(obj->HasMetaData(key));
}

static void GetMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    V8_RETURN_MVALUE(obj->GetMetaData(key));
}

static void SetMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, value);

    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    obj->SetMetaData(key, value);
}

static void DeleteMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    obj->DeleteMetaData(key);
}

static void GetMetaDataKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    const std::vector<std::string> list = obj->GetMetaDataKeys();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

static void Destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_GET_THIS_BASE_OBJECT(obj, alt::IBaseObject);

    alt::ICore::Instance().DestroyBaseObject(obj);
}

extern V8Class v8BaseObject("BaseObject",
                            [](v8::Local<v8::FunctionTemplate> tpl)
                            {
                                v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                tpl->InstanceTemplate()->SetInternalFieldCount(1);

                                V8Helpers::SetAccessor<IBaseObject, IBaseObject::Type, &IBaseObject::GetType>(isolate, tpl, "type");
                                V8Helpers::SetAccessor(isolate, tpl, "valid", &ValidGetter);

                                V8Helpers::SetMethod(isolate, tpl, "hasMeta", HasMeta);
                                V8Helpers::SetMethod(isolate, tpl, "getMeta", GetMeta);
                                V8Helpers::SetMethod(isolate, tpl, "setMeta", SetMeta);
                                V8Helpers::SetMethod(isolate, tpl, "deleteMeta", DeleteMeta);
                                V8Helpers::SetMethod(isolate, tpl, "getMetaDataKeys", GetMetaDataKeys);
                                V8Helpers::SetMethod(isolate, tpl, "destroy", Destroy);
                            });
