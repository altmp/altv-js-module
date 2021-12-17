#pragma once

#include "v8.h"
#include "libplatform/libplatform.h"

class CPlatform : public v8::Platform
{
    // We don't want to implement the whole V8 platform,
    // so we just use the default platform with our own overriden methods.
    std::unique_ptr<v8::Platform> defaultPlatform;

public:
    CPlatform();
    ~CPlatform() = default;

    // * Implementations from default platform
    int NumberOfWorkerThreads() override;
    std::shared_ptr<v8::TaskRunner> GetForegroundTaskRunner(v8::Isolate* isolate) override;
    void CallOnWorkerThread(std::unique_ptr<v8::Task> task) override;
    void CallDelayedOnWorkerThread(std::unique_ptr<v8::Task> task, double delay_in_seconds) override;
    bool IdleTasksEnabled(v8::Isolate* isolate) override;
    std::unique_ptr<v8::JobHandle> PostJob(v8::TaskPriority priority, std::unique_ptr<v8::JobTask> job_state) override;
    double MonotonicallyIncreasingTime() override;
    double CurrentClockTimeMillis() override;
    v8::TracingController* GetTracingController() override;
    StackTracePrinter GetStackTracePrinter() override;
    v8::PageAllocator* GetPageAllocator() override;

    // * Custom overwrites
    v8::ZoneBackingAllocator* GetZoneBackingAllocator() override;
    void DumpWithoutCrashing() override;
};
