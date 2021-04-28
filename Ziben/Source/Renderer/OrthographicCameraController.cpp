#include "OrthographicCameraController.hpp"

#include "Ziben/Window/EventDispatcher.hpp"
#include "Ziben/Window/Input.hpp"

namespace Ziben {

    OrthographicCameraController::OrthographicCameraController(float aspectRation, bool isRotated)
        : m_AspectRation(aspectRation)
        , m_ZoomLevel(1.0f)
        , m_IsRotated(isRotated)
        , m_Camera(-m_AspectRation * m_ZoomLevel, m_AspectRation * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
        , m_CameraPosition(0.0f)
        , m_CameraRotation(0.0f)
        , m_CameraTranslationSpeed(2.0f)
        , m_CameraRotationSpeed(50.0f) {

    }

    void OrthographicCameraController::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseScrolledEvent>(ZIBEN_BIND_EVENT_FUNC(OnMouseScrolled));
        dispatcher.Dispatch<WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
    }

    void OrthographicCameraController::OnUpdate(const TimeStep& ts) {
        // Movement
        if (Input::IsKeyPressed(Ziben::Key::A))
            m_CameraPosition.x -= m_CameraTranslationSpeed * (float)ts;
        else if (Input::IsKeyPressed(Ziben::Key::D))
            m_CameraPosition.x += m_CameraTranslationSpeed * (float)ts;

        if (Input::IsKeyPressed(Ziben::Key::S))
            m_CameraPosition.y -= m_CameraTranslationSpeed * (float)ts;
        else if (Input::IsKeyPressed(Ziben::Key::W))
            m_CameraPosition.y += m_CameraTranslationSpeed * (float)ts;

        m_Camera.SetPosition(m_CameraPosition);

        // Rotation
        if (m_IsRotated) {
            if (Ziben::Input::IsKeyPressed(Ziben::Key::Q))
                m_CameraRotation -= m_CameraRotationSpeed * (float)ts;
            else if (Ziben::Input::IsKeyPressed(Ziben::Key::E))
                m_CameraRotation += m_CameraRotationSpeed * (float)ts;

            m_Camera.SetRotation(m_CameraRotation);
        }

        m_CameraTranslationSpeed = 2.0f * m_ZoomLevel;
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
        m_ZoomLevel -= static_cast<float>(event.GetOffsetY()) * 0.25f;
        m_ZoomLevel  = std::max(m_ZoomLevel, 0.25f);

        m_Camera.SetProjection(
            -m_AspectRation * m_ZoomLevel,
             m_AspectRation * m_ZoomLevel,
            -m_ZoomLevel,
             m_ZoomLevel
        );

        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& event) {
        m_AspectRation = static_cast<float>(event.GetWidth()) / static_cast<float>(event.GetHeight());

        m_Camera.SetProjection(
            -m_AspectRation * m_ZoomLevel,
            m_AspectRation * m_ZoomLevel,
            -m_ZoomLevel,
            m_ZoomLevel
        );

        return false;
    }

} // namespace Ziben