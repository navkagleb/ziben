#include "Scene.hpp"

#include "Entity.hpp"
#include "Component.hpp"
#include "Ziben/Renderer/Renderer2D.hpp"
#include "Ziben/Renderer/EditorCamera.hpp"

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

    void Scene::OnUpdateEditor(const TimeStep& ts, EditorCamera& camera) {

    }

    void Scene::OnRenderEditor(EditorCamera& camera) {
        Renderer2D::BeginScene(camera);
        {
            auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

            for (entt::entity handle : view) {
                const auto& [tc, src] = view.get<TransformComponent, SpriteRendererComponent>(handle);

                Renderer2D::DrawSprite(tc.GetTransform(), src, static_cast<int>(handle));
            }
        }
        Renderer2D::EndScene();
    }

    void Scene::OnUpdateRuntime(const TimeStep& ts) {
        // Update scripts
        m_Registry.view<NativeScriptComponent>().each([&](entt::entity handle, auto& component) {
            if (!component.m_Instance) {
                component.m_Instance = component.m_InstantiateScript();
                component.m_Instance->m_Entity = Entity(handle, this);
                component.m_Instance->OnCreate();
            }

            component.m_Instance->OnUpdate(ts);
        });
    }

    void Scene::OnRenderRuntime() {
        const Camera* primaryCamera          = nullptr;
        glm::mat4     primaryCameraTransform = glm::mat4(1.0f);

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();

            for (entt::entity handle : view) {
                const auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(handle);

                if (camera.IsPrimary) {
                    primaryCamera          = &camera.Camera;
                    primaryCameraTransform = transform.GetTransform();

                    break;
                }
            }
        }

        if (primaryCamera) {
            Renderer2D::BeginScene(*primaryCamera, primaryCameraTransform);
            {
                auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

                for (entt::entity handle : view) {
                    const auto& [tc, src] = view.get<TransformComponent, SpriteRendererComponent>(handle);

                    Renderer2D::DrawSprite(tc.GetTransform(), src, static_cast<int>(handle));
                }
            }
            Renderer2D::EndScene();
        }
    }

    void Scene::OnViewportResize(uint32_t width, uint32_t height) {
        m_ViewportWidth  = width;
        m_ViewportHeight = height;

        for (auto view = m_Registry.view<CameraComponent>(); entt::entity handle : view) {
            auto& cameraComponent = view.get<CameraComponent>(handle);

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

    Entity Scene::GetPrimaryCameraEntity() {
        for (auto view = m_Registry.view<const CameraComponent>(); entt::entity handle : view)
            if (m_Registry.get<CameraComponent>(handle).IsPrimary)
                return Entity(handle, this);

        return Entity::Null;
    }

} // namespace Ziben