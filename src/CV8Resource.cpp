#include "cpp-sdk/objects/IEntity.h"
#include "cpp-sdk/objects/IPlayer.h"

#include "cpp-sdk/events/CEvent.h"
#include "cpp-sdk/events/CClientScriptEvent.h"
#include "cpp-sdk/events/CServerScriptEvent.h"
#include "cpp-sdk/events/CWebViewEvent.h"
#include "cpp-sdk/events/CKeyboardEvent.h"
#include "cpp-sdk/events/CConnectionComplete.h"
#include "cpp-sdk/events/CDisconnectEvent.h"
#include "cpp-sdk/events/CGameEntityCreateEvent.h"
#include "cpp-sdk/events/CGameEntityDestroyEvent.h"
#include "cpp-sdk/events/CSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CStreamSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CGlobalSyncedMetaDataChangeEvent.h"
#include "cpp-sdk/events/CRemoveEntityEvent.h"
#include "cpp-sdk/events/CResourceStartEvent.h"
#include "cpp-sdk/events/CResourceStopEvent.h"
#include "cpp-sdk/events/CResourceErrorEvent.h"
#include "cpp-sdk/events/CRenderEvent.h"
#include "cpp-sdk/SDK.h"

#include "CV8ScriptRuntime.h"
#include "CV8Resource.h"
#include "helpers/V8Helpers.h"
#include "helpers/V8Module.h"

static void StaticRequire(const v8::FunctionCallbackInfo<v8::Value> &info)
{
	v8::Isolate *isolate = info.GetIsolate();

	V8_CHECK(info.Length() == 1, "1 arg expected");
	V8_CHECK(info[0]->IsString(), "moduleName must be a string");

	V8ResourceImpl *resource = V8ResourceImpl::Get(isolate->GetEnteredContext());
	V8_CHECK(resource, "invalid resource");

	std::string name{*v8::String::Utf8Value{isolate, info[0]}};

	v8::MaybeLocal<v8::Value> _exports = static_cast<CV8ResourceImpl *>(resource)->Require(name);

	if (!_exports.IsEmpty())
		info.GetReturnValue().Set(_exports.ToLocalChecked());
	else
		V8Helpers::Throw(isolate, "No such module " + name);
}

void CV8ResourceImpl::ProcessDynamicImports() {
	for(auto import : dynamicImports)
	{
		import();
	}
	dynamicImports.clear();
}

