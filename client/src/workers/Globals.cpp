#include "Globals.h"
#include "CWorker.h"
#include "V8Helpers.h"
#include "V8Module.h"

void Emit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

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
    worker->GetMainEventHandler().Emit(eventName, args);
}

void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->GetWorkerEventHandler().Subscribe(eventName, callback);
}

void Off(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->GetWorkerEventHandler().Unsubscribe(eventName, callback);
}

void Once(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->GetWorkerEventHandler().Subscribe(eventName, callback, true);
}

void NextTick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_FUNCTION(1, callback);

    V8_RETURN_INT(worker->CreateTimer(callback, 0, true, V8Helpers::SourceLocation::GetCurrent(isolate, nullptr)));
}

void SetTimeout(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_FUNCTION(1, callback);
    V8_ARG_TO_UINT(2, time);

    V8_RETURN_INT(worker->CreateTimer(callback, time, true, V8Helpers::SourceLocation::GetCurrent(isolate, nullptr)));
}

void SetInterval(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_FUNCTION(1, callback);
    V8_ARG_TO_UINT(2, time);

    V8_RETURN_INT(worker->CreateTimer(callback, time, false, V8Helpers::SourceLocation::GetCurrent(isolate, nullptr)));
}

void ClearTimer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_UINT(1, timer);

    worker->RemoveTimer(timer);
}

void GetSharedArrayBuffer(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_UINT(1, index);

    auto buffer = worker->GetSharedArrayBuffer(isolate, index);
    V8_CHECK(!buffer.IsEmpty(), "Invalid shared array buffer index");
    V8_RETURN(buffer);
}

extern V8Module altModule;
extern V8Class v8File, v8RGBA, v8Vector2, v8Vector3, v8Utils, v8Resource;
extern V8Module altWorker("alt-worker",
                          nullptr,
                          { v8File, v8RGBA, v8Vector2, v8Vector3, v8Utils, v8Resource },
                          [](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)
                          {
                              v8::Isolate* isolate = ctx->GetIsolate();

                              auto alt = altModule.GetExports(isolate, ctx);
                              auto InheritFromAlt = [&](const char* name)
                              {
                                  auto original = alt->Get(ctx, V8Helpers::JSValue(name));
                                  if(original.IsEmpty()) return;
                                  exports->Set(ctx, V8Helpers::JSValue(name), original.ToLocalChecked());
                              };

                              V8Helpers::RegisterFunc(exports, "emit", &Emit);
                              V8Helpers::RegisterFunc(exports, "on", &On);
                              V8Helpers::RegisterFunc(exports, "off", &Off);
                              V8Helpers::RegisterFunc(exports, "once", &Once);
                              V8Helpers::RegisterFunc(exports, "nextTick", &NextTick);
                              V8Helpers::RegisterFunc(exports, "setInterval", &SetInterval);
                              V8Helpers::RegisterFunc(exports, "setTimeout", &SetTimeout);
                              V8Helpers::RegisterFunc(exports, "clearNextTick", &ClearTimer);
                              V8Helpers::RegisterFunc(exports, "clearInterval", &ClearTimer);
                              V8Helpers::RegisterFunc(exports, "clearTimeout", &ClearTimer);
                              V8Helpers::RegisterFunc(exports, "clearTimer", &ClearTimer);
                              V8Helpers::RegisterFunc(exports, "getSharedArrayBuffer", &::GetSharedArrayBuffer);

                              V8_OBJECT_SET_BOOLEAN(exports, "isWorker", true);

                              // *** All inherited functions from the alt module
                              // Logging
                              InheritFromAlt("log");
                              InheritFromAlt("logWarning");
                              InheritFromAlt("logError");

                              // General functions
                              InheritFromAlt("hash");

                              // Properties
                              InheritFromAlt("version");
                              InheritFromAlt("branch");
                              InheritFromAlt("sdkVersion");
                              InheritFromAlt("debug");
                          });

// Empty wrapper module
extern V8Module altWorkerNatives("natives", nullptr, {}, [](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports) {});
