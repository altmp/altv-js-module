#pragma once
#include "v8.h"
#include "V8Helpers.h"

#include <string>
#include <thread>

class CWorker
{
    std::string filePath;
    std::thread thread;
    bool shouldTerminate = false;

    void Start();
    void Thread();

    v8::Isolate* isolate = nullptr;
    V8::CPersistent<v8::Context> context;

    bool EventLoop();

    void SetupIsolate();
    void DestroyIsolate();
    void SetupGlobals(v8::Local<v8::Object> global);

public:
    CWorker(const std::string& filePath);
    ~CWorker() = default;

    void Destroy();

    std::string GetFilePath()
    {
        return filePath;
    }
};
