#pragma once

#include "cpp-sdk/IResource.h"
#include "cpp-sdk/objects/IEntity.h"

#include "helpers/V8ResourceImpl.h"

#include <queue>

#include "helpers/Log.h"

class CV8ScriptRuntime;

class CV8ResourceImpl : public V8ResourceImpl
{
public:
	CV8ResourceImpl(alt::IResource *resource, v8::Isolate *isolate) : V8ResourceImpl(isolate, resource)
	{
	}

	~CV8ResourceImpl()
	{
		Log::Debug << __FUNCTION__ << Log::Endl;
	}

	bool Start() override;

	bool Stop() override;

	bool OnEvent(const alt::CEvent *ev) override;

	void OnTick() override;

	void OnPromiseRejectedWithNoHandler(v8::PromiseRejectMessage &data);
	void OnPromiseHandlerAdded(v8::PromiseRejectMessage &data);

	void SubscribeWebView(alt::Ref<alt::IWebView> view, const std::string &evName, v8::Local<v8::Function> cb, V8::SourceLocation &&location)
	{
		webViewHandlers[view].insert({evName, V8::EventCallback{isolate, cb, std::move(location)}});
	}

	void UnsubscribeWebView(alt::Ref<alt::IWebView> view, const std::string &evName, v8::Local<v8::Function> cb)
	{
		auto it = webViewHandlers.find(view);
		if (it != webViewHandlers.end())
		{
			auto &viewEvents = it->second;
			auto range = viewEvents.equal_range(evName);

			for (auto it = range.first; it != range.second; ++it)
			{
				if (it->second.fn.Get(isolate)->StrictEquals(cb))
					it->second.removed = true;
			}
		}
	}

	void AddOwned(alt::Ref<alt::IBaseObject> handle)
	{
		ownedObjects.insert(handle);
	}

	void OnRemoveBaseObject(alt::Ref<alt::IBaseObject> handle)
	{
		ownedObjects.erase(handle);

		if (handle->GetType() == alt::IBaseObject::Type::WEBVIEW)
			webViewHandlers.erase(handle.As<alt::IWebView>());

		V8ResourceImpl::OnRemoveBaseObject(handle);
	}

	v8::Local<v8::Object> GetLocalStorage()
	{
		if (localStorage.IsEmpty())
		{
			std::vector<v8::Local<v8::Value>> args;
			extern V8Class v8LocalStorage;
			localStorage.Reset(isolate, v8LocalStorage.New(GetContext(), args).As<v8::Object>());
		}

		return localStorage.Get(isolate);
	}

	bool IsValidModule(const std::string &name);
	std::deque<std::string> GetModuleKeys(const std::string &name);
	std::string GetModulePath(v8::Local<v8::Module> moduleHandle);

	v8::MaybeLocal<v8::Value> Require(const std::string &name);
	v8::MaybeLocal<v8::Module> ResolveFile(const std::string &name, v8::Local<v8::Module> referrer);
	v8::MaybeLocal<v8::Module> ResolveModule(const std::string &name, v8::Local<v8::Module> referrer);

private:
	using WebViewEvents = std::unordered_multimap<std::string, V8::EventCallback>;

	CV8ScriptRuntime *runtime;

	std::unordered_map<std::string, v8::UniquePersistent<v8::Value>> requires;
	std::unordered_map<std::string, v8::UniquePersistent<v8::Module>> modules;

	std::unordered_map<alt::Ref<alt::IWebView>, WebViewEvents> webViewHandlers;

	std::unordered_set<alt::Ref<alt::IBaseObject>> ownedObjects;

	v8::Persistent<v8::Object> localStorage;
};
