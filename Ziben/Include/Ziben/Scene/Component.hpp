#pragma once

#include <glm/glm.hpp>

namespace Ziben {

    class TagComponent {
    public:
        explicit TagComponent(std::string tag = "EnTT");

        [[nodiscard]] inline const std::string& GetTag() const { return m_Tag; }

        inline explicit operator const std::string& () const { return m_Tag; }

    private:
        std::string m_Tag;

    }; // class TagComponent

    class TransformComponent {
    public:
        explicit TransformComponent(const glm::mat4& transform = glm::mat4(1.0f));
        ~TransformComponent() = default;

        inline explicit operator const glm::mat4& () const { return m_Transform; }

    private:
        glm::mat4 m_Transform;

    }; // class TransformComponent

    class SpriteRendererComponent {
    public:
        explicit SpriteRendererComponent(const glm::vec4& color = glm::vec4(1.0f));
        ~SpriteRendererComponent() = default;

        [[nodiscard]] inline glm::vec4& GetColor() { return m_Color; }
        [[nodiscard]] inline const glm::vec4& GetColor() const { return m_Color; }

        inline explicit operator const glm::vec4& () const { return m_Color; }

    private:
        glm::vec4 m_Color;

    }; // class SpriteRendererComponent

} // namespace Ziben