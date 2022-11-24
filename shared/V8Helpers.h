#pragma once

#include <vector>
#include <functional>

#include <v8.h>
#include <limits>

#include "cpp-sdk/objects/IEntity.h"
#include "cpp-sdk/types/MValue.h"
#include "V8Entity.h"

#include "helpers/Serialization.h"
#include "helpers/Convert.h"
#include "helpers/Macros.h"
#include "helpers/Bindings.h"

class V8ResourceImpl;

namespace V8Helpers
{
    inline void Throw(v8::Isolate* isolate, const std::string& msg)
    {
        isolate->ThrowException(v8::Exception::Error(v8::String::NewFromUtf8(isolate, msg.data(), v8::NewStringType::kNormal, (int)msg.size()).ToLocalChecked()));
    }

    bool TryCatch(const std::function<bool()>& fn);

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

    template<typename T>
    using CPersistent = v8::Persistent<T, v8::CopyablePersistentTraits<T>>;

    class SourceLocation
    {
    public:
        SourceLocation(std::string&& fileName, int line, v8::Local<v8::Context> ctx);

        const std::string& GetFileName() const
        {
            return fileName;
        }
        int GetLineNumber() const
        {
            return line;
        }

        std::string ToString();

        static SourceLocation GetCurrent(v8::Isolate* isolate, V8ResourceImpl* resource = nullptr);

    private:
        CPersistent<v8::Context> context;
        std::string fileName;
        int line = 0;
    };

    class StackTrace
    {
        struct Frame
        {
            std::string file;
            std::string function;
            int line;
        };
        std::vector<Frame> frames;
        CPersistent<v8::Context> context;

    public:
        StackTrace(std::vector<Frame>&& frames, v8::Local<v8::Context> ctx);

        const std::vector<Frame>& GetFrames() const
        {
            return frames;
        }

        void Print(uint32_t offset = 0) const;
        std::string ToString(uint32_t offset = 0) const;

        static StackTrace GetCurrent(v8::Isolate* isolate, V8ResourceImpl* resource = nullptr);
        static void Print(v8::Isolate* isolate);
    };

    struct EventCallback
    {
        v8::Global<v8::Function> fn;
        SourceLocation location;
        bool removed = false;
        bool once;

        EventCallback(v8::Isolate* isolate, v8::Local<v8::Function> _fn, SourceLocation&& location, bool once = false) : fn(isolate, _fn), location(std::move(location)), once(once) {}
    };

    class EventHandler
    {
    public:
        using CallbacksGetter = std::function<std::vector<EventCallback*>(V8ResourceImpl* resource, const alt::CEvent*)>;
        using ArgsGetter = std::function<void(V8ResourceImpl* resource, const alt::CEvent*, std::vector<v8::Local<v8::Value>>& args)>;

        EventHandler(alt::CEvent::Type type, CallbacksGetter&& _handlersGetter, ArgsGetter&& _argsGetter);

        // Temp issue fix for https://stackoverflow.com/questions/9459980/c-global-variable-not-initialized-when-linked-through-static-libraries-but-ok
        void Reference();

        std::vector<V8Helpers::EventCallback*> GetCallbacks(V8ResourceImpl* impl, const alt::CEvent* e);
        std::vector<v8::Local<v8::Value>> GetArgs(V8ResourceImpl* impl, const alt::CEvent* e);

        alt::CEvent::Type GetType()
        {
            return type;
        }

        static EventHandler* Get(const alt::CEvent* e);
        static alt::CEvent::Type GetTypeForEventName(const std::string& event);

    protected:
        alt::CEvent::Type type;
        CallbacksGetter callbacksGetter;
        ArgsGetter argsGetter;

        static std::unordered_map<std::string, EventHandler*>& eventNameToHandlerMap()
        {
            static std::unordered_map<std::string, EventHandler*> map;
            return map;
        }
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
        LocalEventHandler(alt::CEvent::Type type, const std::string& name, ArgsGetter&& argsGetter) : EventHandler(type, std::move(GetCallbacksGetter(name)), std::move(argsGetter))
        {
            eventNameToHandlerMap().insert({ name, this });
        }

    private:
        static CallbacksGetter GetCallbacksGetter(const std::string& name);
    };

    v8::Local<v8::Value> Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, const char* name);
    v8::Local<v8::Value> Get(v8::Local<v8::Context> ctx, v8::Local<v8::Object> obj, v8::Local<v8::Name> name);

    v8::Local<v8::Value> New(v8::Isolate* isolate, v8::Local<v8::Context> ctx, v8::Local<v8::Function> constructor, std::vector<v8::Local<v8::Value>>& args);

    v8::Local<v8::Object> CreateCustomObject(v8::Isolate* isolate,
                                             void* data,
                                             v8::GenericNamedPropertyGetterCallback getter,
                                             v8::GenericNamedPropertySetterCallback setter = nullptr,
                                             v8::GenericNamedPropertyDeleterCallback deleter = nullptr,
                                             v8::GenericNamedPropertyEnumeratorCallback enumerator = nullptr,
                                             v8::GenericNamedPropertyQueryCallback query = nullptr);

    // TODO: create c++ classes for v8 classes and move there
    v8::Local<v8::String> Vector3_XKey(v8::Isolate* isolate);
    v8::Local<v8::String> Vector3_YKey(v8::Isolate* isolate);
    v8::Local<v8::String> Vector3_ZKey(v8::Isolate* isolate);

    v8::Local<v8::String> RGBA_RKey(v8::Isolate* isolate);
    v8::Local<v8::String> RGBA_GKey(v8::Isolate* isolate);
    v8::Local<v8::String> RGBA_BKey(v8::Isolate* isolate);
    v8::Local<v8::String> RGBA_AKey(v8::Isolate* isolate);

    v8::Local<v8::String> Fire_PosKey(v8::Isolate* isolate);
    v8::Local<v8::String> Fire_WeaponKey(v8::Isolate* isolate);

    std::string Stringify(v8::Local<v8::Context> ctx, v8::Local<v8::Value> val);
    std::string GetJSValueTypeName(v8::Local<v8::Value> val);

    inline std::string GetStackTrace(const std::string errorMsg)
    {
        auto isolate = v8::Isolate::GetCurrent();
        auto ctx = isolate->GetEnteredOrMicrotaskContext();

        auto exception = v8::Exception::Error(JSValue(errorMsg));
        auto stackTrace = v8::TryCatch::StackTrace(ctx, exception).ToLocalChecked();
        return *v8::String::Utf8Value(isolate, stackTrace);
    }

    inline std::string GetCurrentSourceOrigin(v8::Isolate* isolate)
    {
        auto stackTrace = v8::StackTrace::CurrentStackTrace(isolate, 1);
        if(stackTrace->GetFrameCount() == 0) return "";
        v8::Local<v8::String> scriptName = stackTrace->GetFrame(isolate, 0)->GetScriptName();
        if(scriptName.IsEmpty()) return "";
        v8::String::Utf8Value strValue(isolate, scriptName);
        if(strValue.length() == 0) return "";
        return *strValue;
    }

    v8::MaybeLocal<v8::Value> CallFunctionWithTimeout(v8::Local<v8::Function> fn, v8::Local<v8::Context> ctx, std::vector<v8::Local<v8::Value>>& args, uint32_t timeout = 5000);

}  // namespace V8Helpers
