#include "V8Helpers.h"
#include "V8Class.h"
#include "../CV8ScriptRuntime.h"
#include "../CV8Resource.h"
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
    V8_OBJECT_SET_UINT(stats, "nativeContexts", heapStats.number_of_native_contexts());
    V8_OBJECT_SET_UINT(stats, "detachedContexts", heapStats.number_of_detached_contexts());
    V8_OBJECT_SET_UINT(stats, "totalGlobalHandleSize", heapStats.total_global_handles_size());
    V8_OBJECT_SET_UINT(stats, "usedGlobalHandleSize", heapStats.used_global_handles_size());

    V8_RETURN(stats);
}

// A simple wrapper that just executes the callback with the result
class MeasureMemoryDelegate : public v8::MeasureMemoryDelegate
{
public:
    using Callback = std::function<void(const std::vector<std::pair<v8::Local<v8::Context>, size_t>>&, size_t)>;

    MeasureMemoryDelegate(Callback&& _callback) : callback(_callback){};

    bool ShouldMeasure(v8::Local<v8::Context> context) override
    {
        return true;
    }

    void MeasurementComplete(const std::vector<std::pair<v8::Local<v8::Context>, size_t>>& context_sizes_in_bytes, size_t unattributed_size_in_bytes)
    {
        callback(context_sizes_in_bytes, unattributed_size_in_bytes);
    }

private:
    Callback callback;
};

