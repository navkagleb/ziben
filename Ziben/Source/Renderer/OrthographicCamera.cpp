#include "OrthographicCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ziben {

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f))
        , m_ViewMatrix(1.0f)
        , m_ViewProjectionMatrix(m_ProjectionMatrix * m_ViewMatrix)
        , m_Position(0.0f)
        , m_Rotation(0.0f) {}

    void OrthographicCamera::SetPosition(const glm::vec3& position) {
        m_Position = position;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::SetRotation(float rotation) {
        m_Rotation = rotation;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::MoveLeft(float offset) {
        m_Position.x -= offset;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::MoveRight(float offset) {
        m_Position.x += offset;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::MoveDown(float offset) {
        m_Position.y -= offset;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::MoveUp(float offset) {
        m_Position.y += offset;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RotateLeft(float angle) {
        m_Rotation -= angle;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RotateRight(float angle) {
        m_Rotation += angle;
        RecalculateViewMatrix();
    }

    void OrthographicCamera::RecalculateViewMatrix() {
        glm::mat4 transform = glm::translate(
            glm::mat4(1.0f),
            m_Position
        ) * glm::rotate(
            glm::mat4(1.0f),
            glm::radians(m_Rotation),
            glm::vec3(0.0f, 0.0f, 1.0f)
        );

        m_ViewMatrix           = glm::inverse(transform);
        m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;
    }

} // namespace Ziben