extern V8Module altModule;
bool CV8ResourceImpl::Start()
{
	if (resource->GetMain().IsEmpty())
		return false;

	resource->EnableNatives();
	auto nscope = resource->PushNativesScope();

	v8::Locker locker(isolate);
	v8::Isolate::Scope isolate_scope(isolate);
	v8::HandleScope handle_scope(isolate);

	v8::Local<v8::Context> ctx = v8::Context::New(isolate);

	context.Reset(isolate, ctx);
	ctx->SetAlignedPointerInEmbedderData(1, resource);

	V8ResourceImpl::Start();

	v8::Context::Scope context_scope(ctx);

	//Log::Debug(V8ResourceImpl::GetResource(ctx));
	//Log::Debug(V8ResourceImpl::GetResource(isolate->GetEnteredContext()));

	/*runtime->GetInspector()->contextCreated({
		ctx,
		1,
		v8_inspector::StringView{ (uint8_t*)name.CStr(), name.GetSize() }
	});*/

	std::string path = resource->GetMain().ToString();

	alt::IPackage *pkg = resource->GetPackage();
	alt::IPackage::File *file = pkg->OpenFile(path);

	alt::String src{pkg->GetFileSize(file)};

	pkg->ReadFile(file, src.GetData(), src.GetSize());
	pkg->CloseFile(file);

	Log::Info << "[V8] Starting script " << path << Log::Endl;

	v8::Local<v8::String> sourceCode = v8::String::NewFromUtf8(isolate, src.GetData(), v8::NewStringType::kNormal, src.GetSize()).ToLocalChecked();

	v8::ScriptOrigin scriptOrigin{
		v8::String::NewFromUtf8(isolate, path.c_str()).ToLocalChecked(),
		v8::Local<v8::Integer>(),
		v8::Local<v8::Integer>(),
		v8::Local<v8::Boolean>(),
		v8::Local<v8::Integer>(),
		v8::Local<v8::Value>(),
		v8::Local<v8::Boolean>(),
		v8::Local<v8::Boolean>(),
		v8::True(isolate),
		v8::Local<v8::PrimitiveArray>()};

	bool result = V8Helpers::TryCatch([&]() {
		v8::ScriptCompiler::Source source{sourceCode, scriptOrigin};
		v8::MaybeLocal<v8::Module> maybeModule = v8::ScriptCompiler::CompileModule(isolate, &source);

		if (maybeModule.IsEmpty())
			return false;

		v8::Local<v8::Module> curModule = maybeModule.ToLocalChecked();

		modules.emplace(path, v8::UniquePersistent<v8::Module>{isolate, curModule});

		// Overwrite global console object
		auto console = ctx->Global()->Get(ctx, V8_NEW_STRING("console")).ToLocalChecked().As<v8::Object>();
		if (!console.IsEmpty())
		{
			auto exports = altModule.GetExports(isolate, ctx);

			console->Set(ctx, V8_NEW_STRING("log"), exports->Get(ctx, V8_NEW_STRING("log")).ToLocalChecked());
			console->Set(ctx, V8_NEW_STRING("warn"), exports->Get(ctx, V8_NEW_STRING("logWarning")).ToLocalChecked());
			console->Set(ctx, V8_NEW_STRING("error"), exports->Get(ctx, V8_NEW_STRING("logError")).ToLocalChecked());
		}

		ctx->Global()->Set(ctx, v8::String::NewFromUtf8(isolate, "__internal_get_exports").ToLocalChecked(), v8::Function::New(ctx, &StaticRequire).ToLocalChecked());
		bool res = curModule->InstantiateModule(ctx, CV8ScriptRuntime::ResolveModule).IsJust();

		if (!res)
			return false;

		v8::MaybeLocal<v8::Value> v = curModule->Evaluate(ctx);

		if (v.IsEmpty())
			return false;

		alt::MValue _exports = V8Helpers::V8ToMValue(curModule->GetModuleNamespace());
		resource->SetExports(_exports.As<alt::IMValueDict>());

		Log::Info << "[V8] Started script " << path << Log::Endl;
		return true;
	});

	if (!result)
	{
		modules.erase(path);
	}

	DispatchStartEvent(!result);

	// if all resources are already loaded
	if(CV8ScriptRuntime::instance->resourcesLoaded) {
		ProcessDynamicImports();
	}

	return result;
}

bool CV8ResourceImpl::Stop()
{
	std::vector<alt::Ref<alt::IBaseObject>> objects(ownedObjects.size());

	for (auto handle : ownedObjects)
		objects.push_back(handle);

	ownedObjects.clear();

	// ?????
	// for (auto handle : objects)
	// {
	// 	CGame::Instance().DestroyBaseObject(handle);
	// }

	//runtime->GetInspector()->contextDestroyed(context.Get(isolate));

	if (!context.IsEmpty())
	{
		auto nscope = resource->PushNativesScope();

		v8::Locker locker(isolate);
		v8::Isolate::Scope isolateScope(isolate);
		v8::HandleScope handleScope(isolate);

		v8::Context::Scope scope(GetContext());

		DispatchStopEvent();
	}

	return true;
}

