#include "SceneHierarchyPanel.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

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

            if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                m_SelectedEntity = Entity::Null;
        }

        ImGui::End();

        ImGui::Begin("Properties");

        {
            if (m_SelectedEntity) {
                DrawComponents();
            }
        }

        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(const Entity& entity) {
        auto& tag = entity.GetComponent<TagComponent>().GetTag();

        ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, tag.c_str())) {
            ImGui::Text("Fuck this shit");
            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked())
            m_SelectedEntity = entity;
    }

    void SceneHierarchyPanel::DrawComponents() {
        if (m_SelectedEntity.HasComponent<TagComponent>()) {
            auto& tag = (std::string&)m_SelectedEntity.GetComponent<TagComponent>();

            char buffer[256] = { 0 };
            strcpy_s(buffer, sizeof(buffer), tag.c_str());

            if (ImGui::InputText("Tag", buffer, sizeof(buffer)))
                tag = buffer;
        }

        if (m_SelectedEntity.HasComponent<TransformComponent>()) {
            if (ImGui::TreeNodeEx((void*)typeid(TransformComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "TransformComponent")) {
                auto& transform = (glm::mat4&)m_SelectedEntity.GetComponent<TransformComponent>();

                ImGui::DragFloat3("Position", glm::value_ptr(transform[3]), 0.1f);
                ImGui::TreePop();
            }
        }

        if (m_SelectedEntity.HasComponent<CameraComponent>()) {
            if (ImGui::TreeNodeEx((void*)typeid(CameraComponent).hash_code(), ImGuiTreeNodeFlags_DefaultOpen, "Camera")) {
                auto& cameraComponent = m_SelectedEntity.GetComponent<CameraComponent>();
                auto& camera          = cameraComponent.GetCamera();

                {
                    bool isPrimary = cameraComponent.IsPrimary();

                    if (ImGui::Checkbox("Primary", &isPrimary))
                        cameraComponent.SetPrimary(isPrimary);
                }

                const char* projectionTypeStrings[] = { "Perspective", "Orthographic" };
                const char* currentProjectionTypeString = projectionTypeStrings[static_cast<int8_t>(camera.GetProjectionType())];

                if (ImGui::BeginCombo("Projection", currentProjectionTypeString)) {
                    for (uint8_t i = 0; i < 2; ++i) {
                        bool isSelected = currentProjectionTypeString == projectionTypeStrings[i];

                        if (ImGui::Selectable(projectionTypeStrings[i], isSelected)) {
                            currentProjectionTypeString = projectionTypeStrings[i];
                            camera.SetProjectionType(static_cast<SceneCamera::ProjectionType>(i));
                        }

                        if (isSelected)
                            ImGui::SetItemDefaultFocus();
                    }

                    ImGui::EndCombo();
                }

                switch (camera.GetProjectionType()) {
                    case SceneCamera::ProjectionType::Perspective: {
                        float fov  = glm::degrees(camera.GetPerspectiveProps().Fov);
                        float near = camera.GetPerspectiveProps().Near;
                        float far  = camera.GetPerspectiveProps().Far;

                        if (ImGui::DragFloat("Fov", &fov, 0.1f))
                            camera.SetPerspectiveFov(glm::radians(fov));

                        if (ImGui::DragFloat("Near", &near, 0.1f))
                            camera.SetPerspectiveNear(near);

                        if (ImGui::DragFloat("Far", &far, 0.1f))
                            camera.SetPerspectiveFar(far);

                        break;
                    }

                    case SceneCamera::ProjectionType::Orthographic: {
                        float size                = camera.GetOrthographicProps().Size;
                        float near                = camera.GetOrthographicProps().Near;
                        float far                 = camera.GetOrthographicProps().Far;
                        bool  hasFixedAspectRatio = cameraComponent.HasFixedAspectRatio();

                        if (ImGui::DragFloat("Size", &size, 0.1f))
                            camera.SetOrthographicSize(size);

                        if (ImGui::DragFloat("Near", &near, 0.1f))
                            camera.SetOrthographicNear(near);

                        if (ImGui::DragFloat("Far", &far, 0.1f))
                            camera.SetOrthographicFar(far);

                        if (ImGui::Checkbox("Fixed Aspect Ratio", &hasFixedAspectRatio))
                            cameraComponent.SetFixedAspectRatio(hasFixedAspectRatio);

                        break;
                    }
                }

                ImGui::TreePop();
            }
        }
    }

} // namespace Ziben