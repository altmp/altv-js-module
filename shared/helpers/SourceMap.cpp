#include "SourceMap.h"
#include "./Convert.h"

#include <iostream>
#include <sstream>
#include <vector>

bool SourceMap::Parse(v8::Local<v8::Context> ctx, const std::string& payload)
{
    if(payload.size() == 0) return false;

    v8::Isolate* isolate = ctx->GetIsolate();

    // Check if the payload is a valid Source Map JSON string
    v8::MaybeLocal<v8::Value> maybeResult = v8::JSON::Parse(ctx, V8Helpers::JSValue(payload));
    if(maybeResult.IsEmpty()) return false;
    v8::Local<v8::Value> result = maybeResult.ToLocalChecked();
    if(!result->IsObject()) return false;
    v8::Local<v8::Object> sourceMap = result.As<v8::Object>();

    // Check if the source map is the correct version (we only support version 3)
    v8::MaybeLocal<v8::Value> maybeVersion = sourceMap->Get(ctx, V8Helpers::JSValue("version"));
    if(maybeVersion.IsEmpty()) return false;
    v8::Local<v8::Value> version = maybeVersion.ToLocalChecked();
    if(!version->IsNumber()) return false;
    if(version.As<v8::Uint32>()->Value() != 3) return false;

    // Extract the payload data
    if(!ParseFileName(ctx, sourceMap)) return false;
    if(!ParseSourceRoot(ctx, sourceMap)) return false;
    if(!ParseSources(ctx, sourceMap)) return false;
    if(!ParseSourcesContent(ctx, sourceMap)) return false;
    if(!ParseNames(ctx, sourceMap)) return false;
    if(!ParseMappings(ctx, sourceMap)) return false;

    return true;
}

bool SourceMap::ParseFileName(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap)
{
    v8::MaybeLocal<v8::Value> maybeFile = sourceMap->Get(ctx, V8Helpers::JSValue("file"));
    if(!maybeFile.IsEmpty())
    {
        v8::Local<v8::Value> file = maybeFile.ToLocalChecked();
        if(!file->IsString()) return false;
        fileName = *v8::String::Utf8Value(ctx->GetIsolate(), file.As<v8::String>());
    }
    return true;
}

bool SourceMap::ParseSourceRoot(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap)
{
    v8::MaybeLocal<v8::Value> maybeSourceRoot = sourceMap->Get(ctx, V8Helpers::JSValue("sourceRoot"));
    if(!maybeSourceRoot.IsEmpty())
    {
        v8::Local<v8::Value> sourceRootVal = maybeSourceRoot.ToLocalChecked();
        if(!sourceRootVal->IsString()) return false;
        sourceRoot = *v8::String::Utf8Value(ctx->GetIsolate(), sourceRootVal.As<v8::String>());
    }
    return true;
}

bool SourceMap::ParseSources(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap)
{
    v8::MaybeLocal<v8::Value> maybeSources = sourceMap->Get(ctx, V8Helpers::JSValue("sources"));
    if(!maybeSources.IsEmpty())
    {
        v8::Local<v8::Value> sourcesVal = maybeSources.ToLocalChecked();
        if(!sourcesVal->IsArray()) return false;

        v8::Local<v8::Array> sourcesArray = sourcesVal.As<v8::Array>();
        uint32_t len = sourcesArray->Length();
        v8::Isolate* isolate = ctx->GetIsolate();
        for(uint32_t i = 0; i < len; i++)
        {
            v8::Local<v8::Value> source = sourcesArray->Get(ctx, i).ToLocalChecked();
            if(!source->IsString()) return false;
            sources.push_back(*v8::String::Utf8Value(isolate, source.As<v8::String>()));
        }
    }
    return true;
}

bool SourceMap::ParseSourcesContent(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap)
{
    v8::MaybeLocal<v8::Value> maybeSourcesContent = sourceMap->Get(ctx, V8Helpers::JSValue("sourcesContent"));
    if(!maybeSourcesContent.IsEmpty())
    {
        v8::Local<v8::Value> sourcesContentVal = maybeSourcesContent.ToLocalChecked();
        if(!sourcesContentVal->IsArray()) return false;

        v8::Local<v8::Array> sourcesContentArray = sourcesContentVal.As<v8::Array>();
        uint32_t len = sourcesContentArray->Length();
        v8::Isolate* isolate = ctx->GetIsolate();
        for(uint32_t i = 0; i < len; i++)
        {
            v8::Local<v8::Value> sourceContent = sourcesContentArray->Get(ctx, i).ToLocalChecked();
            if(!sourceContent->IsString() && !sourceContent->IsNull()) return false;
            std::string sourceContentStr;
            if(sourceContent->IsNull()) sourceContentStr = "null";
            else
                sourceContentStr = *v8::String::Utf8Value(isolate, sourceContent.As<v8::String>());
            sourcesContent.push_back(sourceContentStr);
        }
    }
    return true;
}

bool SourceMap::ParseNames(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap)
{
    v8::MaybeLocal<v8::Value> maybeNames = sourceMap->Get(ctx, V8Helpers::JSValue("names"));
    if(!maybeNames.IsEmpty())
    {
        v8::Local<v8::Value> namesVal = maybeNames.ToLocalChecked();
        if(!namesVal->IsArray()) return false;

        v8::Local<v8::Array> namesArray = namesVal.As<v8::Array>();
        uint32_t len = namesArray->Length();
        v8::Isolate* isolate = ctx->GetIsolate();
        for(uint32_t i = 0; i < len; i++)
        {
            v8::Local<v8::Value> name = namesArray->Get(ctx, i).ToLocalChecked();
            if(!name->IsString()) return false;
            names.push_back(*v8::String::Utf8Value(isolate, name.As<v8::String>()));
        }
    }
    return true;
}

// Ripped from SO https://stackoverflow.com/a/46931770/10824299
std::vector<std::string> SplitString(const std::string& str, char delimeter)
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    std::string item;

    while(std::getline(ss, item, delimeter)) result.push_back(item);

    return result;
}

bool SourceMap::ParseMappings(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap)
{
    v8::MaybeLocal<v8::Value> maybeMappings = sourceMap->Get(ctx, V8Helpers::JSValue("mappings"));
    if(!maybeMappings.IsEmpty())
    {
        v8::Local<v8::Value> mappingsVal = maybeMappings.ToLocalChecked();
        if(!mappingsVal->IsString()) return false;
        std::string mappingsStr = *v8::String::Utf8Value(ctx->GetIsolate(), mappingsVal.As<v8::String>());

        std::vector<std::string> groups = SplitString(mappingsStr, ';');
        for(std::string& group : groups)
        {
            std::vector<std::string> parts = SplitString(group, ',');
            if(parts.size() == 0)
            {
                // todo: some special handling for this
            }
            // todo: implement
            Mapping mapping;
            mappings.push_back(mapping);
        }
    }
    return true;
}
