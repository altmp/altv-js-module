
#include "CV8ScriptRuntime.h"
#include "inspector/CV8InspectorClient.h"
#include "inspector/CV8InspectorChannel.h"
#include "helpers/V8Module.h"
#include "events/Events.h"

CV8ScriptRuntime::CV8ScriptRuntime()
{
	platform = v8::platform::NewDefaultPlatform();
	v8::V8::InitializePlatform(platform.get());
	v8::V8::Initialize();

	create_params.array_buffer_allocator = v8::ArrayBuffer::Allocator::NewDefaultAllocator();

	isolate = v8::Isolate::New(create_params);
	isolate->SetFatalErrorHandler([](const char *location, const char *message) {
		Log::Error << "[V8] " << location << ": " << message << Log::Endl;
	});

	isolate->SetOOMErrorHandler([](const char* location, bool isHeap) {
		if(!isHeap) return;
		Log::Error << "[V8] " << location << ": Heap out of memory. Forward this to the server developers." << Log::Endl;
		Log::Error << "[V8] The current heap limit can be shown with the 'heap' console command. Consider increasing your system RAM." << Log::Endl;
	});

	isolate->AddNearHeapLimitCallback([](void*, size_t current, size_t initial) {
		Log::Warning << "[V8] The remaining V8 heap space is approaching critical levels. Forward this to the server developers." << Log::Endl;
		Log::Warning << "[V8] Initial heap limit: " << CV8ScriptRuntime::formatBytes(initial) << " | Current heap limit: " << CV8ScriptRuntime::formatBytes(current) << Log::Endl;

		// Increase the heap limit by 100MB if the heap limit has not exceeded 4GB
		uint64_t currentLimitMb = (current / 1024) / 1024;
		if(currentLimitMb < 4096) return current + (100 * 1024 * 1024);
		else return current;
	}, nullptr);

	isolate->SetPromiseRejectCallback([](v8::PromiseRejectMessage message) {
		v8::Isolate *isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Value> value = message.GetValue();
		v8::Local<v8::Context> ctx = isolate->GetEnteredOrMicrotaskContext();

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
			Log::Error << "You're not supposed to ever see this" << Log::Endl;
		}
	});

	isolate->SetHostImportModuleDynamicallyCallback([](v8::Local<v8::Context> context, v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier, v8::Local<v8::FixedArray>) {
		v8::Isolate* isolate = context->GetIsolate();

		auto referrerVal = referrer->GetResourceName();
		if(referrerVal->IsUndefined())
		{
			return v8::MaybeLocal<v8::Promise>();
		}

		std::string referrerUrl = *v8::String::Utf8Value(isolate, referrer->GetResourceName());
		auto resource = static_cast<CV8ResourceImpl*>(V8ResourceImpl::Get(context));

		auto resolver = v8::Promise::Resolver::New(context).ToLocalChecked();

		V8::CPersistent<v8::Promise::Resolver> presolver(isolate, resolver);
		V8::CPersistent<v8::String> pspecifier(isolate, specifier);
		V8::CPersistent<v8::Module> preferrerModule(isolate, resource->GetModuleFromPath(referrerUrl));

		// careful what we take in by value in the lambda
		// it is possible pass v8::Local but should not be done
		// make a V8::CPersistent out of it and pass that
		auto domodule = [isolate, presolver, pspecifier, preferrerModule]{
			auto referrerModule = preferrerModule.Get(isolate);
			auto resolver = presolver.Get(isolate);
			auto specifier = pspecifier.Get(isolate);

			auto ctx = resolver->GetCreationContext().ToLocalChecked();
			v8::Context::Scope ctxs(ctx);

			auto mmodule = ResolveModule(ctx, specifier, v8::Local<v8::FixedArray>(), referrerModule);
			if(mmodule.IsEmpty())
			{
				resolver->Reject(ctx, v8::Exception::ReferenceError(V8_NEW_STRING("Could not resolve module")));
				return;
			}

			auto module = mmodule.ToLocalChecked();
			V8Helpers::TryCatch([&]{
				if(module->GetStatus() == v8::Module::Status::kUninstantiated && !module->InstantiateModule(ctx, ResolveModule).ToChecked())
				{
					resolver->Reject(ctx, v8::Exception::ReferenceError(V8_NEW_STRING("Error instantiating module")));
					return false;
				}

				if(module->GetStatus() != v8::Module::Status::kEvaluated && module->Evaluate(ctx).IsEmpty())
				{
					resolver->Reject(ctx, v8::Exception::ReferenceError(V8_NEW_STRING("Error evaluating module")));
					return false;
				}

				resolver->Resolve(ctx, module->GetModuleNamespace());
				return true;
			});
		};

		if(Instance().resourcesLoaded && resource->GetResource()->IsStarted())
		{
			// instantly resolve the module
			domodule();
		} else {
			// put it in the queue to resolve after all resource are loaded
			resource->dynamicImports.emplace_back(domodule);
		}

		return v8::MaybeLocal<v8::Promise>(resolver->GetPromise());
	});

	isolate->SetMicrotasksPolicy(v8::MicrotasksPolicy::kExplicit);

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

	RegisterEvents();
}

void CV8ScriptRuntime::OnEntityStreamIn(alt::Ref<alt::IEntity> entity) 
{
	switch(entity->GetType())
	{
		case alt::IEntity::Type::PLAYER:
		{
			streamedInPlayers.insert({ entity->GetID(), entity.As<alt::IPlayer>() });
			break;
		}
		case alt::IEntity::Type::VEHICLE:
		{
			streamedInVehicles.insert({ entity->GetID(), entity.As<alt::IVehicle>() });
			break;
		}
	}
}

void CV8ScriptRuntime::OnEntityStreamOut(alt::Ref<alt::IEntity> entity) 
{
	switch(entity->GetType())
	{
		case alt::IEntity::Type::PLAYER:
		{
			streamedInPlayers.erase(entity->GetID());
			break;
		}
		case alt::IEntity::Type::VEHICLE:
		{
			streamedInVehicles.erase(entity->GetID());
			break;
		}
	}
}
