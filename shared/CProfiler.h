#pragma once

#include <unordered_map>
#include <vector>
#include <chrono>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip>

#include "Log.h"

class CProfiler
{
    using Duration = std::chrono::high_resolution_clock::duration;
    using TimePoint = std::chrono::high_resolution_clock::time_point;

    bool enabled = false;
    std::unordered_multimap<std::string, Duration> samples;
    std::unordered_map<std::string, TimePoint> runningSamples;

    void StartSample(const std::string& name)
    {
        if(!enabled) return;
        runningSamples.insert({ name, std::chrono::high_resolution_clock::now() });
    }
    void EndSample(const std::string& name, bool skipLog)
    {
        if(!enabled) return;
        auto it = runningSamples.find(name);
        if(it == runningSamples.end()) return;
        Duration duration = std::chrono::high_resolution_clock::now() - it->second;
        samples.insert({ name, duration });
        runningSamples.erase(it);

        if(!skipLog)
        {
            float ms = duration.count() / 1000000.f;
            std::string color = ms > 50 ? "~lr~" : ms > 20 ? "~ly~" : "~lg~";
            Log::Colored << "[Profiler] ~lc~" << name << "~w~ took: " << color << ms << "ms" << Log::Endl;
        }
    }

public:
    void Dump()
    {
        if(!enabled) return;

        int64_t t = std::time(nullptr);
        tm time = *std::localtime(&t);
        std::ostringstream stream;
        stream << std::put_time(&time, "%d-%m-%Y %H-%M-%S");
        std::string fileName = stream.str() + ".samples";

        std::ofstream file(fileName);
        if(!file.good())
        {
            Log::Error << "[Profiler] Failed to dump samples" << Log::Endl;
            file.close();
            return;
        }

        // todo: write all samples to file

        file.close();
        Log::Colored << "[Profiler] Dumped samples to ~lc~" << fileName << Log::Endl;
    }

    bool IsEnabled() const
    {
        return enabled;
    }
    void SetIsEnabled(bool state)
    {
        enabled = state;
    }

    static CProfiler& Instance()
    {
        static CProfiler instance;
        return instance;
    }

    class Sample
    {
        std::string name;
        bool skipLog;

    public:
        Sample(const std::string& _name, bool _skipLog = false) : name(_name), skipLog(_skipLog)
        {
            Instance().StartSample(name);
        }
        ~Sample()
        {
            Instance().EndSample(name, skipLog);
        }
    };
};