bool CV8ResourceImpl::OnEvent(const alt::CEvent *e)
{
	auto nscope = resource->PushNativesScope();

	v8::Locker locker(isolate);
	v8::Isolate::Scope isolateScope(isolate);
	v8::HandleScope handleScope(isolate);
	auto ctx = GetContext();
	if (ctx.IsEmpty())
		return true;
	v8::Context::Scope scope(ctx);

	std::vector<V8::EventCallback *> handlers;

	switch (e->GetType())
	{
	case alt::CEvent::Type::RENDER:
	{
		handlers = GetLocalHandlers("render");
		break;
	}
	case alt::CEvent::Type::WEB_VIEW_EVENT:
	{
		auto ev = static_cast<const alt::CWebViewEvent *>(e);
		auto it = webViewHandlers.find(ev->GetTarget().Get());

		if (it != webViewHandlers.end())
		{
			auto range = it->second.equal_range(ev->GetName().ToString());

			for (auto it = range.first; it != range.second; ++it)
				handlers.push_back(&it->second);
		}

		break;
	}
	case alt::CEvent::Type::CLIENT_SCRIPT_EVENT:
	{
		auto ev = static_cast<const alt::CClientScriptEvent *>(e);
		handlers = GetLocalHandlers(ev->GetName().ToString());
		break;
	}
	case alt::CEvent::Type::SERVER_SCRIPT_EVENT:
	{
		auto ev = static_cast<const alt::CServerScriptEvent *>(e);
		handlers = GetRemoteHandlers(ev->GetName().ToString());
		break;
	}
	case alt::CEvent::Type::KEYBOARD_EVENT:
	{
		auto ev = static_cast<const alt::CKeyboardEvent *>(e);

		if (ev->GetKeyState() == alt::CKeyboardEvent::KeyState::UP)
			handlers = GetLocalHandlers("keyup");
		else if (ev->GetKeyState() == alt::CKeyboardEvent::KeyState::DOWN)
			handlers = GetLocalHandlers("keydown");

		break;
	}
	case alt::CEvent::Type::CONNECTION_COMPLETE:
	{
		handlers = GetLocalHandlers("connectionComplete");
		CV8ScriptRuntime* runtime = CV8ScriptRuntime::instance;
		runtime->resourcesLoaded = true;

		ProcessDynamicImports();

		break;
	}
	case alt::CEvent::Type::DISCONNECT_EVENT:
	{
		handlers = GetLocalHandlers("disconnect");
		break;
	}
	case alt::CEvent::Type::REMOVE_ENTITY_EVENT:
	{
		handlers = GetLocalHandlers("removeEntity");
		break;
	}
	case alt::CEvent::Type::CONSOLE_COMMAND_EVENT:
	{
		handlers = GetLocalHandlers("consoleCommand");
		break;
	}
	case alt::CEvent::Type::GAME_ENTITY_CREATE:
	{
		handlers = GetLocalHandlers("gameEntityCreate");
		break;
	}
	case alt::CEvent::Type::GAME_ENTITY_DESTROY:
	{
		handlers = GetLocalHandlers("gameEntityDestroy");
		break;
	}
	case alt::CEvent::Type::SYNCED_META_CHANGE:
	{
		handlers = GetLocalHandlers("syncedMetaChange");
		break;
	}
	case alt::CEvent::Type::STREAM_SYNCED_META_CHANGE:
	{
		handlers = GetLocalHandlers("streamSyncedMetaChange");
		break;
	}
	case alt::CEvent::Type::GLOBAL_META_CHANGE:
	{
		handlers = GetLocalHandlers("globalMetaChange");
		break;
	}
	case alt::CEvent::Type::GLOBAL_SYNCED_META_CHANGE:
	{
		handlers = GetLocalHandlers("globalSyncedMetaChange");
		break;
	}
	case alt::CEvent::Type::RESOURCE_START:
	{
		handlers = GetLocalHandlers("anyResourceStart");
		break;
	}
	case alt::CEvent::Type::RESOURCE_STOP:
	{
		handlers = GetLocalHandlers("anyResourceStop");
		break;
	}
	case alt::CEvent::Type::RESOURCE_ERROR:
	{
		handlers = GetLocalHandlers("anyResourceError");
		break;
	}
	case alt::CEvent::Type::PLAYER_ENTER_VEHICLE:
	{
		handlers = GetLocalHandlers("enteredVehicle");
		break;
	}
	case alt::CEvent::Type::PLAYER_LEAVE_VEHICLE:
	{
		handlers = GetLocalHandlers("leftVehicle");
		break;
	}
	case alt::CEvent::Type::PLAYER_CHANGE_VEHICLE_SEAT:
	{
		handlers = GetLocalHandlers("changedVehicleSeat");
		break;
	}
	}

	if (handlers.size() > 0)
	{
		std::vector<v8::Local<v8::Value>> args;

		switch (e->GetType())
		{
		case alt::CEvent::Type::WEB_VIEW_EVENT:
		{
			auto ev = static_cast<const alt::CWebViewEvent *>(e);

			V8Helpers::MValueArgsToV8(ev->GetArgs(), args);

			break;
		}
		case alt::CEvent::Type::CLIENT_SCRIPT_EVENT:
		{
			auto ev = static_cast<const alt::CClientScriptEvent *>(e);

			V8Helpers::MValueArgsToV8(ev->GetArgs(), args);

			break;
		}
		case alt::CEvent::Type::SERVER_SCRIPT_EVENT:
		{
			auto ev = static_cast<const alt::CServerScriptEvent *>(e);

			V8Helpers::MValueArgsToV8(ev->GetArgs(), args);

			break;
		}
		case alt::CEvent::Type::KEYBOARD_EVENT:
		{
			auto ev = static_cast<const alt::CKeyboardEvent *>(e);

			args.push_back(v8::Integer::NewFromUnsigned(isolate, ev->GetKeyCode()));

			break;
		}
		case alt::CEvent::Type::CONNECTION_COMPLETE:
		{
			break;
		}
		case alt::CEvent::Type::DISCONNECT_EVENT:
		{
			CV8ScriptRuntime::instance->resourcesLoaded = false;
			break;
		}
		case alt::CEvent::Type::REMOVE_ENTITY_EVENT:
		{
			auto ev = static_cast<const alt::CRemoveEntityEvent *>(e);

			args.push_back(GetOrCreateEntity(ev->GetEntity().Get())->GetJSVal(isolate));

			break;
		}
		case alt::CEvent::Type::CONSOLE_COMMAND_EVENT:
		{
			auto ev = static_cast<const alt::CConsoleCommandEvent *>(e);

			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetName().CStr()).ToLocalChecked());
			for (auto &arg : ev->GetArgs())
				args.push_back(v8::String::NewFromUtf8(isolate, arg.CStr()).ToLocalChecked());

			break;
		}
		case alt::CEvent::Type::GAME_ENTITY_CREATE:
		{
			auto ev = static_cast<const alt::CGameEntityCreateEvent *>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));

			break;
		}
		case alt::CEvent::Type::GAME_ENTITY_DESTROY:
		{
			auto ev = static_cast<const alt::CGameEntityDestroyEvent *>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));

			break;
		}
		case alt::CEvent::Type::SYNCED_META_CHANGE:
		{
			auto ev = static_cast<const alt::CSyncedMetaDataChangeEvent *>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetKey().CStr()).ToLocalChecked());
			args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
			args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));

			break;
		}
		case alt::CEvent::Type::STREAM_SYNCED_META_CHANGE:
		{
			auto ev = static_cast<const alt::CStreamSyncedMetaDataChangeEvent *>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetKey().CStr()).ToLocalChecked());
			args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
			args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));

			break;
		}
		case alt::CEvent::Type::GLOBAL_META_CHANGE:
		{
			auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent *>(e);

			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetKey().CStr()).ToLocalChecked());
			args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
			args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));

			break;
		}
		case alt::CEvent::Type::GLOBAL_SYNCED_META_CHANGE:
		{
			auto ev = static_cast<const alt::CGlobalSyncedMetaDataChangeEvent *>(e);

			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetKey().CStr()).ToLocalChecked());
			args.push_back(V8Helpers::MValueToV8(ev->GetVal()));
			args.push_back(V8Helpers::MValueToV8(ev->GetOldVal()));

			break;
		}
		case alt::CEvent::Type::RESOURCE_START:
		{
			auto ev = static_cast<const alt::CResourceStartEvent *>(e);
			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetResource()->GetName().CStr()).ToLocalChecked());
			break;
		}
		case alt::CEvent::Type::RESOURCE_STOP:
		{
			auto ev = static_cast<const alt::CResourceStopEvent *>(e);
			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetResource()->GetName().CStr()).ToLocalChecked());
			break;
		}
		case alt::CEvent::Type::RESOURCE_ERROR:
		{
			auto ev = static_cast<const alt::CResourceErrorEvent *>(e);
			args.push_back(v8::String::NewFromUtf8(isolate, ev->GetResource()->GetName().CStr()).ToLocalChecked());
			break;
		}
		case alt::CEvent::Type::PLAYER_ENTER_VEHICLE:
		{
			auto ev = static_cast<const alt::CPlayerEnterVehicleEvent*>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
			args.push_back(v8::Integer::New(isolate, ev->GetSeat()));
			break;
		}
		case alt::CEvent::Type::PLAYER_LEAVE_VEHICLE:
		{
			auto ev = static_cast<const alt::CPlayerLeaveVehicleEvent*>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
			args.push_back(v8::Integer::New(isolate, ev->GetSeat()));
			break;
		}
		case alt::CEvent::Type::PLAYER_CHANGE_VEHICLE_SEAT:
		{
			auto ev = static_cast<const alt::CPlayerChangeVehicleSeatEvent*>(e);

			args.push_back(GetOrCreateEntity(ev->GetTarget().Get())->GetJSVal(isolate));
			args.push_back(v8::Integer::New(isolate, ev->GetOldSeat()));
			args.push_back(v8::Integer::New(isolate, ev->GetNewSeat()));
			break;
		}

		}

		InvokeEventHandlers(e, handlers, args);
	}

	return true;
}

