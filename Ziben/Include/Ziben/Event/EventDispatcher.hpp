#pragma once

#include "Event.hpp"

namespace Ziben {

    template <typename T>
    concept EventChildConcept = std::is_base_of_v<Event, T>;

    class EventDispatcher {
    public:
        template <EventChildConcept T>
        using EventFunc = std::function<bool(T&)>;

        explicit EventDispatcher(Event& event);
        ~EventDispatcher() = default;

        template <EventChildConcept T>
        bool Dispatch(const EventFunc<T>& eventFunc);

    private:
        Event& m_Event;

    }; // class EventDispatcher

    template <EventChildConcept T>
    bool EventDispatcher::Dispatch(const EventFunc<T>& eventFunc) {
        if (m_Event.GetEventType() == T::GetStaticEventType()) {
            m_Event.m_IsHandled = eventFunc(static_cast<T&>(m_Event));
            return true;
        }

        return false;
    }

} // namespace Ziben