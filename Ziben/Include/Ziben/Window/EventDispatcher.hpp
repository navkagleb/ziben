#pragma once

#include "Event.hpp"

namespace Ziben {

#define ZIBEN_BIND_EVENT_FUNC(func)                                                                \
    [this](auto&&... args) -> decltype(auto) {                                                     \
        return this->func(std::forward<decltype(args)>(args)...);                                  \
    }

    template <typename T>
    concept EventChildConcept = std::is_base_of_v<Event, T>;

    class EventDispatcher {
    public:
        explicit EventDispatcher(Event& event);
        ~EventDispatcher() = default;

        template <EventChildConcept T, typename Func>
        bool Dispatch(const Func& eventFunc);

    private:
        Event& m_Event;

    }; // class EventDispatcher

    template <EventChildConcept T, typename Func>
    bool EventDispatcher::Dispatch(const Func& eventFunc) {
        if (m_Event.GetEventType() == T::GetStaticEventType()) {
            m_Event.m_IsHandled = eventFunc(static_cast<T&>(m_Event));
            return true;
        }

        return false;
    }

} // namespace Ziben