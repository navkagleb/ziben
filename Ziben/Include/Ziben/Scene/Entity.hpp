#pragma once

#include <entt/entt.hpp>

#include "Scene.hpp"

namespace Ziben {

    class Scene;

    class Entity {
    public:
        explicit Entity(entt::entity handle = entt::null, Scene* scene = nullptr);
        ~Entity() = default;

    public:
        template <typename Component>
        [[nodiscard]] bool HasComponent() const;

        template <typename Component>
        [[nodiscard]] Component& GetComponent();

        template <typename Component>
        [[nodiscard]] const Component& GetComponent() const;

        template <typename Component, typename... Args>
        Component& GetOrPushComponent(Args&&... args);

        template <typename Component, typename... Args>
        Component& PushComponent(Args&&... args);

        template <typename Component>
        void PopComponent();

    public:
        explicit inline operator bool() const { return m_Handle != entt::null; }
        explicit inline operator uint32_t() const { return static_cast<uint32_t>(m_Handle); }
        explicit inline operator entt::entity() const { return m_Handle; }

        bool operator !=(const Entity& other) const;
        bool operator ==(const Entity& other) const;

    public:
        static const Entity   Null;

    private:
        entt::entity m_Handle;
        Scene*       m_Scene;

    }; // class Entity

} // namespace Ziben

#include "Entity.inl"