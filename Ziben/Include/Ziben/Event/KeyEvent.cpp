#include "KeyEvent.hpp"

namespace Ziben {

    KeyEvent::KeyEvent(KeyCode keyCode)
        : m_KeyCode(keyCode) {}

    KeyPressedEvent::KeyPressedEvent(KeyCode keyCode, int count)
        : KeyEvent(keyCode)
        , m_Count(count) {}

    std::string KeyPressedEvent::ToString() const {
        return GetName() + ": " + static_cast<char>(m_KeyCode) + "(" + std::to_string(m_KeyCode) + ", " + std::to_string(m_Count) + ")";
    }
    KeyReleasedEvent::KeyReleasedEvent(KeyCode keyCode)
        : KeyEvent(keyCode) {}

    std::string KeyReleasedEvent::ToString() const {
        return GetName() + ": " + static_cast<char>(m_KeyCode) + "(" + std::to_string(m_KeyCode) + ")";
    }

    KeyTypedEvent::KeyTypedEvent(KeyCode keyCode)
        : KeyEvent(keyCode) {}

    std::string KeyTypedEvent::ToString() const {
        return GetName() + ": " + static_cast<char>(m_KeyCode) + "(" + std::to_string(m_KeyCode) + ")";
    }

} // namespace Ziben