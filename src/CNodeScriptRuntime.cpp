#include "stdafx.h"

#include "CNodeScriptRuntime.h"

CNodeScriptRuntime::CNodeScriptRuntime()
{
	int eac;
	const char** eav;

	const char* argv[] = { "alt-server", "--experimental-modules", "--es-module-specifier-resolution=node", "--trace-warnings" };
	int argc = sizeof(argv) / sizeof(const char*);

	node::Init(&argc, argv, &eac, &eav);

	auto* tracing_agent = node::CreateAgent();
	auto* tracing_controller = tracing_agent->GetTracingController();
	node::tracing::TraceEventHelper::SetAgent(tracing_agent);
	platform.reset(node::CreatePlatform(4, tracing_controller));

	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	isolate = v8::Isolate::Allocate();

	platform->RegisterIsolate(isolate, uv_default_loop());

	v8::Isolate::CreateParams params;
	params.array_buffer_allocator = node::CreateArrayBufferAllocator();

	v8::Isolate::Initialize(isolate, params);

	{
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope handle_scope(isolate);

		V8Class::LoadAll(isolate);
	}
}

CNodeScriptRuntime::~CNodeScriptRuntime()
{

}

void CNodeScriptRuntime::OnTick()
{
	v8::Locker locker(isolate);
	v8::Isolate::Scope isolateScope(isolate);
	v8::SealHandleScope seal(isolate);

	uv_run(uv_default_loop(), UV_RUN_NOWAIT);
	platform->DrainTasks(isolate);
}

void CNodeScriptRuntime::OnDispose()
{
	/*{
		v8::SealHandleScope seal(isolate);

		do {
			uv_run(uv_default_loop(), UV_RUN_DEFAULT);
			platform->DrainTasks(isolate);
		} while (uv_loop_alive(uv_default_loop()));
	}*/


	v8::V8::Dispose();
	v8::V8::ShutdownPlatform();

	//node::FreePlatform(platform.release());
}
