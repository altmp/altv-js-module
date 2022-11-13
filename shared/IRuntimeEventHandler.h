#pragma once

#include "cpp-sdk/SDK.h"
#include <array>

class IRuntimeEventHandler
{
    static IRuntimeEventHandler*& _instance()
    {
        static IRuntimeEventHandler* instance = nullptr;
        return instance;
    }

    using EventType = alt::CEvent::Type;
    // All events the module uses, which need to be enabled and never disabled
    static constexpr std::array internalEvents = {
        EventType::CONNECTION_COMPLETE, EventType::DISCONNECT_EVENT, EventType::GAME_ENTITY_CREATE, EventType::GAME_ENTITY_DESTROY, EventType::RESOURCE_STOP
    };

    // Keeps track of the event handlers registered for all events
    std::unordered_map<alt::CEvent::Type, uint32_t> eventHandlersCount;

    static void SetInstance(IRuntimeEventHandler* handler)
    {
        _instance() = handler;
    }

public:
    constexpr static bool IsNeededEvent(alt::CEvent::Type type)
    {
        for(alt::CEvent::Type evType : internalEvents)
        {
            if(evType == type) return true;
        }
        return false;
    }

    void Start()
    {
        SetInstance(this);
        Reset();
    }

    void Reset()
    {
        // Enable all events the module needs
        for(EventType type : internalEvents)
        {
            alt::ICore::Instance().ToggleEvent(type, true);
        }
        eventHandlersCount.clear();
    }

    void EventHandlerAdded(alt::CEvent::Type type)
    {
        // If the event is needed, we don't need to keep track of the amount of handlers for it
        if(IsNeededEvent(type)) return;
        if(eventHandlersCount[type]++ == 0)
        {
            alt::ICore::Instance().ToggleEvent(type, true);
        }
    }
    void EventHandlerRemoved(alt::CEvent::Type type)
    {
        if(IsNeededEvent(type)) return;
        if(--eventHandlersCount[type] == 0)
        {
            // If there are no more handlers for this event, we don't need it anymore
            alt::ICore::Instance().ToggleEvent(type, false);
        }
    }

    static IRuntimeEventHandler& Instance()
    {
        return *_instance();
    }
};
