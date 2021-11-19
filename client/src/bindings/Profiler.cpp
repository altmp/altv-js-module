#include "V8Helpers.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "v8-profiler.h"
#include <chrono>
#include <vector>
#include <unordered_map>

static void GetHeapStatistics(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    v8::HeapStatistics heapStats;
    isolate->GetHeapStatistics(&heapStats);
    V8_NEW_OBJECT(stats);
    V8_OBJECT_SET_UINT(stats, "heapSizeLimit", heapStats.heap_size_limit());
    V8_OBJECT_SET_UINT(stats, "totalHeapSize", heapStats.total_heap_size());
    V8_OBJECT_SET_UINT(stats, "usedHeapSize", heapStats.used_heap_size());
    V8_OBJECT_SET_UINT(stats, "mallocedMemory", heapStats.malloced_memory());
    V8_OBJECT_SET_UINT(stats, "peakMallocedMemory", heapStats.peak_malloced_memory());

    V8_RETURN(stats);
}

// Key = Node ID, Value = Timestamp
// We store a map of the timestamps here, so we can quickly
// access it when setting it while serializing the profiler node
// todo: There is probably some nicer way to do this
static std::unordered_map<unsigned int, int64_t> nodeMap;
static uint32_t profilerRunningCount = 0;
static void GetProfileNodeData(v8::Isolate* isolate, const v8::CpuProfileNode* node, v8::Local<v8::Object> result);

static void StartProfiling(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(0, 1);

    v8::Local<v8::String> name;
    if(info.Length() == 1)
    {
        V8_ARG_TO_STRING(1, profileName);
        name = V8::JSValue(profileName.CStr());
    }
    else
        name = v8::String::Empty(isolate);

    v8::CpuProfilingStatus status = CV8ScriptRuntime::Instance().GetProfiler()->StartProfiling(name, true);
    if(status == v8::CpuProfilingStatus::kStarted)
    {
        profilerRunningCount++;
        return;
    }
    else if(status == v8::CpuProfilingStatus::kAlreadyStarted)
        V8Helpers::Throw(isolate, "A profile with the given name is already running");
    else if(status == v8::CpuProfilingStatus::kErrorTooManyProfilers)
        V8Helpers::Throw(isolate, "There are already too many profilers running");
}

static void StopProfiling(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(0, 1);

    v8::Local<v8::String> name;
    if(info.Length() == 1)
    {
        V8_ARG_TO_STRING(1, profileName);
        name = V8::JSValue(profileName.CStr());
    }
    else
        name = v8::String::Empty(isolate);

    v8::CpuProfile* result = CV8ScriptRuntime::Instance().GetProfiler()->StopProfiling(name);
    V8_CHECK(result, "The specified profiler is not running");

    // Store the node map
    int sampleCount = result->GetSamplesCount();
    for(int i = 0; i < sampleCount; i++)
    {
        unsigned int nodeId = result->GetSample(i)->GetNodeId();
        if(nodeMap.count(nodeId) != 0) continue;
        nodeMap.insert({ nodeId, (result->GetSampleTimestamp(i) / 1000) });
    }

    // Set top level info about the profile
    V8_NEW_OBJECT(resultObj);
    V8_OBJECT_SET_INT(resultObj, "id", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    V8_OBJECT_SET_STRING(resultObj, "type", alt::String("cpu"));
    V8_OBJECT_SET_INT(resultObj, "start", result->GetStartTime() / 1000);
    V8_OBJECT_SET_INT(resultObj, "end", result->GetEndTime() / 1000);
    V8_OBJECT_SET_INT(resultObj, "samples", result->GetSamplesCount());

    V8_NEW_OBJECT(root);
    GetProfileNodeData(isolate, result->GetTopDownRoot(), root);
    resultObj->Set(ctx, V8::JSValue("root"), root);

    // Clear the nodemap to not cause a memory leak
    nodeMap.clear();
    result->Delete();

    profilerRunningCount--;

    V8_RETURN(resultObj);
}

static void SamplingIntervalSetter(v8::Local<v8::String>, v8::Local<v8::Value> value, const v8::PropertyCallbackInfo<void>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_TO_INT32(value, interval);

    V8_CHECK(profilerRunningCount == 0, "Can't set sampling interval while profiler is running");

    CV8ScriptRuntime::Instance().SetProfilerSamplingInterval(interval);
}

static void SamplingIntervalGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_INT(CV8ScriptRuntime::Instance().GetProfilerSamplingInterval());
}

static void ProfilesRunningGetter(v8::Local<v8::String>, const v8::PropertyCallbackInfo<v8::Value>& info)
{
    V8_RETURN_UINT(profilerRunningCount);
}

