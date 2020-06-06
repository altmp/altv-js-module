#pragma once

#include <vector>
#include <functional>

#include <v8.h>

#include "cpp-sdk/entities/IEntity.h"
#include "cpp-sdk/types/MValue.h"

class V8Helpers
{
public:
	struct HashFunc
	{
		size_t operator()(v8::Local<v8::Function> fn) const
		{
			return fn->GetIdentityHash();
		}
	};

	struct EqFunc
	{
		size_t operator()(v8::Local<v8::Function> lhs, v8::Local<v8::Function> rhs) const
		{
			return lhs->StrictEquals(rhs);
		}
	};

	class Binding
	{
	public:
		using Callback = std::function<void(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)>;

		Binding(Callback&& fn)
		{
			All().push_back(std::move(fn));
		}

		static std::vector<Callback>& All()
		{
			static std::vector<Callback> _All;
			return _All;
		}

		static void RegisterAll(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)
		{
			for (auto& binding : All())
				binding(ctx, exports);
		}
	};

	static void Throw(v8::Isolate* isolate, const std::string& msg)
	{
		isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, msg.data(), v8::NewStringType::kNormal, msg.size()).ToLocalChecked()));
	}

	static void RegisterFunc(v8::Local<v8::Object> exports, const std::string& _name, v8::FunctionCallback cb, void* data = nullptr)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		v8::Local<v8::Context> ctx = isolate->GetEnteredContext();

		v8::Local<v8::String> name = v8::String::NewFromUtf8(isolate, _name.data(), v8::NewStringType::kNormal, _name.size()).ToLocalChecked();

		v8::Local<v8::Function> fn = v8::Function::New(ctx, cb, v8::External::New(isolate, data)).ToLocalChecked();
		fn->SetName(name);

		exports->Set(ctx, name, fn);
	}

	static bool TryCatch(const std::function<bool()>& fn);

	static void FunctionCallback(const v8::FunctionCallbackInfo<v8::Value>& info);

	static alt::MValue V8ToMValue(v8::Local<v8::Value> val);

	static v8::Local<v8::Value> MValueToV8(alt::MValueConst val);

	static void MValueArgsToV8(alt::MValueArgs args, std::vector<v8::Local<v8::Value>>& v8Args)
	{
		for (uint64_t i = 0; i < args.GetSize(); ++i)
			v8Args.push_back(MValueToV8(args[i]));
	}

	static void SetAccessor(v8::Local<v8::Template> tpl, v8::Isolate* isolate, const char* name, v8::AccessorGetterCallback getter,
		v8::AccessorSetterCallback setter = nullptr)
	{
		tpl->SetNativeDataProperty(v8::String::NewFromUtf8(isolate, name,
			v8::NewStringType::kInternalized).ToLocalChecked(), getter, setter);
	}
};

class V8ResourceImpl;

namespace V8
{
	class SourceLocation
	{
	public:
		SourceLocation(std::string&& fileName, int line);
		SourceLocation() { };

		const std::string& GetFileName() const { return fileName; }
		int GetLineNumber() const { return line; }

		static SourceLocation GetCurrent(v8::Isolate* isolate);

	private:
		std::string fileName;
		int line = 0;
	};

	struct EventCallback
	{
		v8::UniquePersistent<v8::Function> fn;
		SourceLocation location;
		bool removed = false;

		EventCallback(v8::Isolate* isolate, v8::Local<v8::Function> _fn, SourceLocation&& location) :
			fn(isolate, _fn), location(std::move(location)) { }
	};

	class EventHandler
	{
	public:
		using CallbacksGetter = std::function<std::vector<EventCallback*>(V8ResourceImpl* resource, const alt::CEvent*)>;
		using ArgsGetter = std::function<void(V8ResourceImpl* resource, const alt::CEvent*, std::vector<v8::Local<v8::Value>>& args)>;

		EventHandler(alt::CEvent::Type type, CallbacksGetter&& _handlersGetter, ArgsGetter&& _argsGetter) :
			callbacksGetter(std::move(_handlersGetter)), argsGetter(std::move(_argsGetter))
		{
			Register(type, this);
		}

		std::vector<V8::EventCallback*> GetCallbacks(V8ResourceImpl* impl, const alt::CEvent* e);
		std::vector<v8::Local<v8::Value>> GetArgs(V8ResourceImpl* impl, const alt::CEvent* e);

		static EventHandler* Get(const alt::CEvent* e);

	private:
		CallbacksGetter callbacksGetter;
		ArgsGetter argsGetter;

		static std::unordered_map<alt::CEvent::Type, EventHandler*>& all()
		{
			static std::unordered_map<alt::CEvent::Type, EventHandler*> _all;
			return _all;
		}

		static void Register(alt::CEvent::Type type, EventHandler* handler);
	};