void CV8ResourceImpl::OnTick()
{
	v8::Locker locker(isolate);
	v8::Isolate::Scope isolateScope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Context::Scope scope(GetContext());

	int64_t time = GetTime();

	V8ResourceImpl::OnTick();

	if (GetTime() - time > 10)
	{
		Log::Warning << "Resource " << resource->GetName() << " tick was too long " << GetTime() - time << " ms" << Log::Endl;
	}

	for (auto &view : webViewHandlers)
	{
		for (auto it = view.second.begin(); it != view.second.end();)
		{
			if (it->second.removed)
				it = view.second.erase(it);
			else
				++it;
		}
	}
}

void CV8ResourceImpl::OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage &data)
{
	promiseRejections.RejectedWithNoHandler(this, data);
}

void CV8ResourceImpl::OnPromiseHandlerAdded(v8::PromiseRejectMessage &data)
{
	promiseRejections.HandlerAdded(this, data);
}

static v8::MaybeLocal<v8::Module> CompileESM(v8::Isolate *isolate, const std::string &name, const std::string &src)
{
	v8::Local<v8::String> sourceCode = v8::String::NewFromUtf8(isolate, src.data(), v8::NewStringType::kNormal, src.size()).ToLocalChecked();

	v8::ScriptOrigin scriptOrigin{
		v8::String::NewFromUtf8(isolate, name.c_str()).ToLocalChecked(),
		v8::Local<v8::Integer>(),
		v8::Local<v8::Integer>(),
		v8::Local<v8::Boolean>(),
		v8::Local<v8::Integer>(),
		v8::Local<v8::Value>(),
		v8::Local<v8::Boolean>(),
		v8::Local<v8::Boolean>(),
		v8::True(isolate),
		v8::Local<v8::PrimitiveArray>()};

	v8::ScriptCompiler::Source source{sourceCode, scriptOrigin};
	return v8::ScriptCompiler::CompileModule(isolate, &source);
}

