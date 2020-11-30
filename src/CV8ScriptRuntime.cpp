
#include "CV8ScriptRuntime.h"
#include "inspector/CV8InspectorClient.h"
#include "inspector/CV8InspectorChannel.h"
#include "helpers/V8Module.h"

CV8ScriptRuntime* CV8ScriptRuntime::instance = nullptr;

CV8ScriptRuntime::CV8ScriptRuntime()
{
	instance = this;

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

	/*isolate->SetHostImportModuleDynamicallyCallback([](v8::Local<v8::Context> context, v8::Local<v8::ScriptOrModule> referrer, v8::Local<v8::String> specifier) {

		return v8::MaybeLocal<v8::Promise>();
	});*/

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
