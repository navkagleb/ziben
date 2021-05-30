#include "SceneCamera.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace Ziben {

    SceneCamera::SceneCamera()
        : m_ProjectionType(ProjectionType::Orthographic)
        , m_AspectRatio(0.0f) {

        RecalculateProjection();
    }

    void SceneCamera::SetProjectionType(ProjectionType projectionType) {
        m_ProjectionType = projectionType;
        RecalculateProjection();
    }

    void SceneCamera::SetPerspectiveFov(float fov) {
        m_PerspectiveProps.Fov = fov;
        RecalculateProjection();
    }

    void SceneCamera::SetPerspectiveNear(float near) {
        m_PerspectiveProps.Near = near;
        RecalculateProjection();
    }

    void SceneCamera::SetPerspectiveFar(float far) {
        m_OrthographicProps.Far = far;
        RecalculateProjection();
    }

    void SceneCamera::SetPerspective(const PerspectiveProps& props) {
        m_ProjectionType   = ProjectionType::Perspective;
        m_PerspectiveProps = props;
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographicSize(float size) {
        m_OrthographicProps.Size = size;
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographicNear(float near) {
        m_OrthographicProps.Near = near;
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographicFar(float far) {
        m_OrthographicProps.Far = far;
        RecalculateProjection();
    }

    void SceneCamera::SetOrthographic(const OrthographicProps& props) {
        m_ProjectionType    = ProjectionType::Orthographic;
        m_OrthographicProps = props;
        RecalculateProjection();
    }

    void SceneCamera::SetViewportSize(uint32_t width, uint32_t height) {
        m_AspectRatio = static_cast<float>(width) / static_cast<float>(height);
        RecalculateProjection();
    }

    void SceneCamera::RecalculateProjection() {
        switch (m_ProjectionType) {
            case ProjectionType::Perspective: {
                m_ProjectionMatrix = glm::perspective(
                    m_PerspectiveProps.Fov,
                    m_AspectRatio,
                    m_PerspectiveProps.Near,
                    m_PerspectiveProps.Far
                );

                break;
            }

            case ProjectionType::Orthographic: {
                float orthographicLeft   = -m_OrthographicProps.Size * m_AspectRatio * 0.5f;
                float orthographicRight  =  m_OrthographicProps.Size * m_AspectRatio * 0.5f;
                float orthographicBottom = -m_OrthographicProps.Size * 0.5f;
                float orthographicTop    =  m_OrthographicProps.Size * 0.5f;

                m_ProjectionMatrix = glm::ortho(
                    orthographicLeft,
                    orthographicRight,
                    orthographicBottom,
                    orthographicTop,
                    m_OrthographicProps.Near,
                    m_OrthographicProps.Far
                );

                break;
            }
        }
    }

} // namespace Ziben