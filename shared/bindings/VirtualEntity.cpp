#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(3, 4);

    V8_CHECK(info[2]->IsNumber(), "number expected");
    V8_ARG_TO_BASE_OBJECT(1, ent, IVirtualEntityGroup, "virtualEntityGroup")
    V8_ARG_TO_VECTOR3(2, position)
    V8_ARG_TO_UINT(3, streamingDistance)

    std::unordered_map<std::string, alt::MValue> data;
    if(info.Length() == 4)
    {
        V8_ARG_TO_OBJECT(4, passedData)
        const auto propertyNames = passedData->GetPropertyNames(ctx).ToLocalChecked();

        for(uint32_t i = 0; i < propertyNames->Length(); ++i)
        {
            const auto v8Key = propertyNames->Get(ctx, i).ToLocalChecked();
            V8_TO_STRING(v8Key, key)
            const auto v8Value = passedData->Get(ctx, V8Helpers::JSValue(key)).ToLocalChecked();
            auto value = V8Helpers::V8ToMValue(v8Value, false);
            data.insert({ key, value });
        }
    }

    IVirtualEntity* virtualEntity = alt::ICore::Instance().CreateVirtualEntity(ent, position, streamingDistance, data);

    V8_BIND_BASE_OBJECT(virtualEntity, "Failed to create virtual entity group");
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllVirtualEntities());
}

static void HasMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    V8_RETURN_BOOLEAN(ent->HasMetaData(key));
}

static void GetMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    V8_RETURN_MVALUE(ent->GetMetaData(key));
}

static void GetMetaKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

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

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    ent->SetMetaData(key, value);
}

static void DeleteMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    ent->DeleteMetaData(key);
}

static void HasStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    V8_RETURN_BOOLEAN(ent->HasStreamSyncedMetaData(key));
}

static void GetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    V8_RETURN_MVALUE(ent->GetStreamSyncedMetaData(key));
}

static void GetStreamSyncedMetaDataKeys(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    const std::vector<std::string> list = ent->GetStreamSyncedMetaDataKeys();
    size_t size = list.size();
    v8::Local<v8::Array> arr = v8::Array::New(isolate, size);
    for(size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

#ifdef ALT_SERVER_API

static void SetStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(2);
    V8_ARG_TO_STRING(1, key);
    V8_ARG_TO_MVALUE(2, value);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    ent->SetStreamSyncedMetaData(key, value);
}

static void DeleteStreamSyncedMeta(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_STRING(1, key);

    V8_GET_THIS_BASE_OBJECT(ent, alt::IVirtualEntity);

    ent->DeleteStreamSyncedMetaData(key);
}

#endif  // ALT_SERVER_API

#ifdef ALT_CLIENT_API

static void StreamedInGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    auto streamed = alt::ICore::Instance().GetVirtualEntitiesStreamedIn();
    v8::Local<v8::Array> jsArr = v8::Array::New(isolate, streamed.size());
    for(size_t i = 0; i < streamed.size(); ++i) jsArr->Set(ctx, i, resource->GetBaseObjectOrNull(streamed[i]));
    V8_RETURN(jsArr);
}

#endif  // ALT_CLIENT_API

extern V8Class v8WorldObject;
extern V8Class v8VirtualEntity("VirtualEntity",
                               v8WorldObject,
                               Constructor,
                               [](v8::Local<v8::FunctionTemplate> tpl)
                               {
                                   v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                   V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

                                   V8Helpers::SetAccessor<IVirtualEntity, uint32_t, &IVirtualEntity::GetID>(isolate, tpl, "id");
                                   V8Helpers::SetAccessor<IVirtualEntity, IVirtualEntityGroup*, &IVirtualEntity::GetGroup>(isolate, tpl, "group");
                                   V8Helpers::SetAccessor<IVirtualEntity, uint32_t, &IVirtualEntity::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                                   V8Helpers::SetAccessor<IVirtualEntity, bool, &IVirtualEntity::IsVisible, &IVirtualEntity::SetVisible>(isolate, tpl, "visible");

                                   V8Helpers::SetMethod(isolate, tpl, "hasMeta", HasMeta);
                                   V8Helpers::SetMethod(isolate, tpl, "getMeta", GetMeta);
                                   V8Helpers::SetMethod(isolate, tpl, "getMetaKeys", GetMetaKeys);
                                   V8Helpers::SetMethod(isolate, tpl, "setMeta", SetMeta);
                                   V8Helpers::SetMethod(isolate, tpl, "deleteMeta", DeleteMeta);

                                   V8Helpers::SetMethod(isolate, tpl, "hasStreamSyncedMeta", HasStreamSyncedMeta);
                                   V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMeta", GetStreamSyncedMeta);
                                   V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMetaKeys", GetStreamSyncedMetaDataKeys);

#ifdef ALT_SERVER_API
                                   V8Helpers::SetMethod(isolate, tpl, "setStreamSyncedMeta", SetStreamSyncedMeta);
                                   V8Helpers::SetMethod(isolate, tpl, "deleteStreamSyncedMeta", DeleteStreamSyncedMeta);
#endif  // ALT_SERVER_API

#ifdef ALT_CLIENT_API
                                   V8Helpers::SetAccessor<IVirtualEntity, uint32_t, &IVirtualEntity::GetRemoteID>(isolate, tpl, "remoteId");
                                   V8Helpers::SetAccessor<IVirtualEntity, bool, &IVirtualEntity::IsRemote>(isolate, tpl, "isRemote");
                                   V8Helpers::SetAccessor<IVirtualEntity, bool, &IVirtualEntity::IsStreamedIn>(isolate, tpl, "isStreamedIn");
                                   V8Helpers::SetStaticAccessor(isolate, tpl, "streamedIn", &StreamedInGetter);
#endif  // ALT_CLIENT_API
                               });
