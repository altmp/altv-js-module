#pragma once

#include <vector>
#include <map>
#include <functional>
#include <v8.h>
#include "V8Class.h"
#include <unordered_set>

class V8Module {
    using Callback = std::function<void(v8::Local<v8::Context> ctx, v8::Local<v8::Object> exports)>;
    
public:

    static std::map<std::string, V8Module*>& All()
    {
        static std::map<std::string, V8Module*> all;
        return all;
    }

    static bool Exists(const std::string& name)
    {
        if (All().find(name) == All().end())
			return false;
		else
			return true;
    }

    std::string moduleName;
    std::unordered_set<std::string> classes;
    Callback creator;

    V8Module(
        std::string moduleName,
        std::initializer_list<std::string> _classes,
        Callback fn
    ) : moduleName(moduleName), classes(_classes), creator(fn)
    {
        All()[moduleName] = this;
    }

	void Register(v8::Isolate* isolate, v8::Local<v8::Context> context, v8::Local<v8::Object> exports)
	{
		// Load all classes
		for (auto& c : classes)
		{
			V8Class* v8class = V8Class::Get(c);
			if (!v8class)
			{
				auto msg = "[V8] Module '" + moduleName
					+ "' is attempting to load non-existant class '" + c + "'";

				Log::Error << msg << Log::Endl;
				//throw std::runtime_error(msg);
			}

			v8class->Register(isolate, context, exports);
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
