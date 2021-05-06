#include "Component.hpp"

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

    NativeScriptComponent::NativeScriptComponent()
        : m_Instance(nullptr)
        , m_InstantiateScript(nullptr)
        , m_DestroyScript(nullptr) {}

} // namespace Ziben