#include "Globals.h"
#include "CWorker.h"
#include "V8Helpers.h"

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

void Log(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);

    std::stringstream ss;
    for(int i = 0; i < info.Length(); ++i)
    {
        v8::Local<v8::Value> val = info[i];

        if(i > 0) ss << " ";

        auto str = V8::Stringify(ctx, val);
        if(str.IsEmpty()) continue;

        ss << str.CStr();
    }

    alt::ICore::Instance().LogColored(ss.str());
}

void LogWarning(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);

    std::stringstream ss;
    for(int i = 0; i < info.Length(); ++i)
    {
        v8::Local<v8::Value> val = info[i];

        if(i > 0) ss << " ";

        auto str = V8::Stringify(ctx, val);
        if(str.IsEmpty()) continue;

        ss << str.CStr();
    }

    alt::ICore::Instance().LogWarning(ss.str());
}

void LogError(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);

    std::stringstream ss;
    for(int i = 0; i < info.Length(); ++i)
    {
        v8::Local<v8::Value> val = info[i];

        if(i > 0) ss << " ";

        auto str = V8::Stringify(ctx, val);
        if(str.IsEmpty()) continue;

        ss << str.CStr();
    }

    alt::ICore::Instance().LogError(ss.str());
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
