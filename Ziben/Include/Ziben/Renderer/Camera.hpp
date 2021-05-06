#pragma once

#include <glm/glm.hpp>

namespace Ziben {

    class Camera {
    public:
        explicit Camera(const glm::mat4& projectionMatrix = glm::mat4(1.0f));
        virtual ~Camera() = default;

    public:
        [[nodiscard]] virtual const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

    protected:
        glm::mat4 m_ProjectionMatrix;

    }; // class Camera

} // namespace Ziben