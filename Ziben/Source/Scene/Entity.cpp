#include "Entity.hpp"

namespace Ziben {

    const Entity Entity::Null = Entity();

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_Handle(handle)
        , m_Scene(scene) {}

    bool Entity::operator !=(const Entity& other) const {
        return std::tie(m_Handle, m_Scene) != std::tie(other.m_Handle, other.m_Scene);
    }

    bool Entity::operator ==(const Entity& other) const {
        return std::tie(m_Handle, m_Scene) == std::tie(other.m_Handle, other.m_Scene);
    }

} // namespace Ziben