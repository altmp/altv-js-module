
#include "cpp-sdk/objects/IPlayer.h"
#include "cpp-sdk/objects/IVehicle.h"

#include "V8ResourceImpl.h"

#ifdef ALT_SERVER_API
    #include "CNodeResourceImpl.h"
    #include "node.h"
#endif

using namespace alt;

V8ResourceImpl::~V8ResourceImpl() {}

extern V8Class v8Vector3, v8Vector2, v8RGBA, v8BaseObject;
bool V8ResourceImpl::Start()
{
    vector3Class.Reset(isolate, v8Vector3.JSValue(isolate, GetContext()));
    vector2Class.Reset(isolate, v8Vector2.JSValue(isolate, GetContext()));
    rgbaClass.Reset(isolate, v8RGBA.JSValue(isolate, GetContext()));
    baseObjectClass.Reset(isolate, v8BaseObject.JSValue(isolate, GetContext()));

    return true;
}

void V8ResourceImpl::OnTick()
{
    for(auto& id : oldTimers) timers.erase(id);

    oldTimers.clear();

    for(auto& p : timers)
    {
        int64_t time = GetTime();

        if(!p.second->Update(time)) RemoveTimer(p.first);

        if(GetTime() - time > 10)
        {
            auto& location = p.second->GetLocation();

            if(location.GetLineNumber() != 0)
            {
                Log::Warning << "Timer at " << resource->GetName() << ":" << location.GetFileName() << ":" << location.GetLineNumber() << " was too long " << GetTime() - time << "ms"
                             << Log::Endl;
            }
            else
            {
                Log::Warning << "Timer at " << resource->GetName() << ":" << location.GetFileName() << " was too long " << GetTime() - time << "ms" << Log::Endl;
            }
        }
    }

    for(auto it = localHandlers.begin(); it != localHandlers.end();)
    {
        if(it->second.removed) it = localHandlers.erase(it);
        else
            ++it;
    }

    for(auto it = remoteHandlers.begin(); it != remoteHandlers.end();)
    {
        if(it->second.removed) it = remoteHandlers.erase(it);
        else
            ++it;
    }

    for(auto it = localGenericHandlers.begin(); it != localGenericHandlers.end(); it++)
    {
        if(it->removed) localGenericHandlers.erase(it);
    }

    for(auto it = remoteGenericHandlers.begin(); it != remoteGenericHandlers.end(); it++)
    {
        if(it->removed) remoteGenericHandlers.erase(it);
    }

    promiseRejections.ProcessQueue(this);
}

std::vector<V8::EventCallback*> V8ResourceImpl::GetLocalHandlers(const std::string& name)
{
    std::vector<V8::EventCallback*> handlers;
    auto range = localHandlers.equal_range(name);

    for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);

    return handlers;
}

std::vector<V8::EventCallback*> V8ResourceImpl::GetRemoteHandlers(const std::string& name)
{
    std::vector<V8::EventCallback*> handlers;
    auto range = remoteHandlers.equal_range(name);

    for(auto it = range.first; it != range.second; ++it) handlers.push_back(&it->second);

    return handlers;
}

std::vector<V8::EventCallback*> V8ResourceImpl::GetGenericHandlers(bool local)
{
    std::vector<V8::EventCallback*> handlers;
    if(local)
    {
        for(auto& it : localGenericHandlers)
        {
            handlers.push_back(&it);
        }
    }
    else
    {
        for(auto& it : remoteGenericHandlers)
        {
            handlers.push_back(&it);
        }
    }
    return handlers;
}

void V8ResourceImpl::InvokeEventHandlers(const alt::CEvent* ev, const std::vector<V8::EventCallback*>& handlers, std::vector<v8::Local<v8::Value>>& args)
{
    for(auto handler : handlers)
    {
        int64_t time = GetTime();

        if(handler->removed) continue;

        V8Helpers::TryCatch([&] {
            v8::MaybeLocal<v8::Value> retn = handler->fn.Get(isolate)->Call(GetContext(), v8::Undefined(isolate), args.size(), args.data());

            if(retn.IsEmpty()) return false;

            if(ev && retn.ToLocalChecked()->IsFalse()) ev->Cancel();

            return true;
        });

        if(GetTime() - time > 5)
        {
            if(handler->location.GetLineNumber() != 0)
            {
                Log::Warning << "Event handler at " << resource->GetName() << ":" << handler->location.GetFileName() << ":" << handler->location.GetLineNumber() << " was too long "
                             << (GetTime() - time) << "ms" << Log::Endl;
            }
            else
            {
                Log::Warning << "Event handler at " << resource->GetName() << ":" << handler->location.GetFileName() << " was too long " << (GetTime() - time) << "ms" << Log::Endl;
            }
        }

        if(handler->once) handler->removed = true;
    }
}

alt::MValue V8ResourceImpl::FunctionImpl::Call(alt::MValueArgs args) const
{
    v8::Isolate* isolate = resource->GetIsolate();

    v8::Locker locker(isolate);
    v8::Isolate::Scope isolateScope(isolate);
    v8::HandleScope handleScope(isolate);

    v8::Local<v8::Context> ctx = resource->GetContext();
    v8::Context::Scope scope(ctx);

#ifdef ALT_SERVER_API
    CNodeResourceImpl* nodeRes = static_cast<CNodeResourceImpl*>(resource);
    node::CallbackScope callbackScope(isolate, nodeRes->GetAsyncResource(), nodeRes->GetAsyncContext());
#endif  // ALT_SERVER_API

    std::vector<v8::Local<v8::Value>> v8Args;
    V8Helpers::MValueArgsToV8(args, v8Args);

    alt::MValue res;
    V8Helpers::TryCatch([&] {
        v8::MaybeLocal<v8::Value> _res = function.Get(isolate)->CallAsFunction(resource->GetContext(), v8::Undefined(isolate), v8Args.size(), v8Args.data());

        if(_res.IsEmpty()) return false;

        res = V8Helpers::V8ToMValue(_res.ToLocalChecked());
        return true;
    });

    if(res.IsEmpty()) res = alt::ICore::Instance().CreateMValueNone();

    return res;
}
