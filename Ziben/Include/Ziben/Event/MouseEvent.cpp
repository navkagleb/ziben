#include "MouseEvent.hpp"

namespace Ziben {

    MouseMovedEvent::MouseMovedEvent(int x, int y)
        : m_X(x)
        , m_Y(y) {}

    std::string MouseMovedEvent::ToString() const {
        return GetName() + ": " + std::to_string(m_X) + ", " + std::to_string(m_Y);
    }

    MouseScrolledEvent::MouseScrolledEvent(int offsetX, int offsetY)
        : m_OffsetX(offsetX)
        , m_OffsetY(offsetY) {}

    std::string MouseScrolledEvent::ToString() const {
        return GetName() + ": " + std::to_string(m_OffsetX) + ", " + std::to_string(m_OffsetY);
    }

    MouseButtonEvent::MouseButtonEvent(ButtonCode buttonCode)
        : m_ButtonCode(buttonCode) {}

    MouseButtonPressedEvent::MouseButtonPressedEvent(ButtonCode buttonCode)
        : MouseButtonEvent(buttonCode) {}

    std::string MouseButtonPressedEvent::ToString() const {
        return GetName() + ": " + std::to_string(m_ButtonCode);
    }

    MouseButtonReleasedEvent::MouseButtonReleasedEvent(ButtonCode buttonCode)
        : MouseButtonEvent(buttonCode) {}

    std::string MouseButtonReleasedEvent::ToString() const {
        return GetName() + ": " + std::to_string(m_ButtonCode);
    }

} // namespace Ziben