#pragma once

#include <entt/entt.hpp>

#include "Ziben/Window/TimeStep.hpp"
#include "Ziben/Window/Event.hpp"

namespace Ziben {

    class Entity;

    class Scene {
    public:
        friend class Entity;

    public:
        explicit Scene(std::string name);
        virtual ~Scene() = default;

    public:
        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(const TimeStep& ts);
        virtual void OnRender();
        virtual void OnImGuiRender() {}
        void OnViewportResize(uint32_t width, uint32_t height);

        Entity CreateEntity(const std::string& tag = "EnTT");

    private:
        std::string    m_Name;
        uint32_t       m_ViewportWidth;
        uint32_t       m_ViewportHeight;
        entt::registry m_Registry;

    }; // class Scene

} // namespace Ziben