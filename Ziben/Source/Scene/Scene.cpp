#include "Scene.hpp"

#include "Entity.hpp"
#include "Component.hpp"

#include "Ziben/Renderer/Renderer2D.hpp"

namespace Ziben {

    Scene::Scene(std::string name)
        : m_Name(std::move(name))
        , m_ViewportWidth(0)
        , m_ViewportHeight(0) {}

    void Scene::OnUpdate(const TimeStep& ts) {
        // Update scripts
        m_Registry.view<NativeScriptComponent>().each([&](const auto& entity, auto& nativeScriptComponent) {
            if (!nativeScriptComponent.m_Instance) {
                nativeScriptComponent.m_InstantiateFunction();
                nativeScriptComponent.m_Instance->m_Entity = Entity(entity, this);
                nativeScriptComponent.m_OnCreateFunction(nativeScriptComponent.m_Instance);
            }

            nativeScriptComponent.m_OnUpdateFunction(nativeScriptComponent.m_Instance, ts);
        });
    }

    void Scene::OnRender() {
        const Camera*    primaryCamera          = nullptr;
        const glm::mat4* primaryCameraTransform = nullptr;

        {
            auto view = m_Registry.view<TransformComponent, CameraComponent>();

            for (const auto& entity : view) {
                const auto& [transform, camera] = view.get<TransformComponent, CameraComponent>(entity);

                if (camera.IsPrimary()) {
                    primaryCamera          = &camera.GetCamera();
                    primaryCameraTransform = &transform.GetTransform();

                    break;
                }
            }
        }

        if (primaryCamera) {
            Renderer2D::BeginScene(*primaryCamera, *primaryCameraTransform);

            auto view = m_Registry.view<TransformComponent, SpriteRendererComponent>();

            for (const auto& entity : view) {
                const auto& [transform, sprite] = view.get<TransformComponent, SpriteRendererComponent>(entity);

                Renderer2D::DrawQuad((const glm::mat4&)transform, (const glm::vec4&)sprite);
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

            if (!cameraComponent.HasFixedAspectRatio())
                cameraComponent.GetCamera().SetViewportSize(width, height);
        }
    }

    Entity Scene::CreateEntity(const std::string& tag) {
        Entity entity(m_Registry.create(), this);

        entity.PushComponent<TagComponent>(tag);
        entity.PushComponent<TransformComponent>();

        return entity;
    }

} // namespace Ziben