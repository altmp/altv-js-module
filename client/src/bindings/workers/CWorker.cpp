#include "CWorker.h"
#include <functional>

CWorker::CWorker(const std::string& filePath) : filePath(filePath)
{
    Start();
}

void CWorker::Destroy()
{
    shouldTerminate = true;
}

void CWorker::Start()
{
    thread = std::thread(std::bind(&CWorker::Thread, this));
    thread.detach();
}

void CWorker::Thread()
{
    SetupIsolate();

    while(true)
    {
        // Sleep for a short while to not overload the thread
        std::this_thread::sleep_for(std::chrono::milliseconds(3));
        if(!EventLoop()) break;
    }

    DestroyIsolate();
    delete this;  // ! IMPORTANT TO DO THIS LAST !
}

bool CWorker::EventLoop()
{
    if(shouldTerminate) return false;
    // todo: event loop

    return true;
}

void CWorker::SetupIsolate()
{
    // todo: create isolate and stuff
}

void CWorker::DestroyIsolate()
{
    // todo: clean up isolate and stuff
}

void CWorker::SetupGlobals(v8::Local<v8::Object> global)
{
    // todo: set up global functions for the isolate
}
