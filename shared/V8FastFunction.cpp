#include "V8FastFunction.h"

#ifdef ALT_CLIENT

V8FastFunction* V8FastFunction::Get(const std::string& name, const std::string& className)
{
    // There are probably better ways to do a unique identifier for this
    // but this is enough for our usecase
    auto it = All().find(GetIdentifier(name, className));
    if(it == All().end()) return nullptr;
    return it->second;
}

v8::Local<v8::FunctionTemplate> V8FastFunction::GetTemplate(v8::Isolate* isolate)
{
    auto it = tplMap.find(isolate);
    if(it != tplMap.end()) return it->second.Get(isolate);

    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(
      isolate, slowCallback, v8::Local<v8::Value>(), v8::Local<v8::Signature>(), 1, v8::ConstructorBehavior::kThrow, v8::SideEffectType::kHasSideEffect, &fastCallback);
    tplMap.insert({ isolate, v8::Persistent<v8::FunctionTemplate, v8::CopyablePersistentTraits<v8::FunctionTemplate>>(isolate, tpl) });
    return tpl;
}

void V8FastFunction::UnloadAll(v8::Isolate* isolate)
{
    for(auto [_, func] : All()) func->tplMap.erase(isolate);
}

#endif
