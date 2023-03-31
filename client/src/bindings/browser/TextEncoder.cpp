#include "V8Helpers.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info) {}

// https://encoding.spec.whatwg.org/#dom-textencoder-encoding
static void EncodingGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_RETURN(V8Helpers::JSValue("utf-8"));
}

// https://encoding.spec.whatwg.org/#dom-textencoder-encode
static void Encode(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::MaybeLocal<v8::String> maybeStr = info[0]->ToString(ctx);
    v8::Local<v8::String> str;
    V8_CHECK(maybeStr.ToLocal(&str), "Argument 0 must be a string");

    size_t size = str->Utf8Length(isolate);
    v8::Local<v8::ArrayBuffer> buffer = v8::ArrayBuffer::New(isolate, size);
    std::shared_ptr<v8::BackingStore> backingStore = buffer->GetBackingStore();
    str->WriteUtf8(isolate, (char*)backingStore->Data(), size, nullptr, v8::String::NO_NULL_TERMINATION | v8::String::REPLACE_INVALID_UTF8);

    V8_RETURN(v8::Uint8Array::New(buffer, 0, size));
}

// https://encoding.spec.whatwg.org/#dom-textencoder-encodeinto
static void EncodeInto(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::MaybeLocal<v8::String> maybeStr = info[0]->ToString(ctx);
    v8::Local<v8::String> str;
    V8_CHECK(maybeStr.ToLocal(&str), "Argument 0 must be a string");
    V8_CHECK(info[1]->IsUint8Array(), "Argument 1 must be a Uint8Array");

    size_t size = str->Utf8Length(isolate);
    v8::Local<v8::Uint8Array> uint8Array = info[1].As<v8::Uint8Array>();
    v8::Local<v8::ArrayBuffer> buffer = uint8Array->Buffer();
    char* data = (char*)((uintptr_t)buffer->GetBackingStore()->Data() + uint8Array->ByteOffset());
    size_t length = uint8Array->ByteLength();
    int numChars;
    int writtenChars = str->WriteUtf8(isolate, data, length, &numChars, v8::String::NO_NULL_TERMINATION | v8::String::REPLACE_INVALID_UTF8);

    v8::Local<v8::Object> result = v8::Object::New(isolate);
    V8_OBJECT_SET_INT(result, "read", numChars);
    V8_OBJECT_SET_INT(result, "written", writtenChars);
    V8_RETURN(result);
}

// clang-format off
extern V8Class v8TextEncoder("TextEncoder", Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8Helpers::SetAccessor(isolate, tpl, "encoding", &EncodingGetter);
    V8Helpers::SetMethod(isolate, tpl, "encode", &Encode);
    V8Helpers::SetMethod(isolate, tpl, "encodeInto", &EncodeInto);
});
// clang-format on
