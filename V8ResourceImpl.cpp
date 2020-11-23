
#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

#include "V8ResourceImpl.h"

#ifdef ALT_SERVER_API
#include "CNodeResourceImpl.h"
#include "node.h"
#endif

using namespace alt;

V8ResourceImpl::~V8ResourceImpl() {
	for(auto& [obj, ent] : entities) {
		delete ent;
	}

	entities.clear();
}

extern V8Class v8Vector3, v8RGBA, v8BaseObject;
bool V8ResourceImpl::Start()
{
	vector3Class.Reset(isolate, v8Vector3.JSValue(isolate, GetContext()));
	rgbaClass.Reset(isolate, v8RGBA.JSValue(isolate, GetContext()));
	baseObjectClass.Reset(isolate, v8BaseObject.JSValue(isolate, GetContext()));

	return true;
}

void V8ResourceImpl::OnTick()
{
	for (auto &id : oldTimers)
		timers.erase(id);

	oldTimers.clear();

	for (auto &p : timers)
	{
		int64_t time = GetTime();

		if (!p.second->Update(time))
			RemoveTimer(p.first);

		if (GetTime() - time > 10)
		{
			auto &location = p.second->GetLocation();

			if (location.GetLineNumber() != 0)
			{
				Log::Warning << "Timer at "
							 << resource->GetName() << ":" << location.GetFileName() << ":" << location.GetLineNumber()
							 << " was too long " << GetTime() - time << "ms" << Log::Endl;
			}
			else
			{
				Log::Warning << "Timer at "
							 << resource->GetName() << ":" << location.GetFileName()
							 << " was too long " << GetTime() - time << "ms" << Log::Endl;
			}
		}
	}

	for (auto it = localHandlers.begin(); it != localHandlers.end();)
	{
		if (it->second.removed)
			it = localHandlers.erase(it);
		else
			++it;
	}

	for (auto it = remoteHandlers.begin(); it != remoteHandlers.end();)
	{
		if (it->second.removed)
			it = remoteHandlers.erase(it);
		else
			++it;
	}

	promiseRejections.ProcessQueue(this);
}

bool V8ResourceImpl::DeleteEntity(alt::Ref<alt::IBaseObject> handle)
{
	auto it = entities.find(handle.Get());
	if(it == entities.end())
	{
		Log::Error << "[JS] Attempted to delete invalid object" << Log::Endl;
		return false;
	}
	
	delete it->second;
	entities.erase(it);

	// BAD, SHOULD ONLY BE DONE FOR ENTITIES
	// THAT WAS CREATED FROM JS
	resource->RemoveReference(handle);

	return true;
}

void V8ResourceImpl::BindEntity(v8::Local<v8::Object> val, alt::IBaseObject *handle)
{
	V8Entity *ent = new V8Entity(GetContext(), V8Entity::GetClass(handle), val, handle);
	entities.insert({handle, ent});
}

v8::Local<v8::Value> V8ResourceImpl::GetBaseObjectOrNull(alt::IBaseObject *handle)
{
	if (handle == nullptr)
	{
		return v8::Null(isolate);
	}
	else
	{
		return GetOrCreateEntity(handle)->GetJSVal(isolate);
	}
}

v8::Local<v8::Value> V8ResourceImpl::CreateVector3(alt::Vector3f vec)
{
	std::vector<v8::Local<v8::Value>> args{
		v8::Number::New(isolate, vec[0]),
		v8::Number::New(isolate, vec[1]),
		v8::Number::New(isolate, vec[2])};

	return v8Vector3.CreateInstance(isolate, GetContext(), args);
}

v8::Local<v8::Value> V8ResourceImpl::CreateRGBA(alt::RGBA rgba)
{
	std::vector<v8::Local<v8::Value>> args{
		v8::Number::New(isolate, rgba.r),
		v8::Number::New(isolate, rgba.g),
		v8::Number::New(isolate, rgba.b),
		v8::Number::New(isolate, rgba.a)};

	return V8::New(isolate, GetContext(), rgbaClass.Get(isolate), args);
}

bool V8ResourceImpl::IsVector3(v8::Local<v8::Value> val)
{
	return val->InstanceOf(GetContext(), vector3Class.Get(isolate)).ToChecked();
}

bool V8ResourceImpl::IsRGBA(v8::Local<v8::Value> val)
{
	return val->InstanceOf(GetContext(), rgbaClass.Get(isolate)).ToChecked();
}

bool V8ResourceImpl::IsBaseObject(v8::Local<v8::Value> val)
{
	return val->InstanceOf(GetContext(), baseObjectClass.Get(isolate)).ToChecked();
}

void V8ResourceImpl::OnCreateBaseObject(alt::Ref<alt::IBaseObject> handle)
{
	Log::Debug << "OnCreateBaseObject " << handle.Get() << " " << (entities.find(handle.Get()) != entities.end()) << Log::Endl;

	/*if (entities.find(handle.Get()) == entities.end())
	{
		v8::Locker locker(isolate);
		v8::Isolate::Scope isolateScope(isolate);
		v8::HandleScope handleScope(isolate);

		v8::Context::Scope scope(GetContext());
		CreateEntity(handle.Get());
	}*/

	NotifyPoolUpdate(handle.Get());
}

