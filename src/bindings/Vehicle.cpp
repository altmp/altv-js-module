
#include "../helpers/V8Helpers.h"
#include "../helpers/V8Class.h"
#include "../helpers/V8Entity.h"
#include "../helpers/V8ResourceImpl.h"
#include "cpp-sdk/objects/IVehicle.h"

using namespace alt;

static void HandlingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    std::vector<v8::Local<v8::Value>> args{
        info.This()};

    extern V8Class v8Handling;
    info.GetReturnValue().Set(v8Handling.New(isolate->GetEnteredContext(), args));
}

static void SpeedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IVehicle> vehicle = _this->GetHandle().As<alt::IVehicle>();

    info.GetReturnValue().Set(v8::Number::New(isolate, vehicle->GetWheelSpeed()));
}

static void GearGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IVehicle> vehicle = _this->GetHandle().As<alt::IVehicle>();

    info.GetReturnValue().Set(v8::Integer::New(isolate, vehicle->GetCurrentGear()));
}

static void RPMGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IVehicle> vehicle = _this->GetHandle().As<alt::IVehicle>();

    info.GetReturnValue().Set(v8::Number::New(isolate, vehicle->GetCurrentRPM()));
}

static void WheelsCountGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IVehicle> vehicle = _this->GetHandle().As<alt::IVehicle>();

    info.GetReturnValue().Set(v8::Number::New(isolate, vehicle->GetWheelsCount()));
}

static void SpeedVectorGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    V8Entity *_this = V8Entity::Get(info.This());
    V8_CHECK(_this, "entity is invalid");

    alt::Ref<alt::IVehicle> vehicle = _this->GetHandle().As<alt::IVehicle>();

    info.GetReturnValue().Set(resource->CreateVector3(vehicle->GetSpeedVector()));
}

// static void GravityGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value> &info)
// {
//     V8_GET_ISOLATE_CONTEXT();

//     V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
//     V8_CHECK(resource, "invalid resource");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     auto vehicle = _this->GetHandle().As<alt::IVehicle>()->GetGameVehicle();
//     V8_CHECK(vehicle, "Could not retrieve game vehicle");

//     Log::Debug("GRAVITY", vehicle->gravity);
//     auto ret = v8::Number::New(isolate, vehicle->gravity);
//     Log::Debug("RET GRAVITY", ret->NumberValue(ctx).ToChecked());
//     info.GetReturnValue().Set(ret);
// }

// static void GravitySetter(v8::Local<v8::String>, v8::Local<v8::Value> val, const v8::PropertyCallbackInfo<void> &info)
// {
//     V8_GET_ISOLATE_CONTEXT();
//     V8_CHECK(val->IsNumber(), "val must be a number");

//     V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
//     V8_CHECK(resource, "invalid resource");

//     V8_CHECK(val->IsNumber(), "val needs to be a nummber (float)");

//     V8Entity *_this = V8Entity::Get(info.This());
//     V8_CHECK(_this, "entity is invalid");

//     auto vehicle = _this->GetHandle().As<alt::IVehicle>()->GetGameVehicle();
//     V8_CHECK(vehicle, "Could not retrieve game vehicle");

//     Log::Debug("GRAVITY", vehicle->gravity);
//     auto newval = val->NumberValue(ctx).ToChecked();
//     Log::Debug("NEW GRAVITY", newval);
//     vehicle->gravity = newval;
// }

static void AllGetter(v8::Local<v8::String> name, const v8::PropertyCallbackInfo<v8::Value> &info)
{
    v8::Isolate *isolate = info.GetIsolate();
    v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

    V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
    V8_CHECK(resource, "invalid resource");

    v8::Local<v8::Array> arr = v8::Array::New(info.GetIsolate());

    uint16_t i = 0;
    for (auto vehicle : alt::ICore::Instance().GetVehicles())
    {
        if (vehicle)
            arr->Set(ctx, i++, resource->GetOrCreateEntity(vehicle.Get(), "Vehicle")->GetJSVal(isolate));
    };

    info.GetReturnValue().Set(arr);
}

static void StaticGetByScriptID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, scriptGuid);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByScriptGuid(scriptGuid).As<alt::IVehicle>());
}

static void StaticGetByID(const v8::FunctionCallbackInfo<v8::Value> &info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);
    V8_ARG_TO_INTEGER(1, id);
    V8_RETURN_BASE_OBJECT(alt::ICore::Instance().GetEntityByID(id).As<alt::IVehicle>());
}

extern V8Class v8Entity;
extern V8Class v8Vehicle("Vehicle", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate *isolate = v8::Isolate::GetCurrent();

    v8::Local<v8::ObjectTemplate> proto = tpl->PrototypeTemplate();

    V8::SetStaticMethod(isolate, tpl, "getByID", StaticGetByID);
    V8::SetStaticMethod(isolate, tpl, "getByScriptID", StaticGetByScriptID);

    V8::SetStaticAccessor(isolate, tpl, "all", &AllGetter);

    V8::SetAccessor(isolate, tpl, "speed", &SpeedGetter);
    V8::SetAccessor(isolate, tpl, "gear", &GearGetter);
    V8::SetAccessor(isolate, tpl, "rpm", &RPMGetter);
    V8::SetAccessor(isolate, tpl, "wheelsCount", &WheelsCountGetter);
    V8::SetAccessor(isolate, tpl, "speedVector", &SpeedVectorGetter);
    // V8::SetAccessor(isolate, tpl, "gravity", &GravityGetter, &GravitySetter);
    V8::SetAccessor(isolate, tpl, "handling", &HandlingGetter);
});
