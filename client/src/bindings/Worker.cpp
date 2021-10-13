#include "V8Helpers.h"
#include "V8BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"

#include "./workers/CWorker.h"

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, path);

    info.This()->SetInternalField(0, v8::External::New(isolate, new CWorker(path.ToString())));
}

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);

    std::ostringstream stream;
    stream << "Worker{ file: " << worker->GetFilePath() << " }";
    V8_RETURN_STRING(stream.str().c_str());
}

static void ValidGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);

    V8_RETURN_BOOLEAN(worker == nullptr);
}

static void Destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);

    V8_CHECK(worker, "This worker is already destroyed");
    worker->Destroy();
    info.This()->SetInternalField(0, v8::External::New(isolate, nullptr));
}

extern V8Class v8Worker("Worker", &Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    V8::SetMethod(isolate, tpl, "toString", ToString);
    V8::SetAccessor(isolate, tpl, "valid", ValidGetter);
    V8::SetMethod(isolate, tpl, "destroy", Destroy);
});
