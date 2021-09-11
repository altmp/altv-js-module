#pragma once

#include "cpp-sdk/IScriptRuntime.h"
#include "cpp-sdk/events/CRemoveEntityEvent.h"

#include "helpers/V8Helpers.h"
#include "CNodeResourceImpl.h"

class CNodeScriptRuntime : public alt::IScriptRuntime
{
	v8::Isolate* isolate;
	std::unique_ptr<node::MultiIsolatePlatform> platform;
	std::unordered_set<CNodeResourceImpl*> resources;

public:
	CNodeScriptRuntime();
	~CNodeScriptRuntime();

	v8::Isolate* GetIsolate() { return isolate; }

	alt::IResource::Impl* CreateImpl(alt::IResource* resource) override;

	void DestroyImpl(alt::IResource::Impl* impl) override
	{
		resources.insert(static_cast<CNodeResourceImpl*>(impl));
		delete static_cast<CNodeResourceImpl*>(impl);
	}

	void OnTick() override;
	void OnDispose() override;

	node::MultiIsolatePlatform* GetPlatform() const { return platform.get(); }

	std::unordered_set<CNodeResourceImpl*> GetResources() { return resources; }

	static CNodeScriptRuntime& Instance()
	{
		static CNodeScriptRuntime _Instance;
		return _Instance;
	}
};
