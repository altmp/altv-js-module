#pragma once

#include "v8.h"
#include "v8-inspector.h"
#include "libplatform/libplatform.h"

#include "cpp-sdk/IScriptRuntime.h"

#include "CV8Resource.h"

class CV8ScriptRuntime : public alt::IScriptRuntime
{
	static constexpr char inspectorViewStr[] = "alt:V Multiplayer";

	std::unique_ptr<v8::Platform> platform;
	v8::Isolate::CreateParams create_params;
	v8::Isolate *isolate;
	std::unique_ptr<v8_inspector::V8InspectorClient> inspectorClient;
	std::unique_ptr<v8_inspector::V8Inspector::Channel> inspectorChannel;
	std::unique_ptr<v8_inspector::V8Inspector> inspector;
	std::unique_ptr<v8_inspector::V8InspectorSession> inspectorSession;

public:
	CV8ScriptRuntime();

	v8::Isolate *GetIsolate() const { return isolate; }

	v8_inspector::V8Inspector *GetInspector() const { return inspector.get(); }

	alt::IResource::Impl *CreateImpl(alt::IResource *resource) override
	{
		return new CV8ResourceImpl(resource, isolate);
	}

	void DestroyImpl(alt::IResource::Impl *impl) override
	{
		delete static_cast<CV8ResourceImpl *>(impl);
	}

	void OnTick() override
	{
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope handle_scope(isolate);

		v8::platform::PumpMessageLoop(platform.get(), isolate);
	}

	void HeapBenchmark()
	{
		v8::HeapStatistics heapStats;
		isolate->GetHeapStatistics(&heapStats);
		Log::Info << "================ Heap benchmark info =================" << Log::Endl;
		Log::Info << "total_heap_size = " << formatBytes(heapStats.total_heap_size()) << Log::Endl;
		Log::Info << "total_heap_size_executable = " << formatBytes(heapStats.total_heap_size_executable()) << Log::Endl;
		Log::Info << "total_physical_size = " << formatBytes(heapStats.total_physical_size()) << Log::Endl;
		Log::Info << "total_available_size = " << formatBytes(heapStats.total_available_size()) << Log::Endl;
		Log::Info << "used_heap_size = " << formatBytes(heapStats.used_heap_size()) << Log::Endl;
		Log::Info << "heap_size_limit = " << formatBytes(heapStats.heap_size_limit()) << Log::Endl;
		Log::Info << "malloced_memory = " << formatBytes(heapStats.malloced_memory()) << Log::Endl;
		Log::Info << "external_memory = " << formatBytes(heapStats.external_memory()) << Log::Endl;
		Log::Info << "peak_malloced_memory = " << formatBytes(heapStats.peak_malloced_memory()) << Log::Endl;
		Log::Info << "number_of_native_contexts = " << heapStats.number_of_native_contexts() << Log::Endl;
		Log::Info << "number_of_detached_contexts = " << heapStats.number_of_detached_contexts() << Log::Endl;
		Log::Info << "======================================================" << Log::Endl;
	}

	~CV8ScriptRuntime()
	{
		isolate->Dispose();
		v8::V8::Dispose();
		v8::V8::ShutdownPlatform();
		delete create_params.array_buffer_allocator;
	}

	static v8::MaybeLocal<v8::Module> ResolveModule(v8::Local<v8::Context> ctx, v8::Local<v8::String> specifier, v8::Local<v8::Module> referrer)
	{
		auto isolate = ctx->GetIsolate();
		V8ResourceImpl *resource = V8ResourceImpl::Get(ctx);
		if (!resource)
		{
			isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(ctx->GetIsolate(), "Invalid resource").ToLocalChecked()));
			return v8::MaybeLocal<v8::Module>{};
		}

		std::string _specifier = *v8::String::Utf8Value{isolate, specifier};
		if(_specifier == resource->GetResource()->GetName().ToString())
		{
			isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(ctx->GetIsolate(), "Cannot import the resource itself (self-importing)").ToLocalChecked()));
			return v8::MaybeLocal<v8::Module>{};
		}

		return static_cast<CV8ResourceImpl *>(resource)->ResolveModule(_specifier, referrer);
	}

	static std::string formatBytes(uint64_t bytes)
	{
		static std::string result = "";
		const char *sizes[5] = {"bytes", "KB", "MB", "GB", "TB"};

		if (bytes == 0)
		{
			result = "0 bytes";
			return result;
		}
		else if (bytes == 1)
		{
			result = "1 byte";
			return result;
		}

		uint64_t left = std::floor(std::log(bytes) / std::log(1024));

		if (left == 0)
		{
			std::stringstream ss;
			ss << bytes + " bytes";
			result = ss.str();
			return result;
		}
		else
		{
			std::stringstream ss;
			ss << std::setprecision(2) << std::fixed << (bytes / std::pow(1024, left)) << " " << sizes[left];
			result = ss.str();
			return result;
		}
	};
};