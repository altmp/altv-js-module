#pragma once

#include "v8.h"
#include "V8Helpers.h"
#include <queue>

class IImportHandler
{
protected:
    std::unordered_map<std::string, v8::UniquePersistent<v8::Value>> requires;
    std::unordered_map<std::string, v8::UniquePersistent<v8::Module>> modules;

    bool isUsingBytecode = false;

public:
    bool IsValidModule(const std::string& name);
    bool IsBytecodeModule(uint8_t* buffer, size_t size);
    std::deque<std::string> GetModuleKeys(const std::string& name);
    std::string GetModulePath(v8::Local<v8::Module> moduleHandle);
    v8::Local<v8::Module> GetModuleFromPath(std::string modulePath);

    v8::MaybeLocal<v8::Value> Require(const std::string& name);
    v8::MaybeLocal<v8::Module> ResolveFile(const std::string& name, v8::Local<v8::Module> referrer, alt::IResource* resource);
    v8::MaybeLocal<v8::Module> ResolveModule(const std::string& name, v8::Local<v8::Module> referrer, alt::IResource* resource);
    v8::MaybeLocal<v8::Module> ResolveCode(const std::string& code, const V8Helpers::SourceLocation& location);
    v8::MaybeLocal<v8::Module> ResolveBytecode(uint8_t* buffer, size_t size);
};
