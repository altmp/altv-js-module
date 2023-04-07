#include <iomanip>

#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

constexpr double PI = 3.141592653589793238463;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_WKey(isolate)), z);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_ZKey(isolate)), w);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(4) << "Quaternion{ x: " << x << ", y: " << y << ", z: " << z << ", w: " << w << " }";

    V8_RETURN_STRING(ss.str());
}

static void ToArray(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_WKey(isolate)), z);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Quaternion_ZKey(isolate)), w);

    v8::Local<v8::Array> arr = v8::Array::New(isolate, 4);
    arr->Set(ctx, 0, V8Helpers::JSValue(x));
    arr->Set(ctx, 1, V8Helpers::JSValue(y));
    arr->Set(ctx, 2, V8Helpers::JSValue(z));
    arr->Set(ctx, 3, V8Helpers::JSValue(w));

    V8_RETURN(arr);
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(1, 4);

    v8::Local<v8::Object> _this = info.This();

    v8::Local<v8::Value> x, y, z, w;

    if(info.Length() == 1)
    {
        v8::Local<v8::Value> val = info[0];

        if(info[0]->IsArray())
        {
            v8::Local<v8::Array> arr = val.As<v8::Array>();
            V8_CHECK(arr->Length() == 4, "Array has to contain 4 (X, Y, Z, W) values");

            x = arr->Get(ctx, 0).ToLocalChecked();
            y = arr->Get(ctx, 1).ToLocalChecked();
            z = arr->Get(ctx, 2).ToLocalChecked();
            w = arr->Get(ctx, 3).ToLocalChecked();

            V8_CHECK(x->IsNumber(), "Argument must be an array of 4 numbers");
            V8_CHECK(y->IsNumber(), "Argument must be an array of 4 numbers");
            V8_CHECK(z->IsNumber(), "Argument must be an array of 4 numbers");
            V8_CHECK(w->IsNumber(), "Argument must be an array of 4 numbers");
        }
        else if(info[1]->IsObject())
        {
            v8::Local<v8::Object> obj = val.As<v8::Object>();

            x = obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked();
            y = obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked();
            z = obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked();
            w = obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked();

            V8_CHECK(x->IsNumber(), "Argument must be an array of 4 numbers");
            V8_CHECK(y->IsNumber(), "Argument must be an array of 4 numbers");
            V8_CHECK(z->IsNumber(), "Argument must be an array of 4 numbers");
            V8_CHECK(w->IsNumber(), "Argument must be an array of 4 numbers");
        }
        else if(val->IsNumber())
        {
            x = val;
            y = val;
            z = val;
            w = val;
        }
        else
        {
            V8Helpers::Throw(isolate, "Invalid arguments");
            return;
        }
    }
    else
    {
        V8_ARG_CHECK_NUMBER(1);
        V8_ARG_CHECK_NUMBER(2);
        V8_ARG_CHECK_NUMBER(3);
        V8_ARG_CHECK_NUMBER(4);

        x = info[0];
        y = info[1];
        z = info[2];
        w = info[3];
    }

    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Quaternion_XKey(isolate), x, v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Quaternion_YKey(isolate), y, v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Quaternion_ZKey(isolate), z, v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Quaternion_WKey(isolate), w, v8::PropertyAttribute::ReadOnly);
}

extern V8Class v8Quaternion("Quaternion",
                         Constructor,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                             v8::Isolate* isolate = v8::Isolate::GetCurrent();

                             tpl->InstanceTemplate()->SetInternalFieldCount(1);  // !! Needs to be set so V8 knows its a custom class !!
                         });
