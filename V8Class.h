#pragma once

#include <functional>
#include <v8.h>

#include "Log.h"

class V8Class
{
	using InitCallback = std::function<void(v8::Local<v8::FunctionTemplate>)>;

	V8Class *parent = nullptr;
	std::string name;
	v8::FunctionCallback constructor;
	InitCallback initCb;
	v8::Persistent<v8::FunctionTemplate> tpl;
	bool loaded = false;

public:
	static auto &All()
	{
		static std::unordered_map<std::string, V8Class *> _All;
		return _All;
	}

	V8Class(
		const std::string &className,
		V8Class &parent,
		v8::FunctionCallback constructor,
		InitCallback &&init = {}) : parent(&parent),
									name(className),
									constructor(constructor),
									initCb(std::move(init))
	{
		All()[name] = this;
	}

	V8Class(
		const std::string &className,
		V8Class &parent,
		InitCallback &&init = {}) : parent(&parent),
									name(className),
									initCb(std::move(init))
	{
		All()[name] = this;
	}

	V8Class(
		const std::string &className,
		v8::FunctionCallback constructor,
		InitCallback &&init = {}) : name(className),
									constructor(constructor),
									initCb(std::move(init))
	{
		All()[name] = this;
	}

	V8Class(
		const std::string &className,
		InitCallback &&init = {}) : name(className),
									initCb(std::move(init))
	{
		All()[name] = this;
	}

	const std::string &GetName()
	{
		return name;
	}

	v8::Local<v8::Object> CreateInstance(v8::Local<v8::Context> ctx)
	{
		v8::Isolate *isolate = v8::Isolate::GetCurrent();

		v8::Local<v8::FunctionTemplate> _tpl = tpl.Get(isolate);
		v8::Local<v8::Object> obj = _tpl->InstanceTemplate()->NewInstance(ctx).ToLocalChecked();

		return obj;
	}

	v8::Local<v8::Value> CreateInstance(v8::Isolate *isolate, v8::Local<v8::Context> ctx, std::vector<v8::Local<v8::Value>> args);

	v8::Local<v8::Function> JSValue(v8::Isolate *isolate, v8::Local<v8::Context> ctx)
	{
		return tpl.Get(isolate)->GetFunction(ctx).ToLocalChecked();
	}

	v8::Local<v8::Value> New(v8::Local<v8::Context> ctx, std::vector<v8::Local<v8::Value>> &args);

	static void LoadAll(v8::Isolate *isolate)
	{
		for (auto &p : All())
			p.second->Load(isolate);
	}

	void Load(v8::Isolate *isolate)
	{
		if (loaded)
			return;

		loaded = true;

		v8::Local<v8::FunctionTemplate> _tpl = v8::FunctionTemplate::New(isolate, constructor);
		_tpl->SetClassName(v8::String::NewFromUtf8(isolate, name.c_str(), v8::NewStringType::kNormal).ToLocalChecked());

		if (initCb)
			initCb(_tpl);

		if (parent)
		{
			parent->Load(isolate);
			_tpl->Inherit(parent->tpl.Get(isolate));
		}

		if (!tpl.IsEmpty())
		{
			Log::Error << "Already loaded " << name << Log::Endl;
		}

		tpl.Reset(isolate, _tpl);
	}

	void Register(v8::Isolate *isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> exports)
	{
		exports->Set(context, v8::String::NewFromUtf8(isolate, name.c_str(), v8::NewStringType::kNormal).ToLocalChecked(), tpl.Get(isolate)->GetFunction(context).ToLocalChecked());
	}
};