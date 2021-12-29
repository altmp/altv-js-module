#pragma once

#include "v8.h"

#include <vector>

// A JavaScript source map, conformant to the Source Map Standard Version 3
class SourceMap
{
public:
    struct Mapping
    {
        int32_t generatedColumn = -1;
        int32_t sourceListIndex = -1;
        int32_t startingLine = -1;
        int32_t startingColumn = -1;
        int32_t namesListIndex = -1;

        Mapping(int32_t generatedColumn, int32_t sourceListIndex, int32_t startingLine, int32_t startingColumn, int32_t namesListIndex)
            : generatedColumn(generatedColumn), sourceListIndex(sourceListIndex), startingLine(startingLine), startingColumn(startingColumn), namesListIndex(namesListIndex)
        {
        }
        Mapping() = default;
    };

    bool IsValid()
    {
        return valid;
    }

    // Parses the given payload, if the parsing failed .IsValid() of the returned source map will return false
    static SourceMap FromPayload(v8::Local<v8::Context> ctx, const std::string& payload)
    {
        SourceMap map;
        map.valid = map.Parse(ctx, payload);
        return map;
    }

private:
    bool valid = false;
    std::string fileName;
    std::string sourceRoot;
    std::vector<std::string> sources;
    std::vector<std::string> sourcesContent;
    std::vector<std::string> names;
    std::vector<Mapping> mappings;

    bool Parse(v8::Local<v8::Context> ctx, const std::string& payload);

    bool ParseFileName(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap);
    bool ParseSourceRoot(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap);
    bool ParseSources(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap);
    bool ParseSourcesContent(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap);
    bool ParseNames(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap);
    bool ParseMappings(v8::Local<v8::Context> ctx, v8::Local<v8::Object> sourceMap);

    SourceMap() = default;
};
