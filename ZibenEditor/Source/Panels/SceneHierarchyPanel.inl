namespace Ziben {

    template <typename Component, typename Function>
    void SceneHierarchyPanel::DrawComponent(const char* name, bool canBeDeleted, const Function& function) {
        const auto*        id            = reinterpret_cast<void*>(typeid(Component).hash_code());
        ImGuiTreeNodeFlags treeNodeFlags = 0;

        treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;
        treeNodeFlags |= ImGuiTreeNodeFlags_Framed;
        treeNodeFlags |= ImGuiTreeNodeFlags_FramePadding;
        treeNodeFlags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        if (bool isDeleted = false; m_SelectedEntity.HasComponent<Component>()) {
            auto& component = m_SelectedEntity.GetComponent<Component>();

            ImGui::Separator();

            if (ImGui::TreeNodeEx(id, treeNodeFlags, name)) {
                if (canBeDeleted && ImGui::IsItemClicked(ImGuiMouseButton_Right))
                    ImGui::OpenPopup("ComponentSettings");

                if (ImGui::BeginPopup("ComponentSettings")) {
                    if (ImGui::MenuItem("Delete Component"))
                        isDeleted = true;

                    ImGui::EndPopup();
                }

                function(component);

                ImGui::TreePop();
            }

            if (isDeleted)
                m_SelectedEntity.PopComponent<Component>();
        }
    }

} // namespace Ziben