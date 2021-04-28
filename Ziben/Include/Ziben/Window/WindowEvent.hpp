#pragma once

#include "Event.hpp"

namespace Ziben {

    class WindowResizedEvent : public Event {
    public:
        WindowResizedEvent(int width, int height);
        ~WindowResizedEvent() override = default;

        [[nodiscard]] inline int GetWidth() const { return m_Width; }
        [[nodiscard]] inline int GetHeight() const { return m_Height; }
        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(WindowResizedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    private:
        int m_Width;
        int m_Height;

    }; // class WindowResizedEvent

    class WindowClosedEvent : public Event {
    public:
        WindowClosedEvent() = default;
        ~WindowClosedEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowClosedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)


    }; // class WindowCloseEvent

    class WindowMaximizedEvent : public Event {
    public:
        WindowMaximizedEvent() = default;
        ~WindowMaximizedEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowMaximizedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; // class WindowMaximizedEvent

    class WindowMinimizedEvent : public Event {
    public:
        WindowMinimizedEvent() = default;
        ~WindowMinimizedEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowMinimizedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; // class WindowMinimizedEvent

    class WindowFocusedEvent : public Event {
    public:
        WindowFocusedEvent() = default;
        ~WindowFocusedEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowFocusedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; // class WindowFocusedEvent

    class WindowUnfocusedEvent : public Event {
    public:
        WindowUnfocusedEvent() = default;
        ~WindowUnfocusedEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowUnfocusedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; // class WindowUnfocusedEvent

    class WindowCursorEnteredEvent : public Event {
    public:
        WindowCursorEnteredEvent() = default;
        ~WindowCursorEnteredEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowCursorEnteredEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; // class WindowUnfocusedEvent

    class WindowCursorLeftEvent : public Event {
    public:
        WindowCursorLeftEvent() = default;
        ~WindowCursorLeftEvent() override = default;

        [[nodiscard]] inline std::string ToString() const override { return GetName(); }

        EVENT_CLASS_TYPE(WindowCursorLeftEvent)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

    }; // class WindowUnfocusedEvent

} // namespace Ziben