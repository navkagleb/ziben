#include "WindowEvent.hpp"

namespace Ziben {

    WindowResizeEvent::WindowResizeEvent(int width, int height)
        : m_Width(width)
        , m_Height(height) {}

    std::string WindowResizeEvent::ToString() const {
        return GetName() + ": " + std::to_string(m_Width) + ", " + std::to_string(m_Height);
    }

} // namespace Ziben