#include "V8Helpers.h"

// clang-format off
extern V8Class v8Entity;
extern V8Class v8NetworkObject("NetworkObject", v8Entity, [](v8::Local<v8::FunctionTemplate> tpl)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
});
