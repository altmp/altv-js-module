
#include "CV8ScriptRuntime.h"
#include "inspector/CV8InspectorClient.h"
#include "inspector/CV8InspectorChannel.h"
#include "helpers/V8Module.h"

v8::Persistent<v8::Function> host_import_module_dynamically_callback;

CV8ScriptRuntime::CV8ScriptRuntime()
{
	CV8ScriptRuntime::SetInstance(this);
	platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

	isolate = v8::Isolate::New(create_params);
	isolate->SetFatalErrorHandler([](const char *location, const char *message) {
		Log::Error << "[V8] " << location << ": " << message << Log::Endl;
	});

	isolate->SetPromiseRejectCallback([](v8::PromiseRejectMessage message) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Value> value = message.GetValue();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		CV8ResourceImpl *resource = static_cast<CV8ResourceImpl *>(V8ResourceImpl::Get(ctx));
		if (resource)
		{
			if (message.GetEvent() == v8::PromiseRejectEvent::kPromiseRejectWithNoHandler)
			{
				resource->OnPromiseRejectedWithNoHandler(message);
			}
			else if (message.GetEvent() == v8::PromiseRejectEvent::kPromiseHandlerAddedAfterReject)
			{
				resource->OnPromiseHandlerAdded(message);
			}
			else if (message.GetEvent() == v8::PromiseRejectEvent::kPromiseRejectAfterResolved)
			{
				Log::Warning << "[V8] Promise rejected after being resolved in resource "
							 << resource->GetResource()->GetName() << " ("
							 << *v8::String::Utf8Value(isolate, value->ToString(ctx).ToLocalChecked())
							 << ")" << Log::Endl;
			}
			else if (message.GetEvent() == v8::PromiseRejectEvent::kPromiseResolveAfterResolved)
			{
				Log::Warning << "[V8] Promise resolved after being resolved in resource "
							 << resource->GetResource()->GetName() << " ("
							 << *v8::String::Utf8Value(isolate, value->ToString(ctx).ToLocalChecked())
							 << ")" << Log::Endl;
			}
		}
		else
		{
			Log::Error << "You're not supposed to ever see this";
		}
	});

	static std::list<v8::UniquePersistent<v8::Promise::Resolver>> promises;
	isolate->SetHostImportModuleDynamicallyCallback([](v8::Local<v8::Context> context, v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier)
	{
		v8::Isolate* isolate = context->GetIsolate();
		auto promise = v8::Promise::Resolver::New(context);
		/*auto &persistent = promises.emplace_back(v8::UniquePersistent<v8::Promise::Resolver>(isolate, promise.ToLocalChecked()));

		auto result = CV8ScriptRuntime::ResourcesLoadedResult(context, referrer, specifier, &persistent,
			[](v8::Local<v8::Context> context, v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier, const void* original)
			{
				v8::Isolate* isolate = v8::Isolate::GetCurrent();
				v8::Locker locker(isolate);
				v8::Isolate::Scope isolateScope(isolate);
				v8::HandleScope handleScope(isolate);
				auto persistent = (v8::UniquePersistent<v8::Promise::Resolver>*)original;
				auto resolver = persistent->Get(isolate);
				auto ctx = context;

				v8::Context::Scope ctxscope(ctx);
				v8::String::Utf8Value utfValue(isolate, specifier);
				std::string name(*utfValue);
				V8ResourceImpl* resource = V8ResourceImpl::Get(ctx);

				auto module = static_cast<CV8ResourceImpl*>(resource)->GetModuleFromName(name, isolate);
				if(module.IsEmpty())
					resolver->Reject(ctx, v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Module could not be found").ToLocalChecked()));
				auto resolved = CV8ScriptRuntime::ResolveModule(ctx, specifier, module);
				Log::Info << "Resolved" << Log::Endl;	
				v8::Local<v8::Module> outModule;

				if (resolved.IsEmpty() || !resolved.ToLocal(&outModule)) {
					Log::Info << "FuckNo" << Log::Endl;
					resolver->Reject(ctx, v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Module could not be found").ToLocalChecked()));
					Log::Info << "Fuck" << Log::Endl;
				}
				else
				{
					Log::Info << "Status: " << std::to_string(outModule->GetStatus()) << Log::Endl;
					if (outModule->GetStatus() != v8::Module::Status::kInstantiated) 
						outModule->InstantiateModule(ctx, CV8ScriptRuntime::ResolveModule);
					Log::Info << "Status2: " << std::to_string(outModule->GetStatus()) << Log::Endl;
					if (outModule->GetStatus() != v8::Module::Status::kEvaluated) 
						outModule->Evaluate(ctx);
					Log::Info << "Status3: " << std::to_string(outModule->GetStatus()) << Log::Endl;
					//resolver->Resolve(ctx, outModule->GetModuleNamespace());
					Log::Info << "Ctx: " << std::to_string(ctx.IsEmpty()) << Log::Endl;
					Log::Info << "Isolate: " << std::to_string(isolate == nullptr) << Log::Endl;
					auto str = v8::String::NewFromUtf8(isolate, "Fuck").ToLocalChecked();
					Log::Info << "test" << Log::Endl;
					resolver->Resolve(ctx, str);
				}
				promises.remove(*persistent);
			});
		CV8ScriptRuntime::Instance().OnAllResourcesLoaded(result);*/

		v8::Local<v8::Promise::Resolver> resolver;
		if (!promise.ToLocal(&resolver))
			return v8::MaybeLocal<v8::Promise>();

		v8::String::Utf8Value utfValue(isolate, specifier);
		std::string name(*utfValue);
		V8ResourceImpl* resource = V8ResourceImpl::Get(context);

		auto module = static_cast<CV8ResourceImpl*>(resource)->GetModuleFromName(name, isolate);
		if (module.IsEmpty())
			resolver->Reject(context, v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Module could not be found").ToLocalChecked()));
		auto resolved = CV8ScriptRuntime::ResolveModule(context, specifier, module);
		Log::Info << "Resolved" << Log::Endl;
		v8::Local<v8::Module> outModule;

		if (resolved.IsEmpty() || !resolved.ToLocal(&outModule)) {
			Log::Info << "FuckNo" << Log::Endl;
			resolver->Reject(context, v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, "Module could not be found").ToLocalChecked()));
			Log::Info << "Fuck" << Log::Endl;
		}
		else
		{
			Log::Info << "Status: " << std::to_string(outModule->GetStatus()) << Log::Endl;
			if (outModule->GetStatus() != v8::Module::Status::kInstantiated)
				outModule->InstantiateModule(context, CV8ScriptRuntime::ResolveModule);
			Log::Info << "Status2: " << std::to_string(outModule->GetStatus()) << Log::Endl;
			if (outModule->GetStatus() != v8::Module::Status::kEvaluated)
				outModule->Evaluate(context);
			Log::Info << "Status3: " << std::to_string(outModule->GetStatus()) << Log::Endl;
			//resolver->Resolve(ctx, outModule->GetModuleNamespace());
			Log::Info << "Ctx: " << std::to_string(context.IsEmpty()) << Log::Endl;
			Log::Info << "Isolate: " << std::to_string(isolate == nullptr) << Log::Endl;
			auto str = v8::String::NewFromUtf8(isolate, "Fuck").ToLocalChecked();
			Log::Info << "test" << Log::Endl;
			resolver->Resolve(context, str);
		}

		return v8::MaybeLocal<v8::Promise>(resolver->GetPromise());
	});

	/*{
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope handle_scope(isolate);

		inspectorClient.reset(new alt::CV8InspectorClient);
		inspectorChannel.reset(new alt::CV8InspectorChannel);

		inspector = v8_inspector::V8Inspector::create(isolate, inspectorClient.get());

		v8_inspector::StringView inspectorView{ (uint8_t*)inspectorViewStr, strlen(inspectorViewStr) };
		inspectorSession = inspector->connect(1, inspectorChannel.get(), inspectorView);
	}*/

	{
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolate_scope(isolate);
		v8::HandleScope handle_scope(isolate);

		V8Class::LoadAll(isolate);

		extern V8Module altModule, nativesModule;
		V8Module::Add({altModule, nativesModule});
	}
}
