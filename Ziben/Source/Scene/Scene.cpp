#include "Scene.hpp"

#include "Entity.hpp"
#include "Component.hpp"

#include "Ziben/Renderer/Renderer2D.hpp"

namespace Ziben {

    Scene::Scene(std::string name)
        : m_Name(std::move(name)) {}

    void Scene::OnRender() {
        auto group = m_Registry.group<TransformComponent, SpriteRendererComponent>();

        for (const auto& entity : group) {
            const auto& [transform, sprite] = group.get<TransformComponent, SpriteRendererComponent>(entity);

            Renderer2D::DrawQuad((const glm::mat4&)transform, (const glm::vec4&)sprite);
        }
    }

    Entity Scene::CreateEntity(const std::string& tag) {
        Entity entity(m_Registry.create(), this);

        entity.PushComponent<TagComponent>(tag);
        entity.PushComponent<TransformComponent>();

        return entity;
    }

} // namespace Ziben