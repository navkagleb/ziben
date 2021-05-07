#pragma once

#include <glm/glm.hpp>

#include "SceneCamera.hpp"
#include "ScriptableEntity.hpp"

namespace Ziben {

    class TagComponent {
    public:
        explicit TagComponent(std::string tag = "EnTT");
        ~TagComponent() = default;

    public:
        [[nodiscard]] inline const std::string& GetTag() const { return m_Tag; }

        void SetTag(const std::string& tag);

    public:
        inline explicit operator std::string& () { return m_Tag; }
        inline explicit operator const std::string& () const { return m_Tag; }

    private:
        std::string m_Tag;

    }; // class TagComponent

    class TransformComponent {
    public:
        explicit TransformComponent(const glm::vec3& translation = glm::vec3(0.0f));
        ~TransformComponent() = default;

    public:
        [[nodiscard]] inline float GetX() const { return m_Translation.x; }
        [[nodiscard]] inline float GetY() const { return m_Translation.y; }
        [[nodiscard]] inline float GetZ() const { return m_Translation.z; }
        [[nodiscard]] inline const glm::vec3& GetTranslation() const { return m_Translation; }

        [[nodiscard]] inline float GetRotationX() const { return m_Rotation.x; }
        [[nodiscard]] inline float GetRotationY() const { return m_Rotation.x; }
        [[nodiscard]] inline float GetRotationZ() const { return m_Rotation.z; }
        [[nodiscard]] inline const glm::vec3& GetRotation() const { return m_Rotation; }

        [[nodiscard]] inline float GetScaleX() const { return m_Scale.x; }
        [[nodiscard]] inline float GetScaleY() const { return m_Scale.y; }
        [[nodiscard]] inline float GetScaleZ() const { return m_Scale.z; }
        [[nodiscard]] inline const glm::vec3& GetScale() const { return m_Scale; }

        [[nodiscard]] glm::mat4 GetTransform() const;

        void SetX(float x);
        void SetY(float y);
        void SetZ(float z);
        void SetTranslation(const glm::vec3& translation);

        void SetRotationX(float rotationX);
        void SetRotationY(float rotationY);
        void SetRotationZ(float rotationZ);
        void SetRotation(const glm::vec3& rotation);

        void SetScaleX(float scaleX);
        void SetScaleY(float scaleY);
        void SetScaleZ(float scaleZ);
        void SetScale(const glm::vec3& scale);

    public:
        explicit operator glm::mat4 () const { return GetTransform(); }

    private:
        glm::vec3 m_Translation;
        glm::vec3 m_Rotation;
        glm::vec3 m_Scale;

    }; // class TransformComponent

    class SpriteRendererComponent {
    public:
        explicit SpriteRendererComponent(const glm::vec4& color = glm::vec4(1.0f));
        ~SpriteRendererComponent() = default;

    public:
        [[nodiscard]] inline glm::vec4& GetColor() { return m_Color; }
        [[nodiscard]] inline const glm::vec4& GetColor() const { return m_Color; }

        void SetColor(const glm::vec4& color);

    public:
        inline explicit operator const glm::vec4& () const { return m_Color; }

    private:
        glm::vec4 m_Color;

    }; // class SpriteRendererComponent

    class CameraComponent {
    public:
        explicit CameraComponent(bool isPrimary = false, bool hasFixedAspectRatio = false);
        ~CameraComponent() = default;

    public:
        [[nodiscard]] inline SceneCamera& GetCamera() { return m_Camera; }
        [[nodiscard]] inline const SceneCamera& GetCamera() const { return m_Camera; }
        [[nodiscard]] inline bool IsPrimary() const { return m_IsPrimary; }
        [[nodiscard]] inline bool HasFixedAspectRatio() const { return m_HasFixedAspectRatio; }

        void SetPrimary(bool isPrimary);
        void SetFixedAspectRatio(bool hasFixedAspectRatio);

        void Init() { ZIBEN_WARN("Fuck"); }

    private:
        SceneCamera m_Camera;
        bool        m_IsPrimary;
        bool        m_HasFixedAspectRatio;

    }; // class CameraComponent

    class NativeScriptComponent {
    public:
        NativeScriptComponent();
        ~NativeScriptComponent() = default;

    public:
        template <typename T>
        void Bind();

    public:
        using InstantiateScript = ScriptableEntity*(*)();
        using DestroyScript     = void(*)(NativeScriptComponent*);

    public:
        ScriptableEntity*     m_Instance;
        InstantiateScript     m_InstantiateScript;
        DestroyScript         m_DestroyScript;

    }; // class NativeScriptComponent

} // namespace Ziben

#include "Component.inl"