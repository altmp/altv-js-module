
#include "../V8Class.h"
#include "../V8Helpers.h"
#include "../V8ResourceImpl.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    V8_CHECK_ARGS_LEN_MIN_MAX(1, 4);

    int32_t r = 0, g = 0, b = 0, a = 255;

    if(info.Length() == 1)
    {
        if(info[0]->IsArray())
        {
            // todo: we should probably have a helper instead of doing this awfulness...
            v8::Local<v8::Array> arr = info[0].As<v8::Array>();
            V8_CHECK(arr->Length() >= 3, "Array has to contain atleast 3 (R, G, B) values");
            v8::Local<v8::Value> val;
            V8_CHECK(arr->Get(ctx, 0).ToLocal(&val), "Invalid array value at index 0");
            V8_CHECK(val->IsNumber(), "Invalid array value type at index 0");
            r = val->Int32Value(ctx).FromMaybe(0);
            V8_CHECK(arr->Get(ctx, 1).ToLocal(&val), "Invalid array value at index 1");
            V8_CHECK(val->IsNumber(), "Invalid array value type at index 1");
            g = val->Int32Value(ctx).FromMaybe(0);
            V8_CHECK(arr->Get(ctx, 2).ToLocal(&val), "Invalid array value at index 2");
            V8_CHECK(val->IsNumber(), "Invalid array value type at index 2");
            b = val->Int32Value(ctx).FromMaybe(0);

            if(arr->Length() > 3)
            {
                V8_CHECK(arr->Get(ctx, 3).ToLocal(&val), "Invalid array value at index 3");
                V8_CHECK(val->IsNumber(), "Invalid array value type at index 3");
                a = val->Int32Value(ctx).FromMaybe(255);
            }
            else
                a = 255;
        }
        else if(info[0]->IsObject())
        {
            v8::Local<v8::Object> obj = info[0].As<v8::Object>();
            v8::Local<v8::Value> val;
            V8_CHECK(obj->Get(ctx, V8Helpers::RGBA_RKey(isolate)).ToLocal(&val), "Invalid object value at key 'r'");
            V8_CHECK(val->IsNumber(), "Invalid object value type at key 'r'");
            r = val->Int32Value(ctx).FromMaybe(0);
            V8_CHECK(obj->Get(ctx, V8Helpers::RGBA_GKey(isolate)).ToLocal(&val), "Invalid object value at key 'g'");
            V8_CHECK(val->IsNumber(), "Invalid object value type at key 'g'");
            g = val->Int32Value(ctx).FromMaybe(0);
            V8_CHECK(obj->Get(ctx, V8Helpers::RGBA_BKey(isolate)).ToLocal(&val), "Invalid object value at key 'b'");
            V8_CHECK(val->IsNumber(), "Invalid object value type at key 'b'");
            b = val->Int32Value(ctx).FromMaybe(0);

            if(obj->Has(ctx, V8Helpers::RGBA_AKey(isolate)).FromMaybe(false))
            {
                V8_CHECK(obj->Get(ctx, V8Helpers::RGBA_AKey(isolate)).ToLocal(&val), "Invalid object value at key 'a'");
                V8_CHECK(val->IsNumber(), "Invalid object value type at key 'a'");
                a = val->Int32Value(ctx).FromMaybe(255);
            }
            else
                a = 255;
        }
        else
        {
            V8Helpers::Throw(isolate, "Invalid arguments");
            return;
        }
    }
    else if(info.Length() >= 3)
    {
        V8_ARG_TO_INT32(1, red);
        V8_ARG_TO_INT32(2, green);
        V8_ARG_TO_INT32(3, blue);
        r = red;
        g = green;
        b = blue;
    }
    if(info.Length() == 4)
    {
        V8_ARG_TO_INT32(4, alpha);
        a = alpha;
    }

    V8_CHECK(r >= 0 && r < 256, "Invalid RGBA R value. Allowed is 0 - 255");
    V8_CHECK(g >= 0 && g < 256, "Invalid RGBA G value. Allowed is 0 - 255");
    V8_CHECK(b >= 0 && b < 256, "Invalid RGBA B value. Allowed is 0 - 255");
    V8_CHECK(a >= 0 && a < 256, "Invalid RGBA A value. Allowed is 0 - 255");

    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_RKey(isolate), V8Helpers::JSValue(r), v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_GKey(isolate), V8Helpers::JSValue(g), v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_BKey(isolate), V8Helpers::JSValue(b), v8::PropertyAttribute::ReadOnly);
    V8Helpers::DefineOwnProperty(isolate, ctx, info.This(), V8Helpers::RGBA_AKey(isolate), V8Helpers::JSValue(a), v8::PropertyAttribute::ReadOnly);
}

extern V8Class v8RGBA("RGBA",
                      &Constructor,
                      [](v8::Local<v8::FunctionTemplate> tpl)
                      {
                          v8::Isolate* isolate = v8::Isolate::GetCurrent();

                          tpl->InstanceTemplate()->SetInternalFieldCount(1);  // !! Needs to be set so V8 knows its a custom class !!
                      });