static v8::MaybeLocal<v8::Module> WrapModule(v8::Isolate *isolate, const std::deque<std::string> &_exportKeys, const std::string &name, bool exportAsDefault = false)
{
	bool hasDefault = false;
	std::stringstream src;

	src << "const _exports = __internal_get_exports('" << name << "');\n";

	for (auto &key : _exportKeys)
	{
		if (key == "default")
		{
			src << "export default _exports['" << key << "'];\n";
			hasDefault = true;
		}
		else
		{
			src << "export let " << key << " = _exports['" << key << "'];\n";
		}
	}

	if (!hasDefault && exportAsDefault)
		src << "export default _exports;";

	return CompileESM(isolate, name, src.str());
}

bool CV8ResourceImpl::IsValidModule(const std::string &name)
{
	if (V8Module::Exists(name))
		return true;

	alt::IResource *resource = alt::ICore::Instance().GetResource(name);
	if (resource)
		return true;

	return false;
}

std::deque<std::string> CV8ResourceImpl::GetModuleKeys(const std::string &name)
{
	auto &v8module = V8Module::All().find(name);
	if (v8module != V8Module::All().end())
	{
		auto _exports = v8module->second->GetExports(isolate, GetContext());
		v8::Local<v8::Array> v8Keys = _exports->GetOwnPropertyNames(GetContext()).ToLocalChecked();

		std::deque<std::string> keys;

		for (uint32_t i = 0; i < v8Keys->Length(); ++i)
		{
			v8::Local<v8::String> v8Key = v8Keys->Get(GetContext(), i).ToLocalChecked().As<v8::String>();
			keys.push_back(*v8::String::Utf8Value(isolate, v8Key));
		}

		return keys;
	}

	alt::IResource *resource = alt::ICore::Instance().GetResource(name);
	if (resource)
	{
		std::deque<std::string> keys;

		alt::MValueDict _exports = resource->GetExports();
		for (auto it = _exports->Begin(); it; it = _exports->Next())
			keys.push_back(it->GetKey().ToString());

		return keys;
	}

	return {};
}

