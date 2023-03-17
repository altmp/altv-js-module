#include "V8Helpers.h"

// https://encoding.spec.whatwg.org/#dom-textdecoder
static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN_MIN_MAX(0, 2);

    // TODO: Support more than utf-8
    std::string encoding = "utf-8";
    if(info.Length() > 0)
    {
        V8_ARG_TO_STRING(1, encodingStr);
        encoding = encodingStr;
    }
    V8_CHECK(encoding != "replacement", "Invalid encoding");

    bool fatal = false;
    bool ignoreBOM = false;
    if(info.Length() > 1)
    {
        V8_ARG_TO_OBJECT(2, options);
        V8_OBJECT_GET_BOOLEAN(options, "fatal", fatalBool);
        V8_OBJECT_GET_BOOLEAN(options, "ignoreBOM", ignoreBOMBool);
        fatal = fatalBool;
        ignoreBOM = ignoreBOMBool;
    }

    info.This()->Set(ctx, V8Helpers::JSValue("__encoding"), V8Helpers::JSValue(encoding));
    info.This()->Set(ctx, V8Helpers::JSValue("__fatal"), V8Helpers::JSValue(fatal));
    info.This()->Set(ctx, V8Helpers::JSValue("__ignoreBOM"), V8Helpers::JSValue(ignoreBOM));
}

// https://encoding.spec.whatwg.org/#dom-textdecoder-encoding
static void EncodingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN(info.This()->Get(ctx, V8Helpers::JSValue("__encoding")).ToLocalChecked());
}

// clang-format off
extern V8Class v8TextDecoder("TextDecoder", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    tpl->InstanceTemplate()->SetInternalFieldCount(2);

    V8Helpers::SetAccessor(isolate, tpl, "encoding", &EncodingGetter);
    // 'decode' is implemented in JS
});
// clang-format on
