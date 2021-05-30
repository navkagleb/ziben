#include "Camera.hpp"

namespace Ziben {

    Camera::Camera(const glm::mat4& projectionMatrix)
        : m_ProjectionMatrix(projectionMatrix) {}

} // namespace Ziben