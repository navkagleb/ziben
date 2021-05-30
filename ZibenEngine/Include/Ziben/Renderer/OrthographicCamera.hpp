#pragma once

#include <glm/glm.hpp>

namespace Ziben {

    class OrthographicCamera {
    public:
        OrthographicCamera(float left, float right, float bottom, float top);
        ~OrthographicCamera() = default;

    public:
        [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        [[nodiscard]] inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        [[nodiscard]] inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        [[nodiscard]] inline const glm::vec3& GetPosition() const { return m_Position; }
        [[nodiscard]] inline float GetRotation() const { return m_Rotation; }

        void SetPosition(const glm::vec3& position);
        void SetRotation(float rotation);

        void SetProjection(float left, float right, float bottom, float top);

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;
        glm::vec3 m_Position;
        float     m_Rotation;

    }; // class OrthographicCamera

} // namespace Ziben