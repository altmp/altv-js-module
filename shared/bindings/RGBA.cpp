
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
            v8::Local<v8::Array> arr = info[0].As<v8::Array>();
            V8_CHECK(arr->Length() >= 3, "Array has to contain atleast 3 (R, G, B) values");
            std::optional<std::vector<int>> values = V8Helpers::CppValue<int>(arr);
            V8_CHECK(values.has_value(), "Invalid array passed");
            r = values->at(0);
            g = values->at(1);
            b = values->at(2);
            if(values->size() > 3) a = values->at(3);
        }
        else if(info[0]->IsObject())
        {
            v8::Local<v8::Object> obj = info[0].As<v8::Object>();
            std::optional<std::unordered_map<std::string, int>> values = V8Helpers::CppValue<int>(obj);
            V8_CHECK(values.has_value(), "Invalid object passed");
            r = values->at("r");
            g = values->at("g");
            b = values->at("b");
            if(values->contains("a")) a = values->at("a");
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