void V8ResourceImpl::OnRemoveBaseObject(alt::Ref<alt::IBaseObject> handle)
{
	NotifyPoolUpdate(handle.Get());

	v8::Locker locker(isolate);
	v8::Isolate::Scope isolateScope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Context::Scope scope(GetContext());

	V8Entity *ent = GetEntity(handle.Get());

	if (!ent)
		return;

	entities.erase(handle.Get());

	// TODO: ent->SetWeak();
	ent->GetJSVal(isolate)->SetInternalField(0, v8::External::New(isolate, nullptr));
	delete ent;
}

void V8ResourceImpl::NotifyPoolUpdate(alt::IBaseObject *ent)
{
	switch (ent->GetType())
	{
	case alt::IBaseObject::Type::PLAYER:
		playerPoolDirty = true;
		break;
	case alt::IBaseObject::Type::VEHICLE:
		vehiclePoolDirty = true;
		break;
	}
}

v8::Local<v8::Array> V8ResourceImpl::GetAllPlayers()
{
	if (playerPoolDirty)
	{
		playerPoolDirty = false;

		Array<Ref<IPlayer>> all = ICore::Instance().GetPlayers();
		v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

		for (uint32_t i = 0; i < all.GetSize(); ++i)
			jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

		players.Reset(isolate, jsAll);
		return jsAll;
	}

	return players.Get(isolate);
}

v8::Local<v8::Array> V8ResourceImpl::GetAllVehicles()
{
	if (vehiclePoolDirty)
	{
		vehiclePoolDirty = false;

		Array<Ref<IVehicle>> all = ICore::Instance().GetVehicles();
		v8::Local<v8::Array> jsAll = v8::Array::New(isolate, all.GetSize());

		for (uint32_t i = 0; i < all.GetSize(); ++i)
			jsAll->Set(GetContext(), i, GetBaseObjectOrNull(all[i]));

		vehicles.Reset(isolate, jsAll);
		return jsAll;
	}

	return vehicles.Get(isolate);
}

std::vector<V8::EventCallback *> V8ResourceImpl::GetLocalHandlers(const std::string &name)
{
	std::vector<V8::EventCallback *> handlers;
	auto range = localHandlers.equal_range(name);

	for (auto it = range.first; it != range.second; ++it)
		handlers.push_back(&it->second);

	return handlers;
}

std::vector<V8::EventCallback *> V8ResourceImpl::GetRemoteHandlers(const std::string &name)
{
	std::vector<V8::EventCallback *> handlers;
	auto range = remoteHandlers.equal_range(name);

	for (auto it = range.first; it != range.second; ++it)
		handlers.push_back(&it->second);

	return handlers;
}

void V8ResourceImpl::InvokeEventHandlers(const alt::CEvent *ev, const std::vector<V8::EventCallback *> &handlers, std::vector<v8::Local<v8::Value>> &args)
{
	for (auto handler : handlers)
	{
		int64_t time = GetTime();

		if (handler->removed)
			continue;

		V8Helpers::TryCatch([&] {
			v8::MaybeLocal<v8::Value> retn = handler->fn.Get(isolate)->Call(GetContext(), v8::Undefined(isolate), args.size(), args.data());

			if (retn.IsEmpty())
				return false;

			if (ev && retn.ToLocalChecked()->IsFalse())
				ev->Cancel();

			return true;
		});

		if (GetTime() - time > 5)
		{
			if (handler->location.GetLineNumber() != 0)
			{
				Log::Warning << "Event handler at "
							 << resource->GetName() << ":" << handler->location.GetFileName() << ":" << handler->location.GetLineNumber()
							 << " was too long " << (GetTime() - time) << "ms" << Log::Endl;
			}
			else
			{
				Log::Warning << "Event handler at "
							 << resource->GetName() << ":" << handler->location.GetFileName()
							 << " was too long " << (GetTime() - time) << "ms" << Log::Endl;
			}
		}
	}
}

alt::MValue V8ResourceImpl::FunctionImpl::Call(alt::MValueArgs args) const
{
	v8::Isolate *isolate = resource->GetIsolate();

	v8::Locker locker(isolate);
	v8::Isolate::Scope isolateScope(isolate);
	v8::HandleScope handleScope(isolate);

	v8::Local<v8::Context> ctx = resource->GetContext();
	v8::Context::Scope scope(ctx);

#ifdef ALT_SERVER_API
	CNodeResourceImpl *nodeRes = static_cast<CNodeResourceImpl *>(resource);
	node::CallbackScope callbackScope(isolate, nodeRes->GetAsyncResource(), nodeRes->GetAsyncContext());
#endif // ALT_SERVER_API

	std::vector<v8::Local<v8::Value>> v8Args;
	V8Helpers::MValueArgsToV8(args, v8Args);

	alt::MValue res;
	V8Helpers::TryCatch([&] {
		v8::MaybeLocal<v8::Value> _res = function.Get(isolate)->CallAsFunction(resource->GetContext(), v8::Undefined(isolate), v8Args.size(), v8Args.data());

		if (_res.IsEmpty())
			return false;

		res = V8Helpers::V8ToMValue(_res.ToLocalChecked());
		return true;
	});

	if (res.IsEmpty())
		res = alt::ICore::Instance().CreateMValueNone();

	return res;
}
