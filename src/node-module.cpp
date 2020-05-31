#include "stdafx.h"

#include "helpers/V8Module.h"
#include "CNodeScriptRuntime.h"

/*static void NodeStop()
{
	{
		//v8::Isolate::Scope isolateScope(isolate);
		//v8::SealHandleScope seal(isolate);
		//v8::platform::PumpMessageLoop(platform, isolate);
		//node::EmitBeforeExit(m_env);

		//if (uv_loop_alive(m_env->event_loop())) {
		//	uv_run(m_env->event_loop(), UV_RUN_NOWAIT);
		//}
	}

	//node::EmitExit(m_env);
	//node::RunAtExit(m_env);
	//node::FreeEnvironment(m_env);

	//isolate->Dispose();
	//v8::V8::Dispose();
}*/

static void Initialize(v8::Local<v8::Object> exports)
{
	v8::Isolate* isolate = v8::Isolate::GetCurrent();
	v8::HandleScope handle_scope(isolate);

	V8Module::All().at("alt")->Register(isolate, isolate->GetEnteredContext(), exports);
}
NODE_MODULE_LINKED(alt, Initialize)

static void CommandHandler(alt::StringView cmd, alt::Array<alt::StringView> args, void* userData)
{
	if (args.GetSize() > 0 && args[0] == "--version")
	{
		Log::Colored << "~ly~" NODEMODULE_VERSION << Log::Endl;
		Log::Colored << "~ly~" NODEMODULE_COPYRIGHT << Log::Endl;

		Log::Colored << "~ly~" NODEJS_VERSION << Log::Endl;
		Log::Colored << "~ly~" NODEJS_COPYRIGHT << Log::Endl;
	}
	else if (args.GetSize() > 0 && args[0] == "--help")
	{
		Log::Colored << "~y~Usage: ~w~node-module [options]" << Log::Endl;
		Log::Colored << "~y~Options:" << Log::Endl;
		Log::Colored << "  ~ly~--help    ~w~- this message." << Log::Endl;
		Log::Colored << "  ~ly~--version ~w~- version info." << Log::Endl;
	}
	else
	{
		Log::Colored << "~y~Usage: ~w~node-module [options]" << Log::Endl;
		Log::Colored << "  Use: ~ly~\"node-module --help\" ~w~for more info" << Log::Endl;
	}
}

EXPORT uint32_t GetSDKVersion()
{
	return alt::ICore::SDK_VERSION;
}

EXPORT bool altMain(alt::ICore* _core)
{
	alt::ICore::SetInstance(_core);

	auto& apiCore = alt::ICore::Instance();
	auto& runtime = CNodeScriptRuntime::Instance();

	apiCore.RegisterScriptRuntime("js", &runtime);
	apiCore.SubscribeCommand("node-module", &CommandHandler);

	return true;
}