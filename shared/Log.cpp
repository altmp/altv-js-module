#include "Log.h"

#ifdef ALT_CLIENT_API
    #include "CV8ScriptRuntime.h"
#else
    #include "CNodeScriptRuntime.h"
#endif

Log& Log::Endl(Log& log)
{
    v8::Isolate* isolate = nullptr;
#ifdef ALT_CLIENT_API
    isolate = CV8ScriptRuntime::Instance().GetIsolate();
#else
    isolate = CNodeScriptRuntime::Instance().GetIsolate();
#endif
    v8::Local<v8::Context> ctx;
    if(isolate) ctx = isolate->GetEnteredOrMicrotaskContext();
    V8ResourceImpl* v8Resource = !ctx.IsEmpty() ? V8ResourceImpl::Get(ctx) : nullptr;
    alt::IResource* resource = v8Resource ? v8Resource->GetResource() : nullptr;

    switch(log.type)
    {
        case INFO: alt::ICore::Instance().LogInfo(log.buf.str(), resource); break;
        case DEBUG: alt::ICore::Instance().LogDebug(log.buf.str().c_str(), resource); break;
        case WARNING: alt::ICore::Instance().LogWarning(log.buf.str().c_str(), resource); break;
        case ERR: alt::ICore::Instance().LogError(log.buf.str().c_str(), resource); break;
        case COLORED: alt::ICore::Instance().LogColored(log.buf.str().c_str(), resource); break;
    }

    log.buf.str("");
    return log;
}
