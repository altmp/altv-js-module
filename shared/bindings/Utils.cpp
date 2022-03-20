#include "../V8Class.h"

// Bindings are added here from JS
extern V8Class v8Utils("Utils", [](v8::Local<v8::FunctionTemplate> tpl) {});
