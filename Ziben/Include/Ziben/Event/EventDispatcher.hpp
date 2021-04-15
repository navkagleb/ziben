#pragma once

#include "Event.hpp"

namespace Ziben {

    template <typename T>
    concept EventChild = std::is_base_of_v<Event, T>;

    class EventDispatcher {
    public:
        template <EventChild T>
        using EventFunc = std::function<bool(T&)>;

        explicit EventDispatcher(Event& event);
        ~EventDispatcher() = default;

        template <EventChild T>
        bool Dispatch(const EventFunc<T>& eventFunc);

    private:
        Event& m_Event;

    }; // class EventDispatcher

    EventDispatcher::EventDispatcher(Event& event)
        : m_Event(event) {}

    template <EventChild T>
    bool EventDispatcher::Dispatch(const EventFunc<T>& eventFunc) {
        if (m_Event.GetEventType() == T::GetStaticEventType()) {
            m_Event.m_Handled = eventFunc(static_cast<T&>(m_Event));
            return true;
        }

        return false;
    }

} // namespace Ziben