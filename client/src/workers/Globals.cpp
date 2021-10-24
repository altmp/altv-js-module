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

    std::vector<alt::MValue> args;
    args.reserve(info.Length() - 1);
    for(int i = 1; i < info.Length(); i++)
    {
        V8_ARG_TO_MVALUE(i + 1, arg);
        args.push_back(arg);
    }
    worker->EmitToMain(eventName.ToString(), args);
}

void On(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->SubscribeToWorker(eventName.ToString(), callback);
}

void Once(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->SubscribeToWorker(eventName.ToString(), callback, true);
}

void NextTick(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(1);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_FUNCTION(1, callback);

    V8_RETURN_INT(worker->CreateTimer(callback, 0, true, V8::SourceLocation::GetCurrent(isolate)));
}

void SetTimeout(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_FUNCTION(1, callback);
    V8_ARG_TO_UINT(2, time);

    V8_RETURN_INT(worker->CreateTimer(callback, time, true, V8::SourceLocation::GetCurrent(isolate)));
}

void SetInterval(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    auto worker = static_cast<CWorker*>(ctx->GetAlignedPointerFromEmbedderData(2));

    V8_ARG_TO_FUNCTION(1, callback);
    V8_ARG_TO_UINT(2, time);

    V8_RETURN_INT(worker->CreateTimer(callback, time, false, V8::SourceLocation::GetCurrent(isolate)));
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

extern V8Module sharedModule;
extern V8Class v8File, v8WebSocketClient, v8HttpClient;
extern V8Module altWorker("alt-worker", nullptr, { v8File, v8WebSocketClient, v8HttpClient }, [](v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports) {
    V8Helpers::RegisterFunc(exports, "emit", &Emit);
    V8Helpers::RegisterFunc(exports, "on", &On);
    V8Helpers::RegisterFunc(exports, "once", &Once);
    V8Helpers::RegisterFunc(exports, "nextTick", &NextTick);
    V8Helpers::RegisterFunc(exports, "setInterval", &SetInterval);
    V8Helpers::RegisterFunc(exports, "setTimeout", &SetTimeout);
    V8Helpers::RegisterFunc(exports, "clearNextTick", &ClearTimer);
    V8Helpers::RegisterFunc(exports, "clearInterval", &ClearTimer);
    V8Helpers::RegisterFunc(exports, "clearTimeout", &ClearTimer);
    V8Helpers::RegisterFunc(exports, "getSharedArrayBuffer", &::GetSharedArrayBuffer);

    auto alt = sharedModule.GetExports(ctx->GetIsolate(), ctx);
    exports->Set(ctx, V8::JSValue("log"), alt->Get(ctx, V8::JSValue("log")).ToLocalChecked());
    exports->Set(ctx, V8::JSValue("logWarning"), alt->Get(ctx, V8::JSValue("logWarning")).ToLocalChecked());
    exports->Set(ctx, V8::JSValue("logError"), alt->Get(ctx, V8::JSValue("logError")).ToLocalChecked());
    exports->Set(ctx, V8::JSValue("takeScreenshot"), alt->Get(ctx, V8::JSValue("takeScreenshot")).ToLocalChecked());
    exports->Set(ctx, V8::JSValue("takeScreenshotGameOnly"), alt->Get(ctx, V8::JSValue("takeScreenshotGameOnly")).ToLocalChecked());
});
