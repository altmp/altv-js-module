#pragma once

#include "cpp-sdk/types/MValue.h"
#include "cpp-sdk/types/IConnectionInfo.h"
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
    void OnTick() override;

    bool MakeClient(alt::IResource::CreationInfo* info, alt::Array<std::string>) override;

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

    void AddConnectionInfoObject(alt::IConnectionInfo* info, v8::Local<v8::Object> obj)
    {
        connectionInfoMap.insert({ info, V8Helpers::CPersistent<v8::Object>(isolate, obj) });
    }
    void RemoveConnectionInfoObject(alt::IConnectionInfo* info)
    {
        connectionInfoMap.erase(info);
    }
    v8::Local<v8::Object> GetConnectionInfoObject(alt::IConnectionInfo* info)
    {
        auto it = connectionInfoMap.find(info);
        if(it != connectionInfoMap.end())
        {
            return it->second.Get(isolate);
        }
        return v8::Local<v8::Object>();
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

    std::unordered_map<alt::IConnectionInfo*, V8Helpers::CPersistent<v8::Object>> connectionInfoMap;
};
