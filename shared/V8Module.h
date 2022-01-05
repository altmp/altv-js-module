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
    using ModuleMap = std::unordered_map<v8::Isolate*, std::unordered_map<std::string, V8Module*>>;

public:
    static ModuleMap& All()
    {
        static ModuleMap all;
        return all;
    }

    static void Add(v8::Isolate* isolate, V8Module& mod)
    {
        All()[isolate][mod.moduleName] = &mod;
    }

    static void Add(v8::Isolate* isolate, std::initializer_list<std::reference_wrapper<V8Module>> modules)
    {
        for(auto& m : modules) All()[isolate][m.get().moduleName] = &m.get();
    }

    static void Clear(v8::Isolate* isolate)
    {
        All().erase(isolate);
    }

    static bool Exists(v8::Isolate* isolate, const std::string& name)
    {
        if(All()[isolate].find(name) == All()[isolate].end()) return false;
        else
            return true;
    }

    std::string moduleName;
    std::unordered_set<V8Class*> classes;
    Callback creator;
    V8Module* parentModule;

    template<class... cc>
    V8Module(std::string moduleName, V8Module* parent, std::initializer_list<std::reference_wrapper<V8Class>> _classes, Callback fn)
        : moduleName(moduleName), creator(fn), parentModule(parent)
    {
        for(auto& c : _classes) classes.insert(&c.get());

        // All()[moduleName] = this;
    }

    void Register(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> exports)
    {
        if(parentModule) parentModule->Register(isolate, context, exports);
        // Load all classes
        for(auto c : classes)
        {
            c->Register(isolate, context, exports);
        }

        creator(context, exports);
    }

    v8::Local<v8::Object> GetExports(v8::Isolate* isolate, v8::Local<v8::Context> context)
    {
        v8::Local<v8::Object> _exports = v8::Object::New(isolate);
        Register(isolate, context, _exports);
        return _exports;
    }
};
