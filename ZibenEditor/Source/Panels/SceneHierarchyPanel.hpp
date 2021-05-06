#pragma once

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
        void SetScene(const Ref<Scene>& scene);
        void OnImGuiRender();

    private:
       void DrawEntityNode(const Entity& entity);
       void DrawComponents();

    private:
        Ref<Scene> m_Scene;
        Entity     m_SelectedEntity;

    }; // class SceneHierarchyPanel

} // namespace Ziben