
#include <iomanip>

#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

constexpr double PI = 3.141592653589793238463;

/*
static void AngleTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    V8_ARG_TO_OBJECT(1, vec);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

    double xy = x * x2 + y * y2;
    double posALength = sqrt(std::pow(x, 2) + std::pow(y, 2));
    double posBLength = sqrt(std::pow(x2, 2) + std::pow(y2, 2));

    if(posALength == 0 || posBLength == 0)
    {
        V8Helpers::Throw(isolate, "Division by zero!");
        return;
    }

    double cos = xy / (posALength * posBLength);
    double radians = std::acos(cos);
    // double angle = radians * (180 / PI);

    V8_RETURN_NUMBER(radians);
}

static void AngleToDegrees(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    V8_ARG_TO_OBJECT(1, vec);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

    double xy = x * x2 + y * y2;
    double posALength = sqrt(std::pow(x, 2) + std::pow(y, 2));
    double posBLength = sqrt(std::pow(x2, 2) + std::pow(y2, 2));

    if(posALength == 0 || posBLength == 0)
    {
        V8Helpers::Throw(isolate, "Division by zero!");
        return;
    }

    double cos = xy / (posALength * posBLength);
    double radians = std::acos(cos);
    double angle = radians * (180 / PI);

    V8_RETURN_NUMBER(angle);
}
*/

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    v8::Local<v8::Value> x, y, z;

    if(info.Length() == 3)
    {
        V8_ARG_CHECK_NUMBER(1);
        V8_ARG_CHECK_NUMBER(2);
        V8_ARG_CHECK_NUMBER(3);

        x = info[0];
        y = info[1];
        z = info[2];
    }
    else
    {
        v8::Local<v8::Value> val = info[0];

        if(val->IsArray())
        {
            v8::Local<v8::Array> arr = val.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            x = arr->Get(ctx, 0).ToLocalChecked();
            y = arr->Get(ctx, 1).ToLocalChecked();
            z = arr->Get(ctx, 2).ToLocalChecked();

            V8_CHECK(x->IsNumber(), "Argument must be an array of 3 numbers");
            V8_CHECK(y->IsNumber(), "Argument must be an array of 3 numbers");
            V8_CHECK(z->IsNumber(), "Argument must be an array of 3 numbers");
        }
        else if(val->IsObject())
        {
            v8::Local<v8::Object> obj = val.As<v8::Object>();

            x = obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked();
            y = obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked();
            z = obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked();

            V8_CHECK(x->IsNumber(), "x must be a number");
            V8_CHECK(y->IsNumber(), "y must be a number");
            V8_CHECK(z->IsNumber(), "z must be a number");
        }
        else if(val->IsNumber())
        {
            x = val;
            y = val;
            z = val;
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be an array of 3 numbers or IVector3");
            return;
        }
    }

    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Vector3_XKey(isolate), x, v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Vector3_YKey(isolate), y, v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Vector3_ZKey(isolate), z, v8::PropertyAttribute::ReadOnly);
}

extern V8Class v8Vector3("Vector3",
                         Constructor,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                             v8::Isolate* isolate = v8::Isolate::GetCurrent();

                             tpl->InstanceTemplate()->SetInternalFieldCount(1);  // !! Needs to be set so V8 knows its a custom class !!
                         });
