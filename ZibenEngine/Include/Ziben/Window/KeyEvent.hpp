#pragma once

#include "Event.hpp"
#include "KeyCodes.hpp"

namespace Ziben {

    class KeyEvent : public Event {
    public:
        ~KeyEvent() override = default;

        [[nodiscard]] inline int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryKeyboard)

    protected:
        explicit KeyEvent(KeyCode keyCode);

    protected:
        KeyCode m_KeyCode;

    }; // class KeyCode

    class KeyPressedEvent : public KeyEvent {
    public:
        KeyPressedEvent(KeyCode keyCode, int count);
        ~KeyPressedEvent() override = default;

        [[nodiscard]] inline int GetCount() const { return m_Count; }
        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(KeyPressedEvent)

    private:
        int m_Count;

    }; // class KeyPressedEvent

    class KeyReleasedEvent : public KeyEvent {
    public:
        explicit KeyReleasedEvent(KeyCode keyCode);
        ~KeyReleasedEvent() override = default;

        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(KeyReleasedEvent)

    }; // class KeyReleasedEvent

    class KeyTypedEvent : public KeyEvent {
    public:
        explicit KeyTypedEvent(KeyCode keyCode);
        ~KeyTypedEvent() override = default;

        [[nodiscard]] std::string ToString() const override;

        EVENT_CLASS_TYPE(KeyTypedEvent);

    }; // class KeyTypedEvent

} // namespace Ziben