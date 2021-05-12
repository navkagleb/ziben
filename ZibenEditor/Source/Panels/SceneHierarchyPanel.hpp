#pragma once

#include <imgui.h>

#include <Ziben/Utility/Reference.hpp>
#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Scene/Entity.hpp>

namespace Ziben {

    class SceneHierarchyPanel {
    public:
        SceneHierarchyPanel() = default;
        explicit SceneHierarchyPanel(const Ref<Scene>& scene);
        ~SceneHierarchyPanel() = default;

    public:
        [[nodiscard]] inline const Entity& GetSelectedEntity() const { return m_SelectedEntity; }

        void SetSelectedEntity(const Entity& entity);

        void SetScene(const Ref<Scene>& scene);
        void OnImGuiRender();

    private:
       void DrawEntityNode(const Entity& entity);
       void DrawComponents();

        template <typename Component, typename Function>
        void DrawComponent(const char* name, bool canBeDeleted, const Function& function);

    private:
        Ref<Scene> m_Scene;
        Entity     m_SelectedEntity;

    }; // class SceneHierarchyPanel

} // namespace Ziben

#include "SceneHierarchyPanel.inl"