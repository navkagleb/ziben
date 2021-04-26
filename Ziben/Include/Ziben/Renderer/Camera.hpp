#pragma once

#include <glm/glm.hpp>

namespace Ziben {

    class Camera {
    public:
        [[nodiscard]] virtual const glm::mat4& GetProjectionMatrix() const     = 0;
        [[nodiscard]] virtual const glm::mat4& GetViewMatrix() const           = 0;
        [[nodiscard]] virtual const glm::mat4& GetViewProjectionMatrix() const = 0;
        [[nodiscard]] virtual const glm::vec3& GetPosition() const             = 0;

    }; // class Camera

} // namespace Ziben