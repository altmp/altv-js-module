#include "V8Helpers.h"
#include "helpers/BindHelpers.h"

using namespace alt;

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllVirtualEntityGroups());
}

extern V8Class v8BaseObject;
extern V8Class v8VirtualEntityGroup("VirtualEntityGroup",
                                    v8BaseObject,
                                    [](v8::Local<v8::FunctionTemplate> tpl)
                                    {
                                        v8::Isolate* isolate = v8::Isolate::GetCurrent();

                                        V8Helpers::SetStaticAccessor(isolate, tpl, "all", AllGetter);

                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetID>(isolate, tpl, "id");
                                        V8Helpers::SetAccessor<IVirtualEntityGroup, uint32_t, &IVirtualEntityGroup::GetStreamingRangeLimit>(isolate, tpl, "streamingRangeLimit");
                                    });
