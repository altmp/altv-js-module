#include "../CV8Resource.h"
#include "V8Class.h"

extern V8Class v8MapZoomData;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_CHECK(info[0]->IsNumber() || info[0]->IsString(), "zoomDataId must be a number or string");

    
    V8Helpers::SetObjectClass(info.GetIsolate(), info.This(), V8Class::ObjectClass::MAP_ZOOM_DATA);
    if(info[0]->IsNumber())
    {
        V8_ARG_TO_UINT(1, zoomDataId);
        auto data = alt::ICore::Instance().GetMapData(zoomDataId);
        V8_CHECK(data, "zoomData with this id not found");

        info.This()->SetInternalField(1, info[0]);
    }
    else
    {
        V8_ARG_TO_STRING(1, zoomDataAlias);

        auto data = alt::ICore::Instance().GetMapData(zoomDataAlias);
        V8_CHECK(data, "zoomData with this id not found");

        uint8_t id = alt::ICore::Instance().GetMapDataIDFromAlias(zoomDataAlias);
        info.This()->SetInternalField(1, V8Helpers::JSValue(id));
    }
}

static void Get(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_CHECK_ARGS_LEN(1);
    V8_CHECK(info[0]->IsNumber() || info[0]->IsString(), "zoomDataId must be a number or string");

    std::vector<v8::Local<v8::Value>> args{ info[0] };
    V8_RETURN(v8MapZoomData.New(isolate->GetEnteredOrMicrotaskContext(), args));
}

static void ResetAll(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    alt::ICore::Instance().ResetAllMapData();
}

static void fZoomScaleGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    V8_RETURN_NUMBER(data->GetZoomScale());
}

static void fZoomScaleSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);
    V8_TO_NUMBER(val, fvalue);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    data->SetZoomScale((float)fvalue);
}

static void fZoomSpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    V8_RETURN_NUMBER(data->GetZoomSpeed());
}

static void fZoomSpeedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);
    V8_TO_NUMBER(val, fvalue);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    data->SetZoomSpeed((float)fvalue);
}

static void fScrollSpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    V8_RETURN_NUMBER(data->GetScrollSpeed());
}

static void fScrollSpeedSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);
    V8_TO_NUMBER(val, fvalue);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    data->SetScrollSpeed((float)fvalue);
}

static void vTilesXGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    V8_RETURN_NUMBER(data->GetTilesCountX());
}

static void vTilesXSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);
    V8_TO_NUMBER(val, fvalue);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    data->SetTilesCountX((float)fvalue);
}

static void vTilesYGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    V8_RETURN_NUMBER(data->GetTilesCountY());
}

static void vTilesYSetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);
    V8_TO_NUMBER(val, fvalue);

    auto data = alt::ICore::Instance().GetMapData(zoomDataId);
    V8_CHECK(data, "zoom data not found");

    data->SetTilesCountY((float)fvalue);
}

static void Reset(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_UINT32(1, zoomDataId);

    alt::ICore::Instance().ResetMapData(zoomDataId);
}

// Perhaps rename or something
extern V8Class v8MapZoomData("MapZoomData", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    tpl->InstanceTemplate()->SetInternalFieldCount(static_cast<int>(V8Class::InternalFields::COUNT));

    V8Helpers::SetStaticMethod(isolate, tpl, "get", &Get);
    V8Helpers::SetStaticMethod(isolate, tpl, "resetAll", &ResetAll);

    V8Helpers::SetAccessor(isolate, tpl, "fZoomScale", &fZoomScaleGetter, &fZoomScaleSetter);
    V8Helpers::SetAccessor(isolate, tpl, "fZoomSpeed", &fZoomSpeedGetter, &fZoomSpeedSetter);
    V8Helpers::SetAccessor(isolate, tpl, "fScrollSpeed", &fScrollSpeedGetter, &fScrollSpeedSetter);
    V8Helpers::SetAccessor(isolate, tpl, "vTilesX", &vTilesXGetter, &vTilesXSetter);
    V8Helpers::SetAccessor(isolate, tpl, "vTilesY", &vTilesYGetter, &vTilesYSetter);
    V8Helpers::SetMethod(isolate, tpl, "reset", &Reset);
});