extern V8Class v8Profiler("Profiler", [](v8::Local<v8::FunctionTemplate> tpl) {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();

    V8::SetStaticAccessor(isolate, tpl, "heapStats", GetHeapStatistics);
    V8::SetStaticAccessor(isolate, tpl, "samplingInterval", SamplingIntervalGetter, SamplingIntervalSetter);
    V8::SetStaticAccessor(isolate, tpl, "profilesRunning", ProfilesRunningGetter);

    V8::SetStaticMethod(isolate, tpl, "startProfiling", StartProfiling);
    V8::SetStaticMethod(isolate, tpl, "stopProfiling", StopProfiling);
});

// *** CPU Profile Serialization

inline static const char* GetSourceTypeName(v8::CpuProfileNode::SourceType type)
{
    switch(type)
    {
        case v8::CpuProfileNode::SourceType::kScript: return "script";
        case v8::CpuProfileNode::SourceType::kBuiltin: return "builtins";
        case v8::CpuProfileNode::SourceType::kCallback: return "native-callback";
        case v8::CpuProfileNode::SourceType::kInternal: return "internal";
    }
    return "unknown";
}

static void GetProfileNodeData(v8::Isolate* isolate, const v8::CpuProfileNode* node, v8::Local<v8::Object> result)
{
    auto ctx = isolate->GetEnteredOrMicrotaskContext();

    // Node info
    result->Set(ctx, V8::JSValue("id"), V8::JSValue(node->GetNodeId()));

    v8::Local<v8::String> functionName;
    const char* name = node->GetFunctionNameStr();
    if(name == NULL || strlen(name) == 0) functionName = V8::JSValue("(anonymous function)");
    else
        functionName = V8::JSValue(name);
    result->Set(ctx, V8::JSValue("function"), functionName);

    v8::Local<v8::String> sourceName;
    const char* source = node->GetScriptResourceNameStr();
    if(source == NULL || strlen(source) == 0) sourceName = V8::JSValue("(unknown)");
    else
        sourceName = V8::JSValue(source);
    result->Set(ctx, V8::JSValue("source"), sourceName);

    result->Set(ctx, V8::JSValue("sourceType"), V8::JSValue(GetSourceTypeName(node->GetSourceType())));
    result->Set(ctx, V8::JSValue("line"), V8::JSValue(node->GetLineNumber()));

    v8::Local<v8::Value> bailoutReason;
    const char* reason = node->GetBailoutReason();
    if(reason == NULL || strlen(reason) == 0) bailoutReason = v8::Null(isolate);
    else
        bailoutReason = V8::JSValue(reason);
    result->Set(ctx, V8::JSValue("bailoutReason"), bailoutReason);

    result->Set(ctx, V8::JSValue("hitCount"), V8::JSValue(node->GetHitCount()));

    int64_t timestamp;
    if(nodeMap.count(node->GetNodeId()) == 0) timestamp = -1;
    else
        timestamp = nodeMap.at(node->GetNodeId());
    result->Set(ctx, V8::JSValue("timestamp"), V8::JSValue(timestamp));

    // Children
    {
        int childrenCount = node->GetChildrenCount();
        v8::Local<v8::Value> children;
        if(childrenCount != 0)
        {
            children = v8::Array::New(isolate, childrenCount);
            for(int i = 0; i < childrenCount; i++)
            {
                V8_NEW_OBJECT(child);
                GetProfileNodeData(isolate, node->GetChild(i), child);
                children.As<v8::Array>()->Set(ctx, i, child);
            }
        }
        else
            children = v8::Null(isolate);

        result->Set(ctx, V8::JSValue("children"), children);
    }

    // Line ticks
    {
        std::vector<v8::CpuProfileNode::LineTick> ticks(node->GetHitLineCount());
        auto ticksSize = ticks.size();
        v8::Local<v8::Value> val;
        if(node->GetLineTicks(&ticks[0], ticksSize))
        {
            val = v8::Array::New(isolate, ticksSize);
            for(size_t i = 0; i < ticksSize; i++)
            {
                auto tick = ticks[i];
                V8_NEW_OBJECT(tickObj);
                tickObj->Set(ctx, V8::JSValue("line"), V8::JSValue(tick.line));
                tickObj->Set(ctx, V8::JSValue("hitCount"), V8::JSValue(tick.hit_count));
                val.As<v8::Array>()->Set(ctx, i, tickObj);
            }
        }
        else
            val = v8::Null(isolate);
        result->Set(ctx, V8::JSValue("lineTicks"), val);
    }
}
