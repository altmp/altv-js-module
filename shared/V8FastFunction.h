#pragma once

#ifdef ALT_CLIENT

    #include "v8.h"
    #include "v8-fast-api-calls.h"

    #include <unordered_map>
    #include <functional>

class V8FastFunction
{
    std::unordered_map<v8::Isolate*, v8::Persistent<v8::FunctionTemplate, v8::CopyablePersistentTraits<v8::FunctionTemplate>>> tplMap;
    v8::FunctionCallback slowCallback;
    v8::CFunction fastCallback;

    static auto& All()
    {
        static std::unordered_map<std::string, V8FastFunction*> _All;
        return _All;
    }
    static V8FastFunction* Get(const std::string& name, const std::string& className);

    static inline std::string GetIdentifier(const std::string& name, const std::string& className)
    {
        return name + ":" + className;
    }

    V8FastFunction() = default;

public:
    V8FastFunction(const V8FastFunction&) = delete;
    V8FastFunction(V8FastFunction&&) = delete;
    V8FastFunction& operator=(V8FastFunction&) = delete;

    v8::Local<v8::FunctionTemplate> GetTemplate(v8::Isolate* isolate);

    template<typename Func>
    static V8FastFunction* GetOrCreate(const std::string& name, const std::string& className, v8::FunctionCallback slowFunc, Func&& fastFunc)
    {
        // First check if we have this fast function already cached
        V8FastFunction* cached = Get(name, className);
        if(cached != nullptr) return cached;

        // Not cached, create a new instance
        V8FastFunction* f = new V8FastFunction();
        f->slowCallback = slowFunc;
        f->fastCallback = v8::CFunction::Make(fastFunc);
        All().insert({ GetIdentifier(name, className), f });
        return f;
    }

    static void UnloadAll(v8::Isolate* isolate);
};

#endif
