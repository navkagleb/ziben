#pragma once

#include "Camera.hpp"

namespace Ziben {

    class Event;
    class MouseScrolledEvent;
    class TimeStep;

    class EditorCamera : public Camera {
    public:
        EditorCamera();
        EditorCamera(float fov, float aspectRatio, float near, float far);
        ~EditorCamera() = default;

    public:
        [[nodiscard]] inline const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        [[nodiscard]] inline const glm::vec3& GetPosition() const { return m_Position; }
        [[nodiscard]] inline float GetDistance() const { return m_Distance; }
        [[nodiscard]] inline float GetPitch() const { return m_Pitch; }
        [[nodiscard]] inline float GetYaw() const { return m_Yaw; }
        [[nodiscard]] inline bool IsActive() const { return m_IsActive; }

        [[nodiscard]] glm::mat4 GetViewProjectionMatrix() const;
        [[nodiscard]] glm::vec3 GetUpDirection() const;
        [[nodiscard]] glm::vec3 GetRightDirection() const;
        [[nodiscard]] glm::vec3 GetForwardDirection() const;
        [[nodiscard]] glm::quat GetOrientation() const;

        void OnEvent(Event& event);
        void OnUpdate(const TimeStep& ts);

        void SetDistance(float distance);

        void SetViewportSize(float width, float height);

    private:
        bool OnMouseScrolled(MouseScrolledEvent& event);

        void UpdateProjectionMatrix();
        void UpdateViewMatrix();

        void MousePan(const glm::vec2& delta);
        void MouseRotate(const glm::vec2& delta);
        void MouseZoom(float delta);

        [[nodiscard]] std::pair<float, float> GetPanSpeed() const;
        [[nodiscard]] float GetZoomSpeed() const;

    private:
        float m_Fov;
        float m_AspectRatio;
        float m_Near;
        float m_Far;

        glm::mat4 m_ViewMatrix;
        glm::vec3 m_Position;
        glm::vec3 m_FocalPoint;

        glm::vec2 m_InitialMousePosition;

        float m_Distance;
        float m_Pitch;
        float m_Yaw;

        float m_ViewportWidth;
        float m_ViewportHeight;

        bool  m_IsActive;

    }; // class EditorCamera

} // namespace Ziben