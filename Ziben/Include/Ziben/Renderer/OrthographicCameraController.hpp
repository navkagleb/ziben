#pragma once

#include "OrthographicCamera.hpp"
#include "Ziben/Window/TimeStep.hpp"
#include "Ziben/Window/MouseEvent.hpp"
#include "Ziben/Window/WindowEvent.hpp"

namespace Ziben {

    class OrthographicCameraController {
    public:
        explicit OrthographicCameraController(float aspectRatio, bool isRotated = false);

        [[nodiscard]] inline OrthographicCamera& GetCamera() { return m_Camera; }
        [[nodiscard]] inline const OrthographicCamera& GetCamera() const { return m_Camera; }

        void OnEvent(Event& event);
        void OnUpdate(const TimeStep& ts);

    private:
        bool OnMouseScrolled(MouseScrolledEvent& event);
        bool OnWindowResized(WindowResizedEvent& event);

    private:
        float              m_AspectRation;
        float              m_ZoomLevel;
        bool               m_IsRotated;

        OrthographicCamera m_Camera;

        glm::vec3          m_CameraPosition;
        float              m_CameraRotation;

        float              m_CameraTranslationSpeed;
        float              m_CameraRotationSpeed;

    }; // class OrthographicCameraController

} // namespace Ziben