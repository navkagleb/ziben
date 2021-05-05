#include "OrthographicCameraController.hpp"

#include "Ziben/Window/EventDispatcher.hpp"
#include "Ziben/Window/Input.hpp"

#include "Ziben/System.hpp"

namespace Ziben {

    OrthographicCameraController::OrthographicCameraController(float aspectRation, bool isRotated)
        : m_AspectRatio(aspectRation)
        , m_ZoomLevel(1.0f)
        , m_IsRotated(isRotated)
        , m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel })
        , m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
        , m_CameraPosition(0.0f)
        , m_CameraRotation(0.0f)
        , m_CameraTranslationSpeed(2.0f)
        , m_CameraRotationSpeed(50.0f) {}

    void OrthographicCameraController::SetZoomLevel(float level) {
        m_ZoomLevel = level;
        UpdateCamera();
    }

    void OrthographicCameraController::OnEvent(Event& event) {
        ZIBEN_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseScrolledEvent>(ZIBEN_BIND_EVENT_FUNC(OnMouseScrolled));
        dispatcher.Dispatch<WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
    }

    void OrthographicCameraController::OnUpdate(const TimeStep& ts) {
        ZIBEN_PROFILE_FUNCTION();

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

    void OrthographicCameraController::OnResize(float width, float height) {
        m_AspectRatio = width / height;
        UpdateCamera();
    }

    bool OrthographicCameraController::OnMouseScrolled(MouseScrolledEvent& event) {
        ZIBEN_PROFILE_FUNCTION();

        m_ZoomLevel = std::max(m_ZoomLevel - static_cast<float>(event.GetOffsetY()) * 0.5f, 0.25f);
        UpdateCamera();

        return false;
    }

    bool OrthographicCameraController::OnWindowResized(WindowResizedEvent& event) {
        ZIBEN_PROFILE_FUNCTION();

        if (event.GetWidth() != 0 && event.GetHeight() != 0)
            OnResize(static_cast<float>(event.GetWidth()), static_cast<float>(event.GetHeight()));

        return false;
    }

    void OrthographicCameraController::UpdateCamera() {
        m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };
        m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);
    }

} // namespace Ziben