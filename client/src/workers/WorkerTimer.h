#pragma once

#include "V8Helpers.h"
#include "CWorker.h"

class WorkerTimer
{
public:
    WorkerTimer(CWorker* _worker,
                v8::Isolate* _isolate,
                v8::Local<v8::Context> _context,
                int64_t curTime,
                v8::Local<v8::Function> _callback,
                uint32_t _interval,
                bool _once,
                V8Helpers::SourceLocation&& _location)
        : worker(_worker), isolate(_isolate), context(_isolate, _context), lastRun(curTime), callback(_isolate, _callback), interval(_interval), once(_once), location(std::move(_location))
    {
    }

    bool Update(int64_t curTime)
    {
        if(curTime - lastRun >= interval)
        {
            auto result = CWorker::TryCatch([&] { v8::MaybeLocal<v8::Value> result = callback.Get(isolate)->CallAsFunction(context.Get(isolate), v8::Undefined(isolate), 0, nullptr); });
            if(!result.empty()) worker->EmitError(result);

            lastRun = curTime;

            return !once;
        }

        return true;
    }

    const V8Helpers::SourceLocation& GetLocation() const
    {
        return location;
    }
    int64_t GetInterval()
    {
        return interval;
    }
    bool IsOnce()
    {
        return once;
    }

private:
    CWorker* worker;
    v8::Isolate* isolate;
    V8Helpers::CPersistent<v8::Context> context;
    V8Helpers::CPersistent<v8::Function> callback;
    int64_t interval;
    int64_t lastRun = 0;
    bool once;
    V8Helpers::SourceLocation location;
};
