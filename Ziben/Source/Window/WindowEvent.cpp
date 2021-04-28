#include "WindowEvent.hpp"

namespace Ziben {

    WindowResizedEvent::WindowResizedEvent(int width, int height)
        : m_Width(width)
        , m_Height(height) {}

    std::string WindowResizedEvent::ToString() const {
        return GetName() + ": " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
    }

} // namespace Ziben