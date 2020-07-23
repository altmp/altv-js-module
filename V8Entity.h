#pragma once

#include <v8.h>

#include "cpp-sdk/entities/IEntity.h"

#include "V8Class.h"

class V8Entity
{
	V8Class* _class;
	alt::Ref<alt::IBaseObject> handle;
	v8::Persistent<v8::Object> jsVal;

public:
	/*V8Entity(v8::Local<v8::Context> ctx, V8Class* __class, alt::Ref<alt::IEntity> _handle) :
		_class(__class),
		handle(_handle)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();

		v8::Local<v8::Object> obj = _class->New(ctx);

		//v8::Local<v8::Object> meta = EntityMeta::Get(this);

		obj->SetInternalField(0, v8::External::New(isolate, this));
		//obj->Set(v8::String::NewFromUtf8(isolate, "meta"), meta);

		jsVal.Reset(isolate, obj);
	}*/

	V8Entity(v8::Local<v8::Context> ctx, V8Class* __class, v8::Local<v8::Object> obj, alt::Ref<alt::IBaseObject> _handle) :
		_class(__class),
		handle(_handle)
	{
		v8::Isolate* isolate = v8::Isolate::GetCurrent();
		obj->SetInternalField(0, v8::External::New(isolate, this));
		jsVal.Reset(isolate, obj);
	}

	V8Class* GetClass() { return _class; }

	alt::Ref<alt::IBaseObject> GetHandle() { return handle; }

	v8::Local<v8::Object> GetJSVal() { return jsVal.Get(v8::Isolate::GetCurrent()); }

	static bool IsEntity(v8::Local<v8::Value> val)
	{
		if (!val->IsObject())
			return false;

		v8::Local<v8::Object> obj = val.As<v8::Object>();
		if (obj->InternalFieldCount() != 1)
			return false;

		return true;
	}

	static V8Entity* Get(v8::Local<v8::Value> val)
	{
		if (!val->IsObject())
			return nullptr;

		v8::Local<v8::Object> obj = val.As<v8::Object>();
		if (obj->InternalFieldCount() != 1)
			return nullptr;

		v8::Local<v8::Value> i = obj->GetInternalField(0);
		if (!i->IsExternal())
			return nullptr;

		return static_cast<V8Entity*>(i.As<v8::External>()->Value());
	}

	static V8Class* GetClass(alt::Ref<alt::IBaseObject> handle)
	{
		if (!handle)
			return nullptr;

		switch (handle->GetType())
		{
		case alt::IBaseObject::Type::PLAYER:
			return V8Class::Get("Player");
		case alt::IBaseObject::Type::VEHICLE:
			return V8Class::Get("Vehicle");
		case alt::IBaseObject::Type::BLIP:
			return V8Class::Get("Blip");
		case alt::IBaseObject::Type::WEBVIEW:
			return V8Class::Get("WebView");
		case alt::IBaseObject::Type::VOICE_CHANNEL:
			return V8Class::Get("VoiceChannel");
		case alt::IBaseObject::Type::COLSHAPE:
			return V8Class::Get("Colshape");
		case alt::IBaseObject::Type::CHECKPOINT:
			return V8Class::Get("Checkpoint");
		}

		return nullptr;
	}
};