std::string CV8ResourceImpl::GetModulePath(v8::Local<v8::Module> moduleHandle)
{
	for (auto &md : modules)
	{
		if (md.second == moduleHandle)
			return md.first;
	}

	return std::string{};
}

v8::Local<v8::Module> CV8ResourceImpl::GetModuleFromPath(std::string modulePath)
{
	for (auto& md : modules)
	{
		if (md.first == modulePath)
			return md.second.Get(isolate);
	}

	return v8::Local<v8::Module>{};
}

static bool IsSystemModule(const std::string &name)
{
	return V8Module::Exists(name);
}

v8::MaybeLocal<v8::Value> CV8ResourceImpl::Require(const std::string &name)
{
	auto it = requires.find(name);

	if (it != requires.end())
		return it->second.Get(isolate);

	auto &v8module = V8Module::All().find(name);
	if (v8module != V8Module::All().end())
	{
		auto _exports = v8module->second->GetExports(isolate, GetContext());
		requires.insert({name, v8::UniquePersistent<v8::Value>{isolate, _exports}});

		return _exports;
	}

	alt::IResource *resource = alt::ICore::Instance().GetResource(name);
	if (resource)
	{
		v8::Local<v8::Value> _exports = V8Helpers::MValueToV8(resource->GetExports());

		requires.insert({name, v8::UniquePersistent<v8::Value>{isolate, _exports}});

		return _exports;
	}

	return v8::MaybeLocal<v8::Value>();
}

