namespace Ziben {
 

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