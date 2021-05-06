#include "SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ziben {

    SceneCamera::SceneCamera()
        : m_OrthographicSize(10.0f)
        , m_OrthographicNear(-1.0f)
        , m_OrthographicFar(1.0f)
        , m_AspectRatio(0.0f) {

        RecalculateProjection();
    }

    void SceneCamera::SetOrthographicSize(float size) {
        m_OrthographicSize = size;

        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(float size, float nearClip, float farClip) {
        m_OrthographicSize = size;
        m_OrthographicNear = nearClip;
        m_OrthographicFar  = farClip;

        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
        m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);

        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection() {
        float orthographicLeft   = -m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthographicRight  =  m_OrthographicSize * m_AspectRatio * 0.5f;
        float orthographicBottom = -m_OrthographicSize * 0.5f;
        float orthographicTop    =  m_OrthographicSize * 0.5f;

        m_ProjectionMatrix = glm::ortho(
            orthographicLeft,
            orthographicRight,
            orthographicBottom,
            orthographicTop,
            m_OrthographicNear,
            m_OrthographicFar
        );
    }

} // namespace Ziben