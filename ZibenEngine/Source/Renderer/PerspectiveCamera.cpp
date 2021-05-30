#include "PerspectiveCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ziben {

    PerspectiveCamera::PerspectiveCamera(float fov, float aspect, float near, float far)
        : m_ProjectionMatrix(glm::perspective(glm::radians(fov), aspect, near, far))
        , m_ViewMatrix(glm::mat4(1.0f))
        , m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
        , m_Position(0.0)
        , m_Front(0.0f)
        , m_Up(0.0f, 1.0f, 0.0f)
        , m_Fov(fov)
        , m_Yaw(-90.0f)
        , m_Pitch(0.0f)
        , m_Aspect(aspect)
        , m_Near(near)
        , m_Far(far) {}

    void PerspectiveCamera::SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetFront(const glm::vec3& front) {
        m_Front = front;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::SetUp(const glm::vec3& up) {
        m_Up = up;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::MoveLeft(float offset) {
        m_Position.x -= offset;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::MoveRight(float offset) {
        m_Position.x += offset;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::MoveDown(float offset) {
        m_Position.y -= offset;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::MoveUp(float offset) {
        m_Position.y += offset;
        RecalculateViewMatrix();
    }

    void PerspectiveCamera::RecalculateViewMatrix() {
        m_ProjectionMatrix     = glm::perspective(m_Fov, m_Aspect, m_Near, m_Far);
        m_ViewMatrix           = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

} // namespace Ziben