#pragma once

#include <glm/glm.hpp>

#include "Ziben/Renderer/Camera.hpp"

namespace Ziben {

    class TagComponent {
    public:
        explicit TagComponent(std::string tag = "EnTT");
        ~TagComponent() = default;

    public:
        [[nodiscard]] inline const std::string& GetTag() const { return m_Tag; }

    public:
        inline explicit operator const std::string& () const { return m_Tag; }

    private:
        std::string m_Tag;

    }; // class TagComponent

    class TransformComponent {
    public:
        explicit TransformComponent(const glm::mat4& transform = glm::mat4(1.0f));
        ~TransformComponent() = default;

    public:

        [[nodiscard]] const glm::mat4& GetTransform() const { return m_Transform; }

    public:
        inline explicit operator glm::mat4& () { return m_Transform; }
        inline explicit operator const glm::mat4& () const { return m_Transform; }

    private:
        glm::mat4 m_Transform;

    }; // class TransformComponent

    class SpriteRendererComponent {
    public:
        explicit SpriteRendererComponent(const glm::vec4& color = glm::vec4(1.0f));
        ~SpriteRendererComponent() = default;

    public:
        [[nodiscard]] inline glm::vec4& GetColor() { return m_Color; }
        [[nodiscard]] inline const glm::vec4& GetColor() const { return m_Color; }

    public:
        inline explicit operator const glm::vec4& () const { return m_Color; }

    private:
        glm::vec4 m_Color;

    }; // class SpriteRendererComponent

    class CameraComponent {
    public:
        explicit CameraComponent(const glm::mat4& projectionMatrix = glm::mat4(1.0f), bool isPrimary = false);
        ~CameraComponent() = default;

    public:
        [[nodiscard]] inline const Camera& GetCamera() const { return m_Camera; }
        [[nodiscard]] inline bool IsPrimary() const { return m_IsPrimary; }

        void SetPrimary(bool isPrimary);

    private:
        Camera m_Camera;
        bool   m_IsPrimary;

    }; // class CameraComponent

} // namespace Ziben