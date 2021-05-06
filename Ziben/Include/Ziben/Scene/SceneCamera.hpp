#pragma once

#include "Ziben/Renderer/Camera.hpp"

namespace Ziben {

    class SceneCamera : public Camera {
    public:
        SceneCamera();
        ~SceneCamera() override = default;

    public:
        [[nodiscard]] inline float GetOrthographicSize() const { return m_OrthographicSize; }

        void SetOrthographicSize(float size);
        void SetOrthographic(float size, float nearClip, float farClip);
        void SetViewportSize(uint32_t width, uint32_t height);

    private:
        void RecalculateProjection();

    private:
        float m_OrthographicSize;
        float m_OrthographicNear;
        float m_OrthographicFar;

        float m_AspectRatio;

    }; // class SceneCamera

} // namespace Ziben