v8::MaybeLocal<v8::Module> CV8ResourceImpl::ResolveFile(const std::string &name, v8::Local<v8::Module> referrer)
{
	auto path = alt::ICore::Instance().Resolve(resource, name, GetModulePath(referrer));

	if (!path.pkg)
		return v8::MaybeLocal<v8::Module>();

	auto fileName = path.fileName.ToString();

	if (fileName.size() == 0)
	{
		if (path.pkg->FileExists("index.js"))
			fileName = "index.js";
		else if (path.pkg->FileExists("index.mjs"))
			fileName = "index.mjs";
		else
			return v8::MaybeLocal<v8::Module>();
	}
	else
	{
		if (path.pkg->FileExists(fileName + ".js"))
			fileName += ".js";
		else if (path.pkg->FileExists(fileName + ".mjs"))
			fileName += ".mjs";
		else if (path.pkg->FileExists(fileName + "/index.js"))
			fileName += "/index.js";
		else if (path.pkg->FileExists(fileName + "/index.mjs"))
			fileName += "/index.mjs";
		else if (!path.pkg->FileExists(fileName))
			return v8::MaybeLocal<v8::Module>();
	}

	std::string fullName = path.prefix.ToString() + fileName;

	auto it = modules.find(fullName);

	if (it != modules.end())
		return it->second.Get(isolate);

	v8::MaybeLocal<v8::Module> maybeModule;

	V8Helpers::TryCatch([&] {
		alt::IPackage::File *file = path.pkg->OpenFile(fileName);

		std::string src(path.pkg->GetFileSize(file), '\0');
		path.pkg->ReadFile(file, src.data(), src.size());
		path.pkg->CloseFile(file);

		maybeModule = CompileESM(isolate, fullName, src);

		if (maybeModule.IsEmpty())
			return false;

		v8::Local<v8::Module> _module = maybeModule.ToLocalChecked();

		modules.emplace(fullName, v8::UniquePersistent<v8::Module>{isolate, _module});
		/*v8::Maybe<bool> res = _module->InstantiateModule(GetContext(), CV8ScriptRuntime::ResolveModule);
		if (res.IsNothing())
			return false;

		v8::MaybeLocal<v8::Value> res2 = _module->Evaluate(GetContext());
		if (res2.IsEmpty())
			return false;*/

		return true;
	});

	if (maybeModule.IsEmpty())
	{
		modules.erase(fullName);
	}

	return maybeModule;
}

v8::MaybeLocal<v8::Module> CV8ResourceImpl::ResolveModule(const std::string &_name, v8::Local<v8::Module> referrer)
{
	v8::MaybeLocal<v8::Module> maybeModule;

	std::string name = _name;

	if (name == "alt-client")
		name = "alt";

	auto it = modules.find(name);
	if (it != modules.end())
	{
		maybeModule = it->second.Get(isolate);
	}

	if (maybeModule.IsEmpty())
	{
		if (IsValidModule(name))
		{
			V8Helpers::TryCatch([&] {
				maybeModule = WrapModule(isolate, GetModuleKeys(name), name, IsSystemModule(name));

				if (!maybeModule.IsEmpty())
				{
					v8::Local<v8::Module> _module = maybeModule.ToLocalChecked();

					modules.emplace(name, v8::UniquePersistent<v8::Module>{isolate, _module});

					/*v8::Maybe<bool> res = _module->InstantiateModule(GetContext(), CV8ScriptRuntime::ResolveModule);
					if (res.IsNothing())
					{
						Log::Info(__LINE__, "res.IsNothing()");
						return false;
					}

					v8::MaybeLocal<v8::Value> res2 = _module->Evaluate(GetContext());
					if (res2.IsEmpty())
					{
						Log::Info(__LINE__, "res2.IsEmpty()");
						return false;
					}*/

					return true;
				}

				Log::Info << __LINE__ << "maybeModule.IsEmpty()";
				return false;
			});

			if (maybeModule.IsEmpty())
			{
				modules.erase(name);
				isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, ("Failed to load module: " + name).c_str()).ToLocalChecked()));
				return v8::MaybeLocal<v8::Module>{};
			}
		}
	}

	if (maybeModule.IsEmpty())
	{
		maybeModule = ResolveFile(name, referrer);
	}

	if (maybeModule.IsEmpty())
	{
		modules.erase(name);
		isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, ("No such module: " + name).c_str()).ToLocalChecked()));
		return v8::MaybeLocal<v8::Module>{};
	}

	/*v8::Local<v8::Module> _module = maybeModule.ToLocalChecked();
	if (_module->GetStatus() != v8::Module::kEvaluated)
	{
		modules.erase(name);
		isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, ("Failed to import: " + name).c_str())));
		return v8::MaybeLocal<v8::Module>{ };
	}*/

	return maybeModule;
}