static void GetMemoryProfile(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();

    static std::list<V8Helpers::CPersistent<v8::Promise::Resolver>> promises;
    auto& persistent = promises.emplace_back(V8Helpers::CPersistent<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

    std::unique_ptr<MeasureMemoryDelegate> delegate = std::make_unique<MeasureMemoryDelegate>(
      [isolate, persistent](const std::vector<std::pair<v8::Local<v8::Context>, size_t>>& result, size_t externalBytes)
      {
          v8::Locker locker(isolate);
          v8::Isolate::Scope isolateScope(isolate);
          v8::HandleScope handleScope(isolate);

          v8::Local<v8::Promise::Resolver> resolver = v8::Local<v8::Promise::Resolver>::New(isolate, persistent);
          v8::Local<v8::Context> ctx = resolver->GetCreationContext().ToLocalChecked();
          v8::Context::Scope ctxScope(ctx);

          V8_NEW_OBJECT(resultObj);
          v8::Local<v8::Array> results = v8::Array::New(isolate, result.size());
          for(size_t i = 0; i < result.size(); i++)
          {
              const std::pair<v8::Local<v8::Context>, size_t>& pair = result[i];
              V8_NEW_OBJECT(obj);
              V8_OBJECT_SET_STRING(obj, "resource", V8ResourceImpl::Get(pair.first)->GetResource()->GetName());
              V8_OBJECT_SET_UINT(obj, "size", pair.second);
              results->Set(ctx, i, obj);
          }
          resultObj->Set(ctx, V8Helpers::JSValue("results"), results);
          resultObj->Set(ctx, V8Helpers::JSValue("externalBytes"), V8Helpers::JSValue((uint32_t)externalBytes));

          resolver->Resolve(ctx, resultObj);

          promises.remove(persistent);
      });
    isolate->MeasureMemory(std::move(delegate), v8::MeasureMemoryExecution::kDefault);

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

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
        name = V8Helpers::JSValue(profileName);
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

void FlattenNodesTree(const v8::CpuProfileNode* node,
                      std::vector<const v8::CpuProfileNode*>* nodes) {
    nodes->emplace_back(node);
    const int childrenCount = node->GetChildrenCount();
    for (int i = 0; i < childrenCount; i++)
        FlattenNodesTree(node->GetChild(i), nodes);
}

static void StopProfiling(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    V8_GET_ISOLATE_CONTEXT();
    V8_CHECK_ARGS_LEN2(0, 1);

    v8::Local<v8::String> name;
    if(info.Length() == 1)
    {
        V8_ARG_TO_STRING(1, profileName);
        name = V8Helpers::JSValue(profileName);
    }
    else
        name = v8::String::Empty(isolate);

    v8::CpuProfile* result = CV8ScriptRuntime::Instance().GetProfiler()->StopProfiling(name);
    V8_CHECK(result, "The specified profiler is not running");

    // Set top level info about the profile
    V8_NEW_OBJECT(resultObj);
    // V8_OBJECT_SET_INT(resultObj, "id", std::chrono::duration_cast<std::chrono::seconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());

    std::vector<const v8::CpuProfileNode*> nodes;
    FlattenNodesTree(
        result->GetTopDownRoot(),
        &nodes);

    v8::Local<v8::Value> nodesObj = v8::Array::New(isolate, nodes.size());

    for (size_t i = 0; i < nodes.size(); i++) {
        V8_NEW_OBJECT(nodeObj);
        GetProfileNodeData(isolate, nodes.at(i), nodeObj);
        nodesObj.As<v8::Array>()->Set(ctx, i, nodeObj);
    }

    resultObj->Set(ctx, V8Helpers::JSValue("nodes"), nodesObj);

    // V8_OBJECT_SET_RAW_STRING(resultObj, "type", "cpu");
    V8_OBJECT_SET_INT(resultObj, "startTime", result->GetStartTime());
    V8_OBJECT_SET_INT(resultObj, "endTime", result->GetEndTime());

    auto samplesCount = result->GetSamplesCount();
    if (samplesCount) {
        v8::Local<v8::Value> samples = v8::Array::New(isolate, samplesCount);
        for (int i = 0; i < samplesCount; ++i)
        {
            samples.As<v8::Array>()->Set(ctx, i, V8Helpers::JSValue(result->GetSample(i)->GetNodeId()));
        }
        resultObj->Set(ctx, V8Helpers::JSValue("samples"), samples);

        v8::Local<v8::Value> timeDeltas = v8::Array::New(isolate, samplesCount);
        auto lastTime = result->GetStartTime();
        for (int i = 0; i < samplesCount; ++i)
        {
            auto ts = result->GetSampleTimestamp(i);
            timeDeltas.As<v8::Array>()->Set(ctx, i, V8Helpers::JSValue(static_cast<int>(ts - lastTime)));
            lastTime = ts;
        }
        resultObj->Set(ctx, V8Helpers::JSValue("timeDeltas"), timeDeltas);
    }

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

class StringOutputStream : public v8::OutputStream
{
    std::stringstream stream;
    std::function<void(const std::string&)> callback;

public:
    StringOutputStream(std::function<void(const std::string&)>&& _callback) : callback(_callback) {}

    virtual void EndOfStream() override
    {
        std::string str = stream.str();
        callback(str);
        delete this;
    }
    virtual WriteResult WriteAsciiChunk(char* data, int size) override
    {
        stream << data;
        return WriteResult::kContinue;
    }
};

static void TakeHeapSnapshot(const v8::FunctionCallbackInfo<v8::Value>& info)
{
    static std::list<v8::Global<v8::Promise::Resolver>> promises;
    V8_GET_ISOLATE_CONTEXT_RESOURCE();
    V8_CHECK_ARGS_LEN2(0, 1);

    v8::HeapProfiler* profiler = isolate->GetHeapProfiler();
    const v8::HeapSnapshot* snapshot = profiler->TakeHeapSnapshot();

    auto& persistent = promises.emplace_back(v8::Global<v8::Promise::Resolver>(isolate, v8::Promise::Resolver::New(ctx).ToLocalChecked()));

    auto callback = [&persistent, resource](const std::string& str)
    {
        resource->RunOnNextTick(
          [&persistent, resource, jsonStr = str]()
          {
              if(resource->GetResource()->IsStarted()) persistent.Get(resource->GetIsolate())->Resolve(resource->GetContext(), V8Helpers::JSValue(jsonStr));
              promises.remove(persistent);
          });
    };
    snapshot->Serialize(new StringOutputStream(callback));

    V8_RETURN(persistent.Get(isolate)->GetPromise());
}

extern V8Class v8Profiler("Profiler",
                          [](v8::Local<v8::FunctionTemplate> tpl)
                          {
                              v8::Isolate* isolate = v8::Isolate::GetCurrent();

                              V8Helpers::SetStaticAccessor(isolate, tpl, "heapStats", GetHeapStatistics);
                              V8Helpers::SetStaticAccessor(isolate, tpl, "samplingInterval", SamplingIntervalGetter, SamplingIntervalSetter);
                              V8Helpers::SetStaticAccessor(isolate, tpl, "profilesRunning", ProfilesRunningGetter);

                              V8Helpers::SetStaticMethod(isolate, tpl, "startProfiling", StartProfiling);
                              V8Helpers::SetStaticMethod(isolate, tpl, "stopProfiling", StopProfiling);

                              V8Helpers::SetStaticMethod(isolate, tpl, "getMemoryProfile", GetMemoryProfile);

                              V8Helpers::SetStaticMethod(isolate, tpl, "takeHeapSnapshot", TakeHeapSnapshot);
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
    result->Set(ctx, V8Helpers::JSValue("id"), V8Helpers::JSValue(node->GetNodeId()));

    result->Set(ctx, V8Helpers::JSValue("hitCount"), V8Helpers::JSValue(node->GetHitCount()));

    // Call frame
    {
        V8_NEW_OBJECT(callFrame);

        v8::Local<v8::String> functionName;
        const char* name = node->GetFunctionNameStr();
        if(name == NULL || strlen(name) == 0) functionName = V8Helpers::JSValue("(anonymous function)");
        else
            functionName = V8Helpers::JSValue(name);
        callFrame->Set(ctx, V8Helpers::JSValue("functionName"), functionName);

        callFrame->Set(ctx, V8Helpers::JSValue("lineNumber"), V8Helpers::JSValue(node->GetLineNumber() - 1));
        callFrame->Set(ctx, V8Helpers::JSValue("columnNumber"), V8Helpers::JSValue(node->GetColumnNumber() - 1));
        callFrame->Set(ctx, V8Helpers::JSValue("scriptId"), V8Helpers::JSValue(node->GetScriptId()));

        v8::Local<v8::String> sourceName;
        const char* source = node->GetScriptResourceNameStr();
        if(source == NULL || strlen(source) == 0) sourceName = V8Helpers::JSValue("(unknown)");
        else
            sourceName = V8Helpers::JSValue(source);
        callFrame->Set(ctx, V8Helpers::JSValue("url"), sourceName);

        result->Set(ctx, V8Helpers::JSValue("callFrame"), callFrame);
    }

    // Children
    auto childrenCount = node->GetChildrenCount();
    if (childrenCount) {
        v8::Local<v8::Value> children = v8::Array::New(isolate, childrenCount);
        for (int i = 0; i < childrenCount; ++i)
        {
            children.As<v8::Array>()->Set(ctx, i, V8Helpers::JSValue(node->GetChild(i)->GetNodeId()));
        }

        result->Set(ctx, V8Helpers::JSValue("children"), children);
    }

    const char* reason = node->GetBailoutReason();
    if(reason != NULL && strlen(reason) != 0 && strcmp(reason, "no reason"))
    {
        result->Set(ctx, V8Helpers::JSValue("deoptReason"), V8Helpers::JSValue(reason));
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
                tickObj->Set(ctx, V8Helpers::JSValue("line"), V8Helpers::JSValue(tick.line));
                tickObj->Set(ctx, V8Helpers::JSValue("ticks"), V8Helpers::JSValue(tick.hit_count));
                val.As<v8::Array>()->Set(ctx, i, tickObj);
            }
            result->Set(ctx, V8Helpers::JSValue("positionTicks"), val);
        }
    }
}
