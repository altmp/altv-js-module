#pragma once

#include <vector>
#include <map>
#include <functional>
#include <v8.h>
#include "V8Class.h"
#include <unordered_set>

class V8Module
{
    using Callback = std::function<void(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)>;

public:
    static std::map<std::string, V8Module *> &All()
    {
        static std::map<std::string, V8Module *> all;
        return all;
    }

    static void Add(V8Module &module)
    {
        All()[module.moduleName] = &module;
    }

    static void Add(std::initializer_list<std::reference_wrapper<V8Module>> modules)
    {
        for (auto &m : modules)
            All()[m.get().moduleName] = &m.get();
    }

    static bool Exists(const std::string &name)
    {
        if (All().find(name) == All().end())
            return false;
        else
            return true;
    }

    std::string moduleName;
    std::unordered_set<V8Class *> classes;
    Callback creator;

    template <class... cc>
    V8Module(
        std::string moduleName,
        std::initializer_list<std::reference_wrapper<V8Class>> _classes,
        Callback fn) : moduleName(moduleName), creator(fn)
    {
        for (auto &c : _classes)
            classes.insert(&c.get());

        // All()[moduleName] = this;
    }

    void Register(v8::Isolate *isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> exports)
    {
        // Load all classes
        for (auto c : classes)
        {
            c->Register(isolate, context, exports);
        }

        creator(context, exports);
    }

    v8::Local<v8::Object> GetExports(v8::Isolate *isolate, v8::Local<v8::Context> context)
    {
        v8::Local<v8::Object> _exports = v8::Object::New(isolate);
        Register(isolate, context, _exports);
        return _exports;
    }
};
