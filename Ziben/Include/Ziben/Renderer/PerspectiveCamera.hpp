#pragma once

#include <glm/glm.hpp>

namespace Ziben {

    class PerspectiveCamera {
    public:
        PerspectiveCamera(float fov, float aspect, float near, float far);
        ~PerspectiveCamera() = default;

    public:
        [[nodiscard]] inline const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        [[nodiscard]] inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        [[nodiscard]] inline const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
        [[nodiscard]] inline const glm::vec3& GetPosition() const { return m_Position; }

        [[nodiscard]] inline float GetFov() const { return m_Fov; }
        [[nodiscard]] inline float GetYaw() const { return m_Yaw; }
        [[nodiscard]] inline float GetPitch() const { return m_Pitch; }

        void SetFov(float fov) { m_Fov = fov; RecalculateViewMatrix(); }
        void SetYaw(float yaw) { m_Yaw = yaw; RecalculateViewMatrix(); }
        void SetPitch(float pitch) { m_Pitch = pitch; RecalculateViewMatrix(); }

        void SetPosition(const glm::vec3& position);
        void SetFront(const glm::vec3& front);
        void SetUp(const glm::vec3& up);

        void MoveLeft(float offset);
        void MoveRight(float offset);
        void MoveDown(float offset);
        void MoveUp(float offset);
//
//        void RotateLeft(float angle);
//        void RotateRight(float angle);

    private:
        void RecalculateViewMatrix();

    private:
        glm::mat4 m_ProjectionMatrix;
        glm::mat4 m_ViewMatrix;
        glm::mat4 m_ViewProjectionMatrix;

        glm::vec3 m_Position;
        glm::vec3 m_Front;
        glm::vec3 m_Up;

        float     m_Fov;
        float     m_Yaw;
        float     m_Pitch;
        float     m_Aspect;
        float     m_Near;
        float     m_Far;

    }; // class PerspectiveCamera

} // namespace Ziben