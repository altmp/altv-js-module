#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllVirtualEntities());
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
    for (size_t i = 0; i < size; i++)
    {
        arr->Set(ctx, i, V8Helpers::JSValue(list[i]));
    }

    V8_RETURN(arr);
}

extern V8Class v8WorldObject;
extern V8Class v8VirtualEntity("VirtualEntity",
                        v8WorldObject,
                        [](v8::Local<v8::FunctionTemplate> tpl)
                        {
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

                            V8Helpers::SetAccessor<IVirtualEntity, uint32_t, &IVirtualEntity::GetID>(isolate, tpl, "id");
                            V8Helpers::SetAccessor<IVirtualEntity, IVirtualEntityGroup*, &IVirtualEntity::GetGroup>(isolate, tpl, "group");
                            V8Helpers::SetAccessor<IVirtualEntity, uint32_t, &IVirtualEntity::GetRemoteID>(isolate, tpl, "remoteId");

                            V8Helpers::SetMethod(isolate, tpl, "hasStreamSyncedMeta", HasStreamSyncedMeta);
                            V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMeta", GetStreamSyncedMeta);
                            V8Helpers::SetMethod(isolate, tpl, "getStreamSyncedMetaKeys", GetStreamSyncedMetaDataKeys);
                        });
