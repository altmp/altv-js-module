
#include <iomanip>

#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

constexpr double PI = 3.141592653589793238463;

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(4) << "Vector3{ x: " << x << ", y: " << y << ", z: " << z << " }";

    V8_RETURN_STRING(ss.str().c_str());
}

static void ToArray(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    v8::Local<v8::Array> arr = v8::Array::New(isolate, 3);
    arr->Set(ctx, 0, V8Helpers::JSValue(x));
    arr->Set(ctx, 1, V8Helpers::JSValue(y));
    arr->Set(ctx, 2, V8Helpers::JSValue(z));

    V8_RETURN(arr);
}

static void Length(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    double length = sqrt(x * x + y * y + z * z);

    V8_RETURN_NUMBER(length);
}

static void Add(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);
        V8_ARG_TO_NUMBER(3, z2);

        V8_RETURN(resource->CreateVector3({ x + x2, y + y2, z + z2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_RETURN(resource->CreateVector3({ x + value, y + value, z + value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_TO_NUMBER(arr->Get(ctx, 2).ToLocalChecked(), z2);
            V8_RETURN(resource->CreateVector3({ x + x2, y + y2, z + z2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

            V8_RETURN(resource->CreateVector3({ x + x2, y + y2, z + z2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 3 numbers or IVector3");
        }
    }
}

static void Sub(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);
        V8_ARG_TO_NUMBER(3, z2);

        V8_RETURN(resource->CreateVector3({ x - x2, y - y2, z - z2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_RETURN(resource->CreateVector3({ x - value, y - value, z - value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_TO_NUMBER(arr->Get(ctx, 2).ToLocalChecked(), z2);
            V8_RETURN(resource->CreateVector3({ x - x2, y - y2, z - z2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

            V8_RETURN(resource->CreateVector3({ x - x2, y - y2, z - z2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 3 numbers or IVector3");
        }
    }
}

static void Divide(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);
        V8_ARG_TO_NUMBER(3, z2);

        V8_CHECK(x2 != 0 && y2 != 0 && z2 != 0, "Division by zero");
        V8_RETURN(resource->CreateVector3({ x / x2, y / y2, z / z2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_CHECK(value != 0, "Division by zero");
            V8_RETURN(resource->CreateVector3({ x / value, y / value, z / value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_TO_NUMBER(arr->Get(ctx, 2).ToLocalChecked(), z2);
            V8_CHECK(x2 != 0 && y2 != 0 && z2 != 0, "Division by zero");
            V8_RETURN(resource->CreateVector3({ x / x2, y / y2, z / z2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);
            V8_CHECK(x2 != 0 && y2 != 0 && z2 != 0, "Division by zero");
            V8_RETURN(resource->CreateVector3({ x / x2, y / y2, z / z2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 3 numbers or IVector3");
        }
    }
}

static void Multiply(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);
        V8_ARG_TO_NUMBER(3, z2);

        V8_RETURN(resource->CreateVector3({ x * x2, y * y2, z * z2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_RETURN(resource->CreateVector3({ x * value, y * value, z * value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_TO_NUMBER(arr->Get(ctx, 2).ToLocalChecked(), z2);
            V8_RETURN(resource->CreateVector3({ x * x2, y * y2, z * z2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

            V8_RETURN(resource->CreateVector3({ x * x2, y * y2, z * z2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 3 numbers or IVector3");
        }
    }
}

static void Dot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);
        V8_ARG_TO_NUMBER(3, z2);

        V8_RETURN_NUMBER(x * x2 + y * y2 + z * z2);
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);

            V8_RETURN_NUMBER(x * value + y * value + z * value);
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_TO_NUMBER(arr->Get(ctx, 2).ToLocalChecked(), z2);

            V8_RETURN_NUMBER(x * x2 + y * y2 + z * z2);
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

            V8_RETURN_NUMBER(x * x2 + y * y2 + z * z2);
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 3 numbers or IVector3");
        }
    }
}

static void Cross(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 3);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    if(info.Length() == 3)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);
        V8_ARG_TO_NUMBER(3, z2);

        V8_RETURN(resource->CreateVector3({ (y * z2) - (z * y2), (z * x2) - (x * z2), (x * y2) - (y * x2) }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);

            V8_RETURN(resource->CreateVector3({ (y * value) - (z * value), (z * value) - (x * value), (x * value) - (y * value) }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 3, "Argument must be an array of 3 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_TO_NUMBER(arr->Get(ctx, 2).ToLocalChecked(), z2);

            V8_RETURN(resource->CreateVector3({ (y * z2) - (z * y2), (z * x2) - (x * z2), (x * y2) - (y * x2) }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

            V8_RETURN(resource->CreateVector3({ (y * z2) - (z * y2), (z * x2) - (x * z2), (x * y2) - (y * x2) }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 3 numbers or IVector3");
        }
    }
}

static void Negative(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    V8_RETURN(resource->CreateVector3({ -x, -y, -z }));
}

static void Normalize(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    double length = sqrt(x * x + y * y + z * z);

    V8_RETURN(resource->CreateVector3({ x / length, y / length, z / length }));
}

static void DistanceTo(const v8::FunctionCallbackInfo<v8::Value>& info)
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

    double xFinal = x - x2;
    double yFinal = y - y2;
    double zFinal = z - z2;
    double dist = sqrt((xFinal * xFinal) + (yFinal * yFinal) + (zFinal * zFinal));

    V8_RETURN_NUMBER(dist);
}

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

static void ToDegrees(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    double x2 = (x * 180) / PI;
    double y2 = (y * 180) / PI;
    double z2 = (z * 180) / PI;

    V8_RETURN(resource->CreateVector3({ x2, y2, z2 }));
}

static void ToRadians(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    double x2 = (x * PI) / 180;
    double y2 = (y * PI) / 180;
    double z2 = (z * PI) / 180;

    V8_RETURN(resource->CreateVector3({ x2, y2, z2 }));
}

static void IsInRange(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_ZKey(isolate)), z);

    V8_ARG_TO_OBJECT(1, vec);
    V8_ARG_TO_NUMBER(2, range);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_ZKey(isolate)).ToLocalChecked(), z2);

    double dx = abs(x - x2);
    double dy = abs(y - y2);
    double dz = abs(z - z2);

    bool isInRange = dx <= range &&                                                               // perform fast check first
                     dy <= range && dz <= range && dx * dx + dy * dy + dz * dz <= range * range;  // perform exact check

    V8_RETURN_BOOLEAN(isInRange);
}

static void Lerp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(2);

    V8_TO_VECTOR3(info.This(), _this);
    V8_ARG_TO_VECTOR3(1, vec);
    V8_ARG_TO_NUMBER(2, ratio);

    constexpr auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
    auto lerpedX = lerp(_this[0], vec[0], (float)ratio);
    auto lerpedY = lerp(_this[1], vec[1], (float)ratio);
    auto lerpedZ = lerp(_this[2], vec[2], (float)ratio);
    alt::Vector3f lerpedVector = { lerpedX, lerpedY, lerpedZ };

    V8_RETURN_VECTOR3(lerpedVector);
}

static void StaticZero(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto zero = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 0, 0, 0 }).As<v8::Object>());

    V8_RETURN(zero.Get(isolate));
}

static void StaticOne(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto one = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 1, 1, 1 }).As<v8::Object>());

    V8_RETURN(one.Get(isolate));
}

static void StaticBack(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto back = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 0, -1, 0 }).As<v8::Object>());

    V8_RETURN(back.Get(isolate));
}

static void StaticDown(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto down = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 0, 0, -1 }).As<v8::Object>());

    V8_RETURN(down.Get(isolate));
}

static void StaticForward(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto forward = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 0, 1, 0 }).As<v8::Object>());

    V8_RETURN(forward.Get(isolate));
}

static void StaticLeft(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto left = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ -1, 0, 0 }).As<v8::Object>());

    V8_RETURN(left.Get(isolate));
}

static void StaticRight(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto right = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 1, 0, 0 }).As<v8::Object>());

    V8_RETURN(right.Get(isolate));
}

static void StaticUp(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();
    static auto up = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ 0, 0, 1 }).As<v8::Object>());

    V8_RETURN(up.Get(isolate));
}

static void StaticNegativeInfinity(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();

    float infinity = -std::numeric_limits<float>::infinity();
    static auto negativeInfinity = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ infinity, infinity, infinity }).As<v8::Object>());

    V8_RETURN(negativeInfinity.Get(isolate));
}

