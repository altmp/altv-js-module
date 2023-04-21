#include "IImportHandler.h"
#include "V8Module.h"

static inline v8::MaybeLocal<v8::Module> CompileESM(v8::Isolate* isolate, const std::string& name, const std::string& src)
{
    v8::Local<v8::String> sourceCode = V8Helpers::JSValue(src);

    v8::ScriptOrigin scriptOrigin(isolate, V8Helpers::JSValue(name), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());

    v8::ScriptCompiler::Source source{ sourceCode, scriptOrigin };
    return v8::ScriptCompiler::CompileModule(isolate, &source);
}

static inline bool IsSystemModule(v8::Isolate* isolate, const std::string& name)
{
    return V8Module::Exists(isolate, name);
}

static inline v8::MaybeLocal<v8::Module> WrapModule(v8::Isolate* isolate, const std::deque<std::string>& _exportKeys, const std::string& name, bool exportAsDefault = false)
{
    bool hasDefault = false;
    std::stringstream src;

    src << "const _exports = __internal_get_exports('" << name << "');\n";

    for(auto& key : _exportKeys)
    {
        if(key == "default")
        {
            src << "export default _exports['" << key << "'];\n";
            hasDefault = true;
        }
        else
        {
            src << "export let " << key << " = _exports['" << key << "'];\n";
        }
    }

    if(!hasDefault && exportAsDefault) src << "export default _exports;";

    return CompileESM(isolate, name, src.str());
}

bool IImportHandler::IsValidModule(const std::string& name)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    if(V8Module::Exists(isolate, name)) return true;

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);
    if(resource) return true;

    return false;
}

// !!! Keep this in sync with the magic bytes in bytecode module
static const char bytecodeMagic[] = { 'A', 'L', 'T', 'B', 'C' };
bool IImportHandler::IsBytecodeModule(uint8_t* buffer, size_t size)
{
    if(size < sizeof(bytecodeMagic)) return false;
    if(memcmp(buffer, bytecodeMagic, sizeof(bytecodeMagic)) == 0) return true;
    else
        return false;
}

std::deque<std::string> IImportHandler::GetModuleKeys(const std::string& name)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::Local<v8::Context> context = isolate->GetEnteredOrMicrotaskContext();
    auto v8module = V8Module::All()[isolate].find(name);
    if(v8module != V8Module::All()[isolate].end())
    {
        auto _exports = v8module->second->GetExports(isolate, context);
        v8::Local<v8::Array> v8Keys = _exports->GetOwnPropertyNames(context).ToLocalChecked();

        std::deque<std::string> keys;

        for(uint32_t i = 0; i < v8Keys->Length(); ++i)
        {
            v8::Local<v8::String> v8Key = v8Keys->Get(context, i).ToLocalChecked().As<v8::String>();
            keys.push_back(*v8::String::Utf8Value(isolate, v8Key));
        }

        return keys;
    }

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);
    if(resource)
    {
        std::deque<std::string> keys;

        alt::MValueDict _exports = resource->GetExports();
        for(auto it = _exports->Begin(); it; it = _exports->Next()) keys.push_back(it->GetKey());

        return keys;
    }

    return {};
}

std::string IImportHandler::GetModulePath(v8::Local<v8::Module> moduleHandle)
{
    for(auto& md : modules)
    {
        if(md.second.mod == moduleHandle) return md.first;
    }

    return std::string{};
}

const IImportHandler::ModuleData IImportHandler::GetModuleData(const std::string& name)
{
    auto result = modules.find(name);
    if(result == modules.end()) return ModuleData{};
    return result->second;
}

v8::Local<v8::Module> IImportHandler::GetModuleFromPath(std::string modulePath)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    for(auto& md : modules)
    {
        if(md.first == modulePath) return md.second.mod.Get(isolate);
    }

    return v8::Local<v8::Module>{};
}

v8::MaybeLocal<v8::Value> IImportHandler::Require(const std::string& name)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto it = requiresMap.find(name);
    if(it != requiresMap.end()) return it->second.Get(isolate);

    auto v8module = V8Module::All()[isolate].find(name);
    if(v8module != V8Module::All()[isolate].end())
    {
        auto _exports = v8module->second->GetExports(isolate, isolate->GetEnteredOrMicrotaskContext());
        requiresMap.insert({ name, V8Helpers::CPersistent<v8::Value>{ isolate, _exports } });

        return _exports;
    }

    alt::IResource* resource = alt::ICore::Instance().GetResource(name);
    if(resource)
    {
        v8::Local<v8::Value> _exports = V8Helpers::MValueToV8(resource->GetExports());

        requiresMap.insert({ name, V8Helpers::CPersistent<v8::Value>{ isolate, _exports } });

        return _exports;
    }

    return v8::MaybeLocal<v8::Value>();
}

