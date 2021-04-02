#include "cpp-sdk/SDK.h"
#include "CV8ScriptRuntime.h"
#include "helpers/Log.h"

#ifdef ALTV_JS_SHARED
#define ALTV_JS_EXPORT extern "C" __declspec(dllexport)
#else
#define ALTV_JS_EXPORT extern "C"
#endif

static void ClientJSCommand(alt::Array<alt::StringView> args, void*)
{
    Log::Colored << "~r~--------------------------------" << Log::Endl;
    Log::Colored << "~y~Client JS module - Powered by V8" << Log::Endl;
    Log::Colored << "V8 version: ~g~v" << V8_MAJOR_VERSION << "." << V8_MINOR_VERSION << Log::Endl;
    Log::Colored << "SDK version: ~g~v" << alt::ICore::SDK_VERSION << Log::Endl;
    Log::Colored << "~r~--------------------------------" << Log::Endl;
}

ALTV_JS_EXPORT void CreateScriptRuntime(alt::ICore *core)
{
    alt::ICore::SetInstance(core);
    auto runtime = new CV8ScriptRuntime();
    core->RegisterScriptRuntime("js", runtime);

    core->SubscribeCommand("clientjs", &ClientJSCommand);
}

ALTV_JS_EXPORT const char* GetType()
{
    return "js";
}

ALTV_JS_EXPORT uint32_t GetSDKVersion()
{
    return alt::ICore::SDK_VERSION;
}