	class LocalEventHandler : public EventHandler
	{
	public:
		LocalEventHandler(alt::CEvent::Type type, const std::string& name, ArgsGetter&& argsGetter) :
			EventHandler(type, std::move(GetCallbacksGetter(name)), std::move(argsGetter)) { }

	private:
		static CallbacksGetter GetCallbacksGetter(const std::string& name);
	};

	void DefineOwnProperty(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> val,
		const char* name, v8::Local<v8::Value> value, v8::PropertyAttribute attributes = v8::PropertyAttribute::None);

	void DefineOwnProperty(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> val,
		v8::Local<v8::String> name, v8::Local<v8::Value> value, v8::PropertyAttribute attributes = v8::PropertyAttribute::None);

	void SetAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name,
		v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

	void SetMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name,
		v8::FunctionCallback callback);

	void SetStaticAccessor(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name,
		v8::AccessorGetterCallback getter, v8::AccessorSetterCallback setter = nullptr);

	void SetStaticMethod(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate> tpl, const char* name,
		v8::FunctionCallback callback);

	void SetFunction(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Object> target,
		const char* name, v8::FunctionCallback cb, void* userData = nullptr);

	v8::Local<v8::Value> Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, const char* name);

	void RegisterSharedMain(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports);

	v8::Local<v8::Value> New(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Function> constructor, std::vector<v8::Local<v8::Value>>& args);

	// TODO: create c++ classes for v8 classes and move there
	v8::Local<v8::String> Vector3_XKey(v8::Isolate* isolate);
	v8::Local<v8::String> Vector3_YKey(v8::Isolate* isolate);
	v8::Local<v8::String> Vector3_ZKey(v8::Isolate* isolate);

	v8::Local<v8::String> RGBA_RKey(v8::Isolate* isolate);
	v8::Local<v8::String> RGBA_GKey(v8::Isolate* isolate);
	v8::Local<v8::String> RGBA_BKey(v8::Isolate* isolate);
	v8::Local<v8::String> RGBA_AKey(v8::Isolate* isolate);

	bool SafeToBoolean(v8::Local<v8::Value> val, v8::Isolate* isolate, bool* out);
	bool SafeToNumber(v8::Local<v8::Value> val, v8::Local<v8::Context> ctx, double* out);
	bool SafeToString(v8::Local<v8::Value> val, v8::Isolate* isolate, v8::Local<v8::Context> ctx, alt::String* out);
}

#define V8_GET_ISOLATE(info) v8::Isolate* isolate = (info).GetIsolate()
#define V8_GET_CONTEXT(isolate) v8::Local<v8::Context> ctx = (isolate)->GetEnteredContext()
#define V8_GET_ISOLATE_CONTEXT() \
	V8_GET_ISOLATE(info); \
	V8_GET_CONTEXT(isolate)

#define V8_GET_RESOURCE() \
	V8ResourceImpl* resource = V8ResourceImpl::Get(isolate->GetEnteredContext()); \
	V8_CHECK(resource, "invalid resource");

#define V8_GET_ISOLATE_CONTEXT_RESOURCE() \
	V8_GET_ISOLATE_CONTEXT(); \
	V8_GET_RESOURCE()

#define V8_CHECK_RETN(a, b, c) if (!(a)) { V8Helpers::Throw(isolate, (b)); return c; }
#define V8_CHECK(a, b) V8_CHECK_RETN(a, b,)

#define V8_GET_THIS_ENTITY(val, type) \
	::alt::Ref<type> val; \
	{ \
		V8Entity* __val = V8Entity::Get(info.This()); \
		V8_CHECK(__val, "entity is invalid"); \
		val = __val->GetHandle().As<type>(); \
	}


#define V8_CHECK_ARGS_LEN(count) V8_CHECK((info).Length() == (count), #count " arguments expected")

// idx starts with 1
#define V8_TO_BOOLEAN(idx, val) \
	bool val; \
	V8_CHECK(V8::SafeToBoolean(info[(idx) - 1], isolate, &val), "Failed to convert argument " #idx " to bool")

#define V8_TO_NUMBER(idx, val) \
	double val; \
	V8_CHECK(V8::SafeToNumber(info[(idx) - 1], ctx, &val), "Failed to convert argument " #idx " to number")

#define V8_TO_STRING(idx, val) \
	alt::String val; \
	V8_CHECK(V8::SafeToString(info[(idx) - 1], isolate, ctx, &val), "Failed to convert argument " #idx " to string")

#define V8_RETURN(val) info.GetReturnValue().Set(val)
#define V8_RETURN_BOOL(val) V8_RETURN(v8::Boolean::New(isolate, (val)))

#define V8_BIND_BASE_OBJECT(baseObjectRef, error) \
	{ \
		auto baseObject = (baseObjectRef); \
		V8_CHECK(!baseObject.IsEmpty(), error); \
		resource->BindEntity(info.This(), baseObject.Get()); \
	}