v8::MaybeLocal<v8::Module> IImportHandler::ResolveFile(const std::string& name, v8::Local<v8::Module> referrer, alt::IResource* resource)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    auto path = alt::ICore::Instance().Resolve(resource, name, GetModulePath(referrer));

    if(!path.pkg) return v8::MaybeLocal<v8::Module>();

    std::string fileName = path.fileName;
    if(!path.pkg->FileExists(fileName)) return v8::MaybeLocal<v8::Module>();
    std::string fullName = path.prefix + fileName;

    auto it = modules.find(fullName);

    if(it != modules.end()) return it->second.mod.Get(isolate);

    v8::MaybeLocal<v8::Module> maybeModule;

    V8Helpers::TryCatch(
      [&]
      {
          alt::IPackage::File* file = path.pkg->OpenFile(fileName);

          size_t fileSize = path.pkg->GetFileSize(file);
          uint8_t* byteBuffer = new uint8_t[fileSize];
          path.pkg->ReadFile(file, byteBuffer, fileSize);
          path.pkg->CloseFile(file);

          bool isBytecode = IsBytecodeModule(byteBuffer, fileSize);

          if(!isBytecode)
          {
              std::string src{ (char*)byteBuffer, fileSize };
              maybeModule = CompileESM(isolate, fullName, src);
          }
          else
          {
              maybeModule = ResolveBytecode(fullName, byteBuffer, fileSize);
          }
          delete byteBuffer;

          if(maybeModule.IsEmpty()) return false;

          v8::Local<v8::Module> _module = maybeModule.ToLocalChecked();

          modules.insert({ fullName, ModuleData{ V8Helpers::CPersistent<v8::Module>{ isolate, _module }, isBytecode } });

          return true;
      });

    if(maybeModule.IsEmpty())
    {
        modules.erase(fullName);
    }

    return maybeModule;
}

v8::MaybeLocal<v8::Module> IImportHandler::ResolveModule(const std::string& _name, v8::Local<v8::Module> referrer, alt::IResource* resource)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::MaybeLocal<v8::Module> maybeModule;

    std::string name = _name;

    if(name.starts_with("alt:")) name = name.substr(4);
    if(name == "alt-client") name = "alt";

    auto it = modules.find(name);
    if(it != modules.end())
    {
        maybeModule = it->second.mod.Get(isolate);
    }

    if(maybeModule.IsEmpty())
    {
        if(IsValidModule(name))
        {
            V8Helpers::TryCatch(
              [&]
              {
                  maybeModule = WrapModule(isolate, GetModuleKeys(name), name, IsSystemModule(isolate, name));

                  if(!maybeModule.IsEmpty())
                  {
                      v8::Local<v8::Module> _module = maybeModule.ToLocalChecked();
                      modules.insert({ name, ModuleData{ V8Helpers::CPersistent<v8::Module>{ isolate, _module }, false } });

                      return true;
                  }

                  Log::Info << __LINE__ << "maybeModule.IsEmpty()";
                  return false;
              });

            if(maybeModule.IsEmpty())
            {
                modules.erase(name);
                isolate->ThrowException(v8::Exception::ReferenceError(V8Helpers::JSValue(("Failed to load module: " + name))));
                return v8::MaybeLocal<v8::Module>{};
            }
        }
    }

    if(maybeModule.IsEmpty())
    {
        maybeModule = ResolveFile(name, referrer, resource);
    }

    if(maybeModule.IsEmpty())
    {
        modules.erase(name);
        isolate->ThrowException(v8::Exception::ReferenceError(V8Helpers::JSValue(("No such module: " + name))));
        return v8::MaybeLocal<v8::Module>{};
    }

    /*v8::Local<v8::Module> _module = maybeModule.ToLocalChecked();
    if (_module->GetStatus() != v8::Module::kEvaluated)
    {
            modules.erase(name);
            isolate->ThrowException(v8::Exception::ReferenceError(v8::String::NewFromUtf8(isolate, ("Failed to import: " + name).c_str())));
            return v8::MaybeLocal<v8::Module>{ };
    }*/

    return maybeModule;
}

v8::MaybeLocal<v8::Module> IImportHandler::ResolveCode(const std::string& name, const std::string& code, const V8Helpers::SourceLocation& location)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::MaybeLocal<v8::Module> maybeModule;
    std::stringstream nameStream;
    if(name.empty()) nameStream << "[module " << location.GetFileName() << ":" << location.GetLineNumber() << "]";
    else
        nameStream << name;
    maybeModule = CompileESM(isolate, nameStream.str(), code);

    return maybeModule;
}

v8::MaybeLocal<v8::Module> IImportHandler::ResolveBytecode(const std::string& name, uint8_t* buffer, size_t size)
{
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    // Copy source code size
    int sourceCodeSize = 0;
    memcpy(&sourceCodeSize, buffer + sizeof(bytecodeMagic), sizeof(int));
    // Copy bytecode buffer
    size_t bytecodeSize = size - sizeof(bytecodeMagic) - sizeof(int);
    uint8_t* bytecode = new uint8_t[bytecodeSize];
    memcpy(bytecode, buffer + sizeof(bytecodeMagic) + sizeof(int), bytecodeSize);

    v8::ScriptCompiler::CachedData* cachedData = new v8::ScriptCompiler::CachedData(bytecode, bytecodeSize, v8::ScriptCompiler::CachedData::BufferOwned);
    v8::ScriptOrigin origin(isolate, V8Helpers::JSValue(name), 0, 0, false, -1, v8::Local<v8::Value>(), false, false, true, v8::Local<v8::PrimitiveArray>());

    // Create source string
    std::string sourceString;
    sourceString.reserve(sourceCodeSize + 2);
    sourceString += "'";
    for(size_t i = 0; i < sourceCodeSize; i++)
    {
        sourceString += "a";
    }
    sourceString += "'";
    v8::ScriptCompiler::Source source{ V8Helpers::JSValue(sourceString), origin, cachedData };
    v8::MaybeLocal<v8::Module> module = v8::ScriptCompiler::CompileModule(isolate, &source, v8::ScriptCompiler::kConsumeCodeCache);
    if(cachedData->rejected)
    {
        Log::Error << "[V8] Trying to load invalid bytecode" << Log::Endl;
        return v8::MaybeLocal<v8::Module>();
    }
    else
        return module;
}
