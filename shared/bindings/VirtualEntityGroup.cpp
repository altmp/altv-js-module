#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_CHECK(info[0]->IsNumber(), "number expected");
    V8_ARG_TO_UINT(1, maxStreamingDistance);

    IVirtualEntityGroup* virtualEntityGroup = ICore::Instance().CreateVirtualEntityGroup(maxStreamingDistance);

    V8_BIND_BASE_OBJECT(virtualEntityGroup, "Failed to create virtual entity group");
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllVirtualEntityGroups());
}

static void HasMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntityGroup);

    V8_RETURN_BOOLEAN(ent->HasMetaData(key));
}

static void GetMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntityGroup);

    V8_RETURN_MVALUE(ent->GetMetaData(key));
}

static void GetMetaKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntityGroup);

    const std::vector<std::string> list = ent->GetMetaDataKeys();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

static void SetMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, value);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntityGroup);

    ent->SetMetaData(key, value);
}

static void DeleteMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntityGroup);

    ent->DeleteMetaData(key);
}

extern V8Class v8BaseObject;
extern V8Class v8VirtualEntityGroup("VirtualEntityGroup",
                                    v8BaseObject,
                                    Constructor,
                                    [](v8::Local<v8::FunctionTemplate> tpl)
                                    {
                                        v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                        V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

                                        V8Helpers::SetMethod(isolate, tpl, "hasMeta", HasMeta);
                                        V8Helpers::SetMethod(isolate, tpl, "getMeta", GetMeta);
                                        V8Helpers::SetMethod(isolate, tpl, "getMetaKeys", GetMetaKeys);
                                        V8Helpers::SetMethod(isolate, tpl, "setMeta", SetMeta);
                                        V8Helpers::SetMethod(isolate, tpl, "deleteMeta", DeleteMeta);

                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetID>(isolate, tpl, "id");
                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetStreamingRangeLimit>(isolate, tpl, "streamingRangeLimit");
#ifdef ALT_CLIENT_API
                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetRemoteID>(isolate, tpl, "remoteId");
                                        V8Helpers::SetAccessor<IVirtualEntityGroup, bool, &IVirtualEntityGroup::IsRemote>(isolate, tpl, "isRemote");
#endif  // ALT_CLIENT_API

                                    });
