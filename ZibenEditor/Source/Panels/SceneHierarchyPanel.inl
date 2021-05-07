namespace Ziben {

    template <typename Component, typename Function>
    void SceneHierarchyPanel::DrawComponent(const char* name, const Function& function) {
        constexpr auto  treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
        const     auto* id            = reinterpret_cast<void*>(typeid(Component).hash_code());

        if (bool isDeleted = false; m_SelectedEntity.HasComponent<Component>()) {
            if (ImGui::TreeNodeEx(id, treeNodeFlags, name)) {
                if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
                    ImGui::OpenPopup("ComponentSettings");

                if (ImGui::BeginPopup("ComponentSettings")) {
                    if (ImGui::MenuItem("Delete Component"))
                        isDeleted = true;

                    ImGui::EndPopup();
                }

                function();

                ImGui::TreePop();
            }

            if (isDeleted)
                m_SelectedEntity.PopComponent<Component>();
        }
    }

} // namespace Ziben