#pragma once

#include "v8.h"
#include "V8Helpers.h"
#include <queue>

class IImportHandler
{
protected:
    struct ModuleData
    {
        V8Helpers::CPersistent<v8::Module> mod;
        bool isBytecode = false;
    };

    std::unordered_map<std::string, V8Helpers::CPersistent<v8::Value>> requiresMap;
    std::unordered_map<std::string, ModuleData> modules;

    bool IsValidModule(const std::string& name);
    bool IsBytecodeModule(uint8_t* buffer, size_t size);

public:
    std::deque<std::string> GetModuleKeys(const std::string& name);
    std::string GetModulePath(v8::Local<v8::Module> moduleHandle);
    const ModuleData GetModuleData(const std::string& name);
    v8::Local<v8::Module> GetModuleFromPath(std::string modulePath);

    v8::MaybeLocal<v8::Value> Require(const std::string& name);
    v8::MaybeLocal<v8::Module> ResolveFile(const std::string& name, v8::Local<v8::Module> referrer, alt::IResource* resource);
    v8::MaybeLocal<v8::Module> ResolveModule(const std::string& name, v8::Local<v8::Module> referrer, alt::IResource* resource);
    v8::MaybeLocal<v8::Module> ResolveCode(const std::string& code, const V8Helpers::SourceLocation& location);
    v8::MaybeLocal<v8::Module> ResolveBytecode(const std::string& name, uint8_t* buffer, size_t size);
};
