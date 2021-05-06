#pragma once

#include "Ziben/Renderer/Camera.hpp"

namespace Ziben {

    class SceneCamera : public Camera {
    public:
        enum class ProjectionType : uint8_t { Perspective = 0, Orthographic = 1 };

    public:
        struct PerspectiveProps {
            float Fov  = glm::radians(45.0f);
            float Near = 0.01f;
            float Far  = 100.0f;
        };

        struct OrthographicProps {
            float Size =  10.0f;
            float Near = -1.0f;
            float Far  =  1.0f;
        };

    public:
        SceneCamera();
        ~SceneCamera() override = default;

    public:
        [[nodiscard]] inline ProjectionType GetProjectionType() const { return m_ProjectionType; }
        [[nodiscard]] inline const PerspectiveProps& GetPerspectiveProps() const { return m_PerspectiveProps; }
        [[nodiscard]] inline const OrthographicProps& GetOrthographicProps() const { return m_OrthographicProps; }

        [[nodiscard]] inline bool IsPerspective() const { return m_ProjectionType == ProjectionType::Perspective; }
        [[nodiscard]] inline bool IsOrthographic() const { return m_ProjectionType == ProjectionType::Orthographic; }

        void SetProjectionType(ProjectionType projectionType);

        void SetPerspectiveFov(float fov);
        void SetPerspectiveNear(float near);
        void SetPerspectiveFar(float far);
        void SetPerspective(const PerspectiveProps& props);

        void SetOrthographicSize(float size);
        void SetOrthographicNear(float near);
        void SetOrthographicFar(float far);
        void SetOrthographic(const OrthographicProps& props);

        void SetViewportSize(uint32_t width, uint32_t height);

    private:
        void RecalculateProjection();

    private:
        ProjectionType    m_ProjectionType;
        OrthographicProps m_OrthographicProps;
        PerspectiveProps  m_PerspectiveProps;
        float             m_AspectRatio;

    }; // class SceneCamera

} // namespace Ziben