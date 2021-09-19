#pragma once

#include "v8-inspector.h"

namespace alt
{
    class CV8InspectorChannel : public v8_inspector::V8Inspector::Channel
    {
        void sendResponse(int callId, std::unique_ptr<v8_inspector::StringBuffer> message)
        {
            Log::Debug << __FUNCTION__ << callId << message->string().characters8();
        }

        void sendNotification(std::unique_ptr<v8_inspector::StringBuffer> message)
        {
            Log::Debug << __FUNCTION__ << message->string().characters8();
        }

        void flushProtocolNotifications()
        {
            Log::Debug << __FUNCTION__;
        }
    };
}  // namespace alt
