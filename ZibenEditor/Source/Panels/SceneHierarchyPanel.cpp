#include "SceneHierarchyPanel.hpp"

#include <imgui.h>

#include <Ziben/Scene/Component.hpp>

namespace Ziben {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : m_Scene(scene) {

    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) {
        m_Scene = scene;
    }

    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");

        {
            m_Scene->m_Registry.each([this](const auto& handle) {
                Entity entity(handle, m_Scene.get());

                DrawEntityNode(entity);
            });
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(const Entity& entity) {
        auto& tag = entity.GetComponent<TagComponent>().GetTag();

        ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        bool isOpened = ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str());

        if (ImGui::IsItemClicked()) {
            m_SelectedEntity = entity;
        }

        if (isOpened) {
            ImGui::Text("Fuck this shit");
            ImGui::TreePop();
        }
    }

} // namespace Ziben