#include "Scene.hpp"

#include "Entity.hpp"
#include "Component.hpp"
#include "Ziben/Renderer/Renderer2D.hpp"

namespace Ziben {

    template <typename Component>
    void Scene::OnComponentPushed(entt::registry& registry, entt::entity handle) {
        assert(false);
    }

    template <>
    void Scene::OnComponentPushed<CameraComponent>(entt::registry& registry, entt::entity handle) {
        registry.get<CameraComponent>(handle).Camera.SetViewportSize(m_ViewportWidth, m_ViewportHeight);
    }

    Scene::Scene(std::string name)
        : m_Name(std::move(name))
        , m_ViewportWidth(0)
        , m_ViewportHeight(0) {

        m_Registry.on_construct<CameraComponent>().connect<&Scene::OnComponentPushed<CameraComponent>>(this);
    }

    void Scene::OnUpdate(const TimeStep& ts) {
        // Update scripts
        m_Registry.view<NativeScriptComponent>().each([&](const auto& entity, auto& nsc) {
            if (!nsc.m_Instance) {
                nsc.m_Instance = nsc.m_InstantiateScript();
                nsc.m_Instance->m_Entity = Entity(entity, this);
                nsc.m_Instance->OnCreate();
            }

            nsc.m_Instance->OnUpdate(ts);
        });
    }

    void Scene::OnRender() {
        const Camera* primaryCamera          = nullptr;
        glm::mat4     primaryCameraTransform = glm::mat4(1.0f);

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();

            for (const auto& entity : view) {
                const auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.IsPrimary) {
                    primaryCamera          = &camera.Camera;
                    primaryCameraTransform = transform.GetTransform();

                    break;
                }
            }
        }

        if (primaryCamera) {
            Renderer2D::BeginScene(*primaryCamera, primaryCameraTransform);

            auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

            for (const auto& entity : view) {
                const auto& [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad((glm::mat4)transform, (const glm::vec4&)sprite);
            }

            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height) {
        m_ViewportWidth  = width;
        m_ViewportHeight = height;

        auto view = m_Registry.view<CameraComponent>();

        for (auto& entity : view) {
            auto& cameraComponent = view.get<CameraComponent>(entity);

            if (!cameraComponent.HasFixedAspectRatio)
                cameraComponent.Camera.SetViewportSize(width, height);
        }
    }

    Entity Scene::CreateEntity(const std::string& tag) {
        Entity entity(m_Registry.create(), this);

        entity.PushComponent<TagComponent>(tag);
        entity.PushComponent<TransformComponent>();

        return entity;
    }

    void Scene::DestroyEntity(const Entity& entity) {
        m_Registry.destroy((entt::entity)entity);
    }

} // namespace Ziben