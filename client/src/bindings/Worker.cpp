#include "V8Helpers.h"
#include "V8BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"

#include "../workers/CWorker.h"

static constexpr int MAX_WORKERS_PER_RESOURCE = 20;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, path);

    V8_CHECK(static_cast<CV8ResourceImpl*>(resource)->GetWorkerCount() < MAX_WORKERS_PER_RESOURCE, "Maximum amount of workers per resource reached");

    // todo: use a proper way to get the current path
    auto stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 1);
    alt::String origin = *v8::String::Utf8Value(isolate, stackTrace->GetFrame(isolate, 0)->GetScriptName());

    auto worker = new CWorker(path, origin, static_cast<CV8ResourceImpl*>(resource));
    info.This()->SetInternalField(0, v8::External::New(isolate, worker));
    static_cast<CV8ResourceImpl*>(resource)->AddWorker(worker);
}

static void ToString(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

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

static void FilePathGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_RETURN_STRING(worker->GetFilePath().c_str());
}

static void Start(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(!worker->IsReady(), "Worker is already started");
    worker->Start();
}

static void Destroy(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    worker->Destroy();
    info.This()->SetInternalField(0, v8::External::New(isolate, nullptr));
    static_cast<CV8ResourceImpl*>(resource)->RemoveWorker(worker);
}

static void Emit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(worker->IsReady(), "The worker is not ready yet, wait for the 'load' event");

    V8_ARG_TO_STRING(1, eventName);

    std::vector<alt::MValue> args;
    args.reserve(info.Length() - 1);
    for(int i = 1; i < info.Length(); i++)
    {
        V8_ARG_TO_MVALUE(i + 1, arg);
        args.push_back(arg);
    }
    worker->EmitToWorker(eventName.ToString(), args);
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->SubscribeToMain(eventName.ToString(), callback);
}

static void Once(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->SubscribeToMain(eventName.ToString(), callback, true);
}

static void IsPausedGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_RETURN_BOOLEAN(worker->IsPaused());
}

static void Pause(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(!worker->IsPaused(), "The worker is already paused");
    worker->Pause();
}

static void Resume(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(worker->IsPaused(), "The worker is not paused");
    worker->Resume();
}

extern V8Class v8Worker("Worker", &Constructor, [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    tpl->Set(V8::JSValue("maxWorkers"), V8::JSValue(MAX_WORKERS_PER_RESOURCE), v8::PropertyAttribute::ReadOnly);

    V8::SetMethod(isolate, tpl, "toString", ToString);
    V8::SetAccessor(isolate, tpl, "valid", ValidGetter);
    V8::SetAccessor(isolate, tpl, "filePath", FilePathGetter);

    V8::SetMethod(isolate, tpl, "start", Start);
    V8::SetMethod(isolate, tpl, "destroy", Destroy);

    V8::SetMethod(isolate, tpl, "emit", Emit);
    V8::SetMethod(isolate, tpl, "on", On);
    V8::SetMethod(isolate, tpl, "once", Once);

    V8::SetAccessor(isolate, tpl, "isPaused", IsPausedGetter);
    V8::SetMethod(isolate, tpl, "pause", Pause);
    V8::SetMethod(isolate, tpl, "resume", Resume);
});
