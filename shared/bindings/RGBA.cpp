
#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_TO_NUMBER(info.This()->Get(ctx, V8Helpers::RGBA_RKey(isolate)).ToLocalChecked(), r);
    V8_TO_NUMBER(info.This()->Get(ctx, V8Helpers::RGBA_GKey(isolate)).ToLocalChecked(), g);
    V8_TO_NUMBER(info.This()->Get(ctx, V8Helpers::RGBA_BKey(isolate)).ToLocalChecked(), b);
    V8_TO_NUMBER(info.This()->Get(ctx, V8Helpers::RGBA_AKey(isolate)).ToLocalChecked(), a);

    std::ostringstream ss;
    ss << "RGBA{ r: " << r << ", g: " << g << ", b: " << b << ", a: " << a << " }";

    V8_RETURN_STRING(ss.str().c_str());
}

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN(4);

    V8_ARG_TO_INT32(1, r);
    V8_CHECK(r >= 0 && r < 256, "Invalid RGBA R value. Allowed is 0 - 255");
    V8_ARG_TO_INT32(2, g);
    V8_CHECK(g >= 0 && g < 256, "Invalid RGBA G value. Allowed is 0 - 255");
    V8_ARG_TO_INT32(3, b);
    V8_CHECK(b >= 0 && b < 256, "Invalid RGBA B value. Allowed is 0 - 255");
    V8_ARG_TO_INT32(4, a);
    V8_CHECK(a >= 0 && a < 256, "Invalid RGBA A value. Allowed is 0 - 255");

    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_RKey(isolate), V8Helpers::JSValue(r), v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_GKey(isolate), V8Helpers::JSValue(g), v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_BKey(isolate), V8Helpers::JSValue(b), v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_AKey(isolate), V8Helpers::JSValue(a), v8::PropertyAttribute::ReadOnly);
}

extern V8Class v8RGBA("RGBA", &Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetMethod(isolate, tpl, "toString", ToString);
});
