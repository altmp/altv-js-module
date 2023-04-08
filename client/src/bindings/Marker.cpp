#include "../CV8Resource.h"
#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/IMarker.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(3);

    V8_ARG_TO_NUMBER(1, type);
    V8_ARG_TO_VECTOR3(2, position);
    V8_ARG_TO_RGBA(3, color);

    alt::IMarker* marker = alt::ICore::Instance().CreateMarker((alt::IMarker::MarkerType)type, position, color, resource->GetResource());

    V8_BIND_BASE_OBJECT(marker, "Failed to create Marker");
}

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_RETURN(resource->GetAllMarkers()->Clone());
}

static void TypeGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();

    V8_GET_THIS_BASE_OBJECT(marker, alt::IMarker);

    V8_RETURN_NUMBER(marker->GetMarkerType());
}

static void TypeSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_BASE_OBJECT(marker, alt::IMarker);

    V8_TO_NUMBER(val, markertype);
    marker->SetMarkerType((alt::IMarker::MarkerType)markertype);
}

extern V8Class v8WorldObject;
extern V8Class v8Marker("Marker",
                        v8WorldObject,
                        Constructor,
                        [](v8::Local<v8::FunctionTemplate> tpl)
                        {
                            using namespace alt;
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();

                            V8Helpers::SetStaticAccessor(isolate, tpl, "all", &AllGetter);

                            V8Helpers::SetAccessor<IMarker, uint32_t, &IMarker::GetID>(isolate, tpl, "id");
                            V8Helpers::SetAccessor<IMarker, bool, &IMarker::GetVisible, &IMarker::SetVisible>(isolate, tpl, "visible");
                            V8Helpers::SetAccessor(isolate, tpl, "type", &TypeGetter, &TypeSetter);
                            V8Helpers::SetAccessor<IMarker, RGBA, &IMarker::GetColor, &IMarker::SetColor>(isolate, tpl, "color");
                            V8Helpers::SetAccessor<IMarker, Position, &IMarker::GetScale, &IMarker::SetScale>(isolate, tpl, "scale");
                            V8Helpers::SetAccessor<IMarker, Rotation, &IMarker::GetRotation, &IMarker::SetRotation>(isolate, tpl, "rotation");
                            V8Helpers::SetAccessor<IMarker, Vector4f, &IMarker::GetDirection, &IMarker::SetDirection>(isolate, tpl, "direction");
                        });
