#include "V8Helpers.h"
#include "helpers/BindHelpers.h"
#include "V8Class.h"
#include "V8Entity.h"
#include "V8ResourceImpl.h"
#include "../CV8Resource.h"
#include "../CV8ScriptRuntime.h"

#include "../workers/CWorker.h"

static constexpr uint32_t MAX_WORKERS = 10;

static void Constructor(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    // Deprecation added: 11/11/2022 (version 13)
    Log::Warning << "alt.Worker is deprecated and will be removed in future versions." << Log::Endl;

    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_CONSTRUCTOR();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_STRING(1, path);

    std::string origin = V8Helpers::GetCurrentSourceOrigin(isolate);

    alt::IPackage::PathInfo pathInfo = alt::ICore::Instance().Resolve(resource->GetResource(), path, origin);
    V8_CHECK(pathInfo.pkg, "Worker file does not exist");

    std::string filePath = pathInfo.prefix + pathInfo.fileName;

    CWorker* worker = new CWorker(filePath, static_cast<CV8ResourceImpl*>(resource));
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
    V8_RETURN_STRING(stream.str());
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

    V8_RETURN_STRING(worker->GetFilePath());
}

static void Start(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(!worker->IsReady(), "Worker is already started");
    V8_CHECK(CV8ScriptRuntime::Instance().GetActiveWorkerCount() < MAX_WORKERS, "Maximum amount of running workers reached");
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

    V8_ARG_TO_STRING(1, eventName);

    std::vector<V8Helpers::Serialization::Value> args;
    args.reserve(info.Length() - 1);
    for(int i = 1; i < info.Length(); i++)
    {
        auto arg = V8Helpers::Serialization::Serialize(ctx, info[i]);
        if(!arg.Valid())
        {
            V8Helpers::Throw(isolate, "Invalid argument");
            return;
        }
        args.push_back(arg);
    }
    worker->GetWorkerEventHandler().Emit(eventName, args);
}

static void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->GetMainEventHandler().Subscribe(eventName, callback);
}

static void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->GetMainEventHandler().Unsubscribe(eventName, callback);
}

static void Once(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->GetMainEventHandler().Subscribe(eventName, callback, true);
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
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(!worker->IsPaused(), "The worker is already paused");
    worker->Pause();
    CV8ScriptRuntime::Instance().RemoveActiveWorker();
}

static void Resume(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);
    V8_CHECK(worker, "Worker is invalid");

    V8_CHECK(worker->IsPaused(), "The worker is not paused");
    V8_CHECK(CV8ScriptRuntime::Instance().GetActiveWorkerCount() < MAX_WORKERS, "Maximum amount of running workers reached");
    worker->Resume();
    CV8ScriptRuntime::Instance().AddActiveWorker();
}

static void AddSharedArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_CHECK(info[0]->IsSharedArrayBuffer(), "Failed to convert argument 1 to SharedArrayBuffer");
    auto buffer = info[0].As<v8::SharedArrayBuffer>();

    CWorker::BufferId index = CWorker::AddSharedArrayBuffer(buffer);
    V8_RETURN(index);
}

static void RemoveSharedArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);

    V8_ARG_TO_UINT(1, index);

    auto result = CWorker::RemoveSharedArrayBuffer(index);
    V8_CHECK(result, "Invalid shared array buffer index");
}

static void ActiveWorkersGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE();
    V8_RETURN(CV8ScriptRuntime::Instance().GetActiveWorkerCount());
}

extern V8Class v8Worker("Worker",
                        &Constructor,
                        [](v8::Local<v8::FunctionTemplate> tpl)
                        {
                            v8::Isolate* isolate = v8::Isolate::GetCurrent();
                            tpl->InstanceTemplate()->SetInternalFieldCount(1);

                            tpl->Set(V8Helpers::JSValue("maxWorkers"), V8Helpers::JSValue(MAX_WORKERS), v8::PropertyAttribute::ReadOnly);
                            V8Helpers::SetStaticAccessor(isolate, tpl, "activeWorkers", &ActiveWorkersGetter);

                            V8Helpers::SetStaticMethod(isolate, tpl, "addSharedArrayBuffer", AddSharedArrayBuffer);
                            V8Helpers::SetStaticMethod(isolate, tpl, "removeSharedArrayBuffer", RemoveSharedArrayBuffer);

                            V8Helpers::SetMethod(isolate, tpl, "toString", ToString);
                            V8Helpers::SetAccessor(isolate, tpl, "valid", ValidGetter);
                            V8Helpers::SetAccessor(isolate, tpl, "filePath", FilePathGetter);

                            V8Helpers::SetMethod(isolate, tpl, "start", Start);
                            V8Helpers::SetMethod(isolate, tpl, "destroy", Destroy);

                            V8Helpers::SetMethod(isolate, tpl, "emit", Emit);
                            V8Helpers::SetMethod(isolate, tpl, "on", On);
                            V8Helpers::SetMethod(isolate, tpl, "off", Off);
                            V8Helpers::SetMethod(isolate, tpl, "once", Once);

                            V8Helpers::SetAccessor(isolate, tpl, "isPaused", IsPausedGetter);
                            V8Helpers::SetMethod(isolate, tpl, "pause", Pause);
                            V8Helpers::SetMethod(isolate, tpl, "resume", Resume);
                        });
