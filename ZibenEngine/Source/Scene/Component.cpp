#include "Component.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>

namespace Ziben {

    TransformComponent::TransformComponent(const glm::vec3& translation)
        : m_Translation(translation)
        , m_Rotation(0.0f)
        , m_Scale(1.0f) {}

    glm::mat4 TransformComponent::GetTransform() const {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_Translation);
        glm::mat4 rotation    = glm::toMat4(glm::quat(m_Rotation));
        glm::mat4 scale       = glm::scale(glm::mat4(1.0f), m_Scale);

        return translation * rotation * scale;
    }

    void TransformComponent::SetX(float x) {
        m_Translation.x = x;
    }

    void TransformComponent::SetY(float y) {
        m_Translation.y = y;
    }

    void TransformComponent::SetZ(float z) {
        m_Translation.z = z;
    }

    void TransformComponent::SetTranslation(const glm::vec3& translation) {
        m_Translation = translation;
    }

    void TransformComponent::SetRotationX(float rotationX) {
        m_Rotation.x = rotationX;
    }

    void TransformComponent::SetRotationY(float rotationY) {
        m_Rotation.y = rotationY;
    }

    void TransformComponent::SetRotationZ(float rotationZ) {
        m_Rotation.z = rotationZ;
    }

    void TransformComponent::SetRotation(const glm::vec3& rotation) {
        m_Rotation = rotation;
    }

    void TransformComponent::SetScaleX(float scaleX) {
        m_Scale.x = scaleX;
    }

    void TransformComponent::SetScaleY(float scaleY) {
        m_Scale.y = scaleY;
    }

    void TransformComponent::SetScaleZ(float scaleZ) {
        m_Scale.z = scaleZ;
    }

    void TransformComponent::SetScale(const glm::vec3& scale) {
        m_Scale = scale;
    }

    NativeScriptComponent::NativeScriptComponent()
        : m_Instance(nullptr)
        , m_InstantiateScript(nullptr)
        , m_DestroyScript(nullptr) {}

} // namespace Ziben