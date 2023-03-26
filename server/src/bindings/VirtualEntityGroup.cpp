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

extern V8Class v8BaseObject;
extern V8Class v8VirtualEntityGroup("VirtualEntityGroup",
                                    v8BaseObject,
                                    Constructor,
                                    [](v8::Local<v8::FunctionTemplate> tpl)
                                    {
                                        v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                        V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetID>(isolate, tpl, "id");
                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetStreamingRangeLimit>(isolate, tpl, "streamingRangeLimit");
                                    });
