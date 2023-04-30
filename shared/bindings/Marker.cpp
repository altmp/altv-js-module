#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "cpp-sdk/script-objects/IMarker.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
#ifdef ALT_SERVER_API
    V8_CHECK_ARGS_LEN(3);
#else
    V8_CHECK_ARGS_LEN_MIN_MAX(3, 5);
#endif

    V8_ARG_TO_NUMBER(1, type);
    V8_ARG_TO_VECTOR3(2, position);
    V8_ARG_TO_RGBA(3, color);

#ifdef ALT_SERVER_API
    alt::IMarker* marker = alt::ICore::Instance().CreateMarker(nullptr, (alt::IMarker::MarkerType)type, position, color, resource->GetResource());
#else

    V8_ARG_TO_BOOLEAN_OPT(4, useStreaming, false);
    V8_ARG_TO_UINT_OPT(5, streamingDistance, 0);

    alt::IMarker* marker = alt::ICore::Instance().CreateMarker((alt::IMarker::MarkerType)type, position, color, useStreaming, streamingDistance, resource->GetResource());
#endif

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

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_INT(1, id);

    alt::IBaseObject* baseObject = alt::ICore::Instance().GetBaseObjectByID(alt::IBaseObject::Type::MARKER, id);

    if(baseObject && baseObject->GetType() == alt::IEntity::Type::MARKER)
    {
        V8_RETURN_BASE_OBJECT(baseObject);
    }
    else
    {
        V8_RETURN_NULL();
    }
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
                            V8Helpers::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);

#ifdef ALT_CLIENT_API
                            V8Helpers::SetAccessor<IMarker, bool, &IMarker::IsRemote>(isolate, tpl, "isRemote");
                            V8Helpers::SetAccessor<IMarker, uint32_t, &IMarker::GetRemoteID>(isolate, tpl, "remoteId");
                            V8Helpers::SetAccessor<IMarker, bool, &IMarker::IsStreamedIn>(isolate, tpl, "isStreamedIn");
#endif

                            V8Helpers::SetAccessor<IMarker, uint32_t, &IMarker::GetID>(isolate, tpl, "id");
                            V8Helpers::SetAccessor<IMarker, bool, &IMarker::IsGlobal>(isolate, tpl, "isGlobal");
                            V8Helpers::SetAccessor<IMarker, IPlayer*, &IMarker::GetTarget>(isolate, tpl, "target");
                            V8Helpers::SetAccessor<IMarker, bool, &IMarker::IsVisible, &IMarker::SetVisible>(isolate, tpl, "visible");
                            V8Helpers::SetAccessor(isolate, tpl, "markerType", &TypeGetter, &TypeSetter);
                            V8Helpers::SetAccessor<IMarker, RGBA, &IMarker::GetColor, &IMarker::SetColor>(isolate, tpl, "color");
                            V8Helpers::SetAccessor<IMarker, Position, &IMarker::GetScale, &IMarker::SetScale>(isolate, tpl, "scale");
                            V8Helpers::SetAccessor<IMarker, Rotation, &IMarker::GetRotation, &IMarker::SetRotation>(isolate, tpl, "rot");
                            V8Helpers::SetAccessor<IMarker, Position, &IMarker::GetDirection, &IMarker::SetDirection>(isolate, tpl, "dir");

                            V8Helpers::SetAccessor<IMarker, uint32_t, &IMarker::GetStreamingDistance>(isolate, tpl, "streamingDistance");
                        });
