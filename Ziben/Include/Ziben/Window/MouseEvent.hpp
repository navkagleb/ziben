#pragma once

#include "Event.hpp"
#include "ButtonCodes.hpp"

namespace Ziben {

    class MouseMovedEvent : public Event {
    public:
        MouseMovedEvent(int x, int y);
        ~MouseMovedEvent() override = default;

        [[nodiscard]] inline int GetX() const { return m_X; }
        [[nodiscard]] inline int GetY() const { return m_Y; }
        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(MouseMovedEvent)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

    private:
        int m_X;
        int m_Y;

    }; // class MouseMovedEvent

    class MouseScrolledEvent : public Event {
    public:
        MouseScrolledEvent(int offsetX, int offsetY);
        ~MouseScrolledEvent() override = default;

        [[nodiscard]] inline int GetOffsetX() const { return m_OffsetX; }
        [[nodiscard]] inline int GetOffsetY() const { return m_OffsetY; }
        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(MouseScrolledEvent)
        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)

    private:
        int m_OffsetX;
        int m_OffsetY;

    }; // MouseScrolledEvent

    class MouseButtonEvent : public Event {
    public:
        explicit MouseButtonEvent(ButtonCode buttonCode);
        ~MouseButtonEvent() override = default;

        [[nodiscard]] inline int GetButtonCode() const { return m_ButtonCode; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryMouseButton)

    protected:
        int m_ButtonCode;

    }; // class MouseButtonEvent

    class MouseButtonPressedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonPressedEvent(ButtonCode buttonCode);
        ~MouseButtonPressedEvent() override = default;

        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(MouseButtonPressedEvent)

    }; // MouseButtonPressedEvent

    class MouseButtonReleasedEvent : public MouseButtonEvent {
    public:
        explicit MouseButtonReleasedEvent(ButtonCode buttonCode);
        ~MouseButtonReleasedEvent() override = default;

        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(MouseButtonReleasedEvent);

    }; // class MouseButtonReleasedEvent

} // namespace Ziben