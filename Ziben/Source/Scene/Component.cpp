#include "Component.hpp"

#include <utility>

namespace Ziben {

    TagComponent::TagComponent(std::string tag)
        : m_Tag(std::move(tag)) {}

    TransformComponent::TransformComponent(const glm::mat4& transform)
        : m_Transform(transform) {}

    SpriteRendererComponent::SpriteRendererComponent(const glm::vec4& color)
        : m_Color(color) {}

    CameraComponent::CameraComponent(bool isPrimary, bool hasFixedAspectRatio)
        : m_IsPrimary(isPrimary)
        , m_HasFixedAspectRatio(hasFixedAspectRatio) {}

    void CameraComponent::SetPrimary(bool isPrimary) {
        m_IsPrimary = isPrimary;
    }

    void CameraComponent::SetFixedAspectRatio(bool hasFixedAspectRatio) {
        m_HasFixedAspectRatio = hasFixedAspectRatio;
    }

} // namespace Ziben