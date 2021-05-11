#pragma once

#include <entt/entt.hpp>

#include "Ziben/Window/TimeStep.hpp"
#include "Ziben/Window/Event.hpp"

namespace Ziben {

    class Entity;
    class EditorCamera;

    class Scene {
    public:
        friend class Entity;
        friend class SceneSerializer;
        friend class SceneHierarchyPanel;

    private:
        template <typename Component>
        void OnComponentPushed(entt::registry& registry, entt::entity handle);

    public:
        explicit Scene(std::string name);
        virtual ~Scene() = default;

    public:
        void OnUpdateEditor(const TimeStep& ts, EditorCamera& camera);
        void OnRenderEditor(EditorCamera& camera);

        void OnUpdateRuntime(const TimeStep& ts);
        void OnRenderRuntime();

        void OnViewportResize(uint32_t width, uint32_t height);

        Entity CreateEntity(const std::string& tag = "EnTT");
        void DestroyEntity(const Entity& entity);

        Entity GetPrimaryCameraEntity();

    private:
        std::string    m_Name;
        uint32_t       m_ViewportWidth;
        uint32_t       m_ViewportHeight;
        entt::registry m_Registry;

    }; // class Scene

} // namespace Ziben