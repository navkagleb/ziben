#include "EditorCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtx/quaternion.hpp>

#include "Ziben/Window/Event.hpp"
#include "Ziben/Window/EventDispatcher.hpp"
#include "Ziben/Window/MouseEvent.hpp"
#include "Ziben/Window/Input.hpp"

namespace Ziben {

    EditorCamera::EditorCamera()
        : Camera(glm::perspective(glm::radians(45.0f), 1.778f, 0.1f, 1000.0f))
        , m_Fov(45.0f)
        , m_AspectRatio(1.778f)
        , m_Near(0.1f)
        , m_Far(1000.0f)
        , m_ViewMatrix(1.0f)
        , m_Position(0.0f)
        , m_FocalPoint(0.0f)
        , m_InitialMousePosition(0.0f)
        , m_Distance(10.0f)
        , m_Pitch(0.0f)
        , m_Yaw(0.0f)
        , m_ViewportWidth(1280.0f)
        , m_ViewportHeight(720.0f)
        , m_IsActive(false) {

        UpdateViewMatrix();
    }

    EditorCamera::EditorCamera(float fov, float aspectRatio, float near, float far)
        : Camera(glm::perspective(glm::radians(fov), aspectRatio, near, far))
        , m_Fov(fov)
        , m_AspectRatio(aspectRatio)
        , m_Near(near)
        , m_Far(far)
        , m_ViewMatrix(1.0f)
        , m_Position(0.0f)
        , m_FocalPoint(0.0f)
        , m_InitialMousePosition(0.0f)
        , m_Distance(10.0f)
        , m_Pitch(0.0f)
        , m_Yaw(0.0f)
        , m_ViewportWidth(1280.0f)
        , m_ViewportHeight(720.0f)
        , m_IsActive(false) {

        UpdateViewMatrix();
    }

    glm::mat4 EditorCamera::GetViewProjectionMatrix() const {
        return m_ProjectionMatrix * m_ViewMatrix;
    }

    glm::vec3 EditorCamera::GetUpDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetRightDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));
    }

    glm::vec3 EditorCamera::GetForwardDirection() const {
        return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));
    }

    glm::quat EditorCamera::GetOrientation() const {
        return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
    }

    void EditorCamera::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<MouseScrolledEvent>(ZIBEN_BIND_EVENT_FUNC(OnMouseScrolled));
    }

    void EditorCamera::OnUpdate(const TimeStep& ts) {
        m_IsActive = false;

        if (Input::IsKeyPressed(Key::LeftAlt)) {
            m_IsActive = true;

            glm::vec2 delta = (Input::GetMousePosition<float>() - m_InitialMousePosition) * 0.003f;

            m_InitialMousePosition = Input::GetMousePosition<float>();

            if (Input::IsButtonPressed(Button::Middle))
                MousePan(delta);
            else if (Input::IsButtonPressed(Button::Left))
                MouseRotate(delta);
            else if (Input::IsButtonPressed(Button::Right))
                MouseZoom(delta.y);
        }

        UpdateViewMatrix();
    }

    void EditorCamera::SetDistance(float distance) {
        m_Distance = distance;
    }

    void EditorCamera::SetViewportSize(float width, float height) {
        m_ViewportWidth  = width;
        m_ViewportHeight = height;

        UpdateProjectionMatrix();
    }

    bool EditorCamera::OnMouseScrolled(MouseScrolledEvent& event) {
        MouseZoom(static_cast<float>(event.GetOffsetY()) * 0.1f);
        UpdateViewMatrix();

        return false;
    }

    void EditorCamera::UpdateProjectionMatrix() {
        m_AspectRatio      = m_ViewportWidth / m_ViewportHeight;
        m_ProjectionMatrix = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);
    }

    void EditorCamera::UpdateViewMatrix() {
        // m_Yaw = m_Pitch = 0.0f; // Lock the camera's rotation

        m_Position            = m_FocalPoint - GetForwardDirection() * m_Distance;

        glm::quat orientation = GetOrientation();
        m_ViewMatrix          = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);
        m_ViewMatrix          = glm::inverse(m_ViewMatrix);
    }

    void EditorCamera::MousePan(const glm::vec2& delta) {
        auto [speedX, speedY] = GetPanSpeed();

        m_FocalPoint += -GetRightDirection() * delta.x * speedX * m_Distance;
        m_FocalPoint +=  GetUpDirection()    * delta.y * speedY * m_Distance;
    }

    void EditorCamera::MouseRotate(const glm::vec2& delta) {
        float yawSign       = GetUpDirection().y < 0 ? -1.0f : 1.0f;
        float rotationSpeed = 0.8f;

        m_Yaw   += rotationSpeed * yawSign * delta.x;
        m_Pitch += rotationSpeed * delta.y;
    }

    void EditorCamera::MouseZoom(float delta) {
        m_Distance -= delta * GetZoomSpeed();

        if (m_Distance < 1.0f) {
            m_FocalPoint += GetForwardDirection();
            m_Distance = 1.0f;
        }
    }

    std::pair<float, float> EditorCamera::GetPanSpeed() const {
        float x       = std::min(m_ViewportWidth / 1000.0f, 2.4f);
        float factorX = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

        float y       = std::min(m_ViewportHeight / 1000.0f, 2.4f);
        float factorY = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

        return { factorX, factorY };
    }

    float EditorCamera::GetZoomSpeed() const {
        float distance = std::max(m_Distance * 0.2f, 0.0f);

        return std::min(std::pow(distance, 2.0f), 100.0f);
    }

} // namespace Ziben