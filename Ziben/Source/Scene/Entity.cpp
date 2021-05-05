#include "Entity.hpp"

namespace Ziben {

    Entity::Entity(entt::entity handle, Scene* scene)
        : m_Handle(handle)
        , m_Scene(scene) {}

} // namespace Ziben