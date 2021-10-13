#include "Globals.h"
#include "CWorker.h"
#include "V8Helpers.h"

void Emit(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN_MIN(1);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);

    V8_ARG_TO_STRING(1, eventName);

    std::vector<alt::MValue> args(info.Length() - 1);
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
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->SubscribeToWorker(eventName.ToString(), callback);
}

void Once(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN(2);
    V8_GET_THIS_INTERNAL_FIELD_EXTERNAL(1, worker, CWorker);

    V8_ARG_TO_STRING(1, eventName);
    V8_ARG_TO_FUNCTION(2, callback);

    worker->SubscribeToWorker(eventName.ToString(), callback, true);
}