static void StaticPositiveInfinity(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_RESOURCE();

    float infinity = std::numeric_limits<float>::infinity();
    static auto positiveInfinity = v8::Eternal<v8::Object>(isolate, resource->CreateVector3({ infinity, infinity, infinity }).As<v8::Object>());

    V8_RETURN(positiveInfinity.Get(isolate));
}

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

extern V8Class v8Vector3("Vector3", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetStaticAccessor(isolate, tpl, "zero", StaticZero);
    V8Helpers::SetStaticAccessor(isolate, tpl, "one", StaticOne);
    V8Helpers::SetStaticAccessor(isolate, tpl, "back", StaticBack);
    V8Helpers::SetStaticAccessor(isolate, tpl, "down", StaticDown);
    V8Helpers::SetStaticAccessor(isolate, tpl, "forward", StaticForward);
    V8Helpers::SetStaticAccessor(isolate, tpl, "left", StaticLeft);
    V8Helpers::SetStaticAccessor(isolate, tpl, "right", StaticRight);
    V8Helpers::SetStaticAccessor(isolate, tpl, "up", StaticUp);
    V8Helpers::SetStaticAccessor(isolate, tpl, "negativeInfinity", StaticNegativeInfinity);
    V8Helpers::SetStaticAccessor(isolate, tpl, "positiveInfinity", StaticPositiveInfinity);

    V8Helpers::SetAccessor(isolate, tpl, "length", Length);
    V8Helpers::SetMethod(isolate, tpl, "toString", ToString);
    V8Helpers::SetMethod(isolate, tpl, "toArray", ToArray);
    V8Helpers::SetMethod(isolate, tpl, "add", Add);
    V8Helpers::SetMethod(isolate, tpl, "sub", Sub);
    V8Helpers::SetMethod(isolate, tpl, "div", Divide);
    V8Helpers::SetMethod(isolate, tpl, "mul", Multiply);
    V8Helpers::SetMethod(isolate, tpl, "dot", Dot);
    V8Helpers::SetMethod(isolate, tpl, "cross", Cross);
    V8Helpers::SetMethod(isolate, tpl, "negative", Negative);
    V8Helpers::SetMethod(isolate, tpl, "normalize", Normalize);
    V8Helpers::SetMethod(isolate, tpl, "distanceTo", DistanceTo);
    V8Helpers::SetMethod(isolate, tpl, "angleTo", AngleTo);
    V8Helpers::SetMethod(isolate, tpl, "angleToDegrees", AngleToDegrees);
    V8Helpers::SetMethod(isolate, tpl, "toRadians", ToRadians);
    V8Helpers::SetMethod(isolate, tpl, "toDegrees", ToDegrees);
    V8Helpers::SetMethod(isolate, tpl, "isInRange", IsInRange);
    V8Helpers::SetMethod(isolate, tpl, "lerp", Lerp);
});
