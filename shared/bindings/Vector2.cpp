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

    std::ostringstream ss;
    ss << std::fixed << std::setprecision(4) << "Vector2{ x: " << x << ", y: " << y << " }";

    V8_RETURN_STRING(ss.str());
}

static void ToArray(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    v8::Local<v8::Array> arr = v8::Array::New(isolate, 2);
    arr->Set(ctx, 0, V8Helpers::JSValue(x));
    arr->Set(ctx, 1, V8Helpers::JSValue(y));

    V8_RETURN(arr);
}

static void Length(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    double length = sqrt(x * x + y * y);

    V8_RETURN_NUMBER(length);
}

static void Add(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    if(info.Length() == 2)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);

        V8_RETURN(resource->CreateVector2({ x + x2, y + y2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_RETURN(resource->CreateVector2({ x + value, y + value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_RETURN(resource->CreateVector2({ x + x2, y + y2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

            V8_RETURN(resource->CreateVector2({ x + x2, y + y2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 2 numbers or IVector2");
        }
    }
}

static void Sub(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    if(info.Length() == 2)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);

        V8_RETURN(resource->CreateVector2({ x - x2, y - y2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_RETURN(resource->CreateVector2({ x - value, y - value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_RETURN(resource->CreateVector2({ x - x2, y - y2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

            V8_RETURN(resource->CreateVector2({ x - x2, y - y2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 2 numbers or IVector2");
        }
    }
}

static void Divide(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    if(info.Length() == 2)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);

        V8_CHECK(x2 != 0 && y2 != 0, "Division by zero");
        V8_RETURN(resource->CreateVector2({ x / x2, y / y2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_CHECK(value != 0, "Division by zero");
            V8_RETURN(resource->CreateVector2({ x / value, y / value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_CHECK(x2 != 0 && y2 != 0, "Division by zero");
            V8_RETURN(resource->CreateVector2({ x / x2, y / y2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);
            V8_CHECK(x2 != 0 && y2 != 0, "Division by zero");
            V8_RETURN(resource->CreateVector2({ x / x2, y / y2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 2 numbers or IVector2");
        }
    }
}

static void Multiply(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    if(info.Length() == 2)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);

        V8_RETURN(resource->CreateVector2({ x * x2, y * y2 }));
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);
            V8_RETURN(resource->CreateVector2({ x * value, y * value }));
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);
            V8_RETURN(resource->CreateVector2({ x * x2, y * y2 }));
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

            V8_RETURN(resource->CreateVector2({ x * x2, y * y2 }));
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 2 numbers or IVector2");
        }
    }
}

static void Dot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN2(1, 2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    if(info.Length() == 2)
    {
        V8_ARG_TO_NUMBER(1, x2);
        V8_ARG_TO_NUMBER(2, y2);

        V8_RETURN_NUMBER(x * x2 + y * y2);
    }
    else if(info.Length() == 1)
    {
        auto arg = info[0];
        if(arg->IsNumber())
        {
            V8_ARG_TO_NUMBER(1, value);

            V8_RETURN_NUMBER(x * value + y * value);
        }
        else if(arg->IsArray())
        {
            v8::Local<v8::Array> arr = arg.As<v8::Array>();
            V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

            V8_TO_NUMBER(arr->Get(ctx, 0).ToLocalChecked(), x2);
            V8_TO_NUMBER(arr->Get(ctx, 1).ToLocalChecked(), y2);

            V8_RETURN_NUMBER(x * x2 + y * y2);
        }
        else if(arg->IsObject())
        {
            v8::Local<v8::Object> obj = arg.As<v8::Object>();

            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
            V8_TO_NUMBER(obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

            V8_RETURN_NUMBER(x * x2 + y * y2);
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be a number, an array of 2 numbers or IVector2");
        }
    }
}

static void Negative(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    V8_RETURN(resource->CreateVector2({ -x, -y }));
}

static void Normalize(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    double length = sqrt(x * x + y * y);

    V8_RETURN(resource->CreateVector2({ x / length, y / length }));
}

static void DistanceTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    V8_ARG_TO_OBJECT(1, vec);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

    double xFinal = x - x2;
    double yFinal = y - y2;
    double dist = sqrt((xFinal * xFinal) + (yFinal * yFinal));

    V8_RETURN_NUMBER(dist);
}

static void AngleTo(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(1);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    V8_ARG_TO_OBJECT(1, vec);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

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

    V8_ARG_TO_OBJECT(1, vec);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

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

    double x2 = (x * 180) / PI;
    double y2 = (y * 180) / PI;

    V8_RETURN(resource->CreateVector2({ x2, y2 }));
}

static void ToRadians(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    double x2 = (x * PI) / 180;
    double y2 = (y * PI) / 180;

    V8_RETURN(resource->CreateVector2({ x2, y2 }));
}

static void IsInRange(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();

    V8_CHECK_ARGS_LEN(2);

    v8::Local<v8::Object> _this = info.This();

    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_XKey(isolate)), x);
    V8_TO_NUMBER(V8Helpers::Get(ctx, _this, V8Helpers::Vector3_YKey(isolate)), y);

    V8_ARG_TO_OBJECT(1, vec);
    V8_ARG_TO_NUMBER(2, range);

    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked(), x2);
    V8_TO_NUMBER(vec->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked(), y2);

    double dx = abs(x - x2);
    double dy = abs(y - y2);

    bool isInRange = dx <= range &&                                      // perform fast check first
                     dy <= range && dx * dx + dy * dy <= range * range;  // perform exact check

    V8_RETURN_BOOLEAN(isInRange);
}

static void Lerp(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN(2);

    V8_TO_VECTOR2(info.This(), _this);
    V8_ARG_TO_VECTOR2(1, vec);
    V8_ARG_TO_NUMBER(2, ratio);

    constexpr auto lerp = [](float a, float b, float t) { return a + (b - a) * t; };
    auto lerpedX = lerp(_this[0], vec[0], (float)ratio);
    auto lerpedY = lerp(_this[1], vec[1], (float)ratio);
    alt::Vector2f lerpedVector = { lerpedX, lerpedY };

    V8_RETURN_VECTOR2(lerpedVector);
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN2(1, 2);

    v8::Local<v8::Object> _this = info.This();

    v8::Local<v8::Value> x, y;

    if(info.Length() == 2)
    {
        V8_ARG_CHECK_NUMBER(1);
        V8_ARG_CHECK_NUMBER(2);

        x = info[0];
        y = info[1];
    }
    else
    {
        v8::Local<v8::Value> val = info[0];

        if(val->IsArray())
        {
            v8::Local<v8::Array> arr = val.As<v8::Array>();
            V8_CHECK(arr->Length() == 2, "Argument must be an array of 2 numbers");

            x = arr->Get(ctx, 0).ToLocalChecked();
            y = arr->Get(ctx, 1).ToLocalChecked();

            V8_CHECK(x->IsNumber(), "Argument must be an array of 2 numbers");
            V8_CHECK(y->IsNumber(), "Argument must be an array of 2 numbers");
        }
        else if(val->IsObject())
        {
            v8::Local<v8::Object> obj = val.As<v8::Object>();

            x = obj->Get(ctx, V8Helpers::Vector3_XKey(isolate)).ToLocalChecked();
            y = obj->Get(ctx, V8Helpers::Vector3_YKey(isolate)).ToLocalChecked();

            V8_CHECK(x->IsNumber(), "x must be a number");
            V8_CHECK(y->IsNumber(), "y must be a number");
        }
        else if(val->IsNumber())
        {
            x = val;
            y = val;
        }
        else
        {
            V8Helpers::Throw(isolate, "Argument must be an array of 2 numbers or IVector2");
            return;
        }
    }

    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Vector3_XKey(isolate), x, v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, _this, V8Helpers::Vector3_YKey(isolate), y, v8::PropertyAttribute::ReadOnly);
}

extern V8Class v8Vector2("Vector2",
                         Constructor,
                         [](v8::Local<v8::FunctionTemplate> tpl)
                         {
                             v8::Isolate* isolate = v8::Isolate::GetCurrent();

                             tpl->InstanceTemplate()->SetInternalFieldCount(1);  // !! Needs to be set so V8 knows its a custom class !!
                         });
