#pragma once

#include "cpp-sdk/types/MValue.h"
#include "cpp-sdk/script-objects/IConnectionInfo.h"
#include "cpp-sdk/IPackage.h"
#include "cpp-sdk/IResource.h"

#include "V8ResourceImpl.h"
#include "V8Entity.h"
#include "V8Helpers.h"

#include "node.h"
#include "uv.h"

class CNodeScriptRuntime;

class CNodeResourceImpl : public V8ResourceImpl
{
public:
    CNodeResourceImpl(CNodeScriptRuntime* _runtime, v8::Isolate* isolate, alt::IResource* resource);

    CNodeResourceImpl(const CNodeResourceImpl&) = delete;

    bool Start() override;
    bool Stop() override;

    void OnEvent(const alt::CEvent* ev) override;
    void HandleVehiclePassengerSeatEvents(const alt::CEvent* ev);
    void HandleClientRpcEvent(alt::CScriptRPCEvent* ev);
    void HandleClientRpcAnswerEvent(const alt::CScriptRPCAnswerEvent* ev);

    void OnTick() override;

    bool MakeClient(alt::IResource::CreationInfo* info, std::vector<std::string>) override;

    void Started(v8::Local<v8::Value> exports);
    node::Environment* GetEnv()
    {
        return env;
    }
    node::async_context GetAsyncContext()
    {
        return asyncContext;
    }
    v8::Local<v8::Object> GetAsyncResource()
    {
        return asyncResource.Get(isolate);
    }
    bool IsEnvStarted()
    {
        return envStarted;
    }

private:
    CNodeScriptRuntime* runtime;

    bool envStarted = false;
    bool startError = false;

    node::IsolateData* nodeData = nullptr;
    node::Environment* env = nullptr;
    uv_loop_t* uvLoop = nullptr;
    V8Helpers::CPersistent<v8::Object> asyncResource;
    node::async_context asyncContext{};
};
