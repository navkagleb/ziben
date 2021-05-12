#include "SceneHierarchyPanel.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <Ziben/Scene/Component.hpp>

namespace Ziben {

    SceneHierarchyPanel::SceneHierarchyPanel(const Ref<Scene>& scene)
        : m_Scene(scene)
        , m_SelectedEntity(Entity::Null) {

    }

    void SceneHierarchyPanel::SetSelectedEntity(const Entity& entity) {
        m_SelectedEntity = entity;
    }

    void SceneHierarchyPanel::SetScene(const Ref<Scene>& scene) {
        m_Scene          = scene;
        m_SelectedEntity = Entity::Null;
    }

    void SceneHierarchyPanel::OnImGuiRender() {
        ImGui::Begin("Scene Hierarchy");
        {
            if (m_Scene) {
                m_Scene->m_Registry.each([this](const auto& handle) {
                    DrawEntityNode(Entity(handle, m_Scene.get()));
                });

                if (ImGui::IsMouseDown(0) && ImGui::IsWindowHovered())
                    m_SelectedEntity = Entity::Null;

                // Right-click on blank space
                if (ImGui::BeginPopupContextWindow(nullptr, ImGuiMouseButton_Right, false)) {
                    if (ImGui::MenuItem("Create Empty Entity"))
                        m_Scene->CreateEntity("Unnamed");

                    ImGui::EndPopup();
                }
            }
        }
        ImGui::End();

        ImGui::Begin("Properties");
        {
            if (m_SelectedEntity)
                DrawComponents();
        }
        ImGui::End();
    }

    void SceneHierarchyPanel::DrawEntityNode(const Entity& entity) {
        auto& tag = entity.GetComponent<TagComponent>().Tag;

        ImGuiTreeNodeFlags flags = (m_SelectedEntity == entity ? ImGuiTreeNodeFlags_Selected : 0) | ImGuiTreeNodeFlags_OpenOnArrow;

        flags |= ImGuiTreeNodeFlags_SpanAvailWidth;

        if (ImGui::TreeNodeEx((void*)(uint64_t)(uint32_t)entity, flags, "%s", tag.c_str())) {
            ImGui::Text("Fuck this shit");
            ImGui::TreePop();
        }

        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
            m_SelectedEntity = entity;

        bool isEntityDeleted = false;

        if (ImGui::BeginPopupContextItem()) {
            if (ImGui::MenuItem("Delete Entity"))
                isEntityDeleted = true;

            ImGui::EndPopup();
        }

        if (isEntityDeleted) {
            if (m_SelectedEntity == entity)
                m_SelectedEntity = Entity::Null;

            m_Scene->DestroyEntity(entity);
        }
    }

    void SceneHierarchyPanel::DrawComponents() {
        {
            auto& component = m_SelectedEntity.GetComponent<TagComponent>();

            auto& tag = component.Tag;

            char buffer[256] = { 0 };
            strcpy_s(buffer, sizeof(buffer), tag.c_str());

            ImGui::PushItemWidth(ImGui::GetContentRegionAvailWidth() - 150.0f);

            if (ImGui::InputText("##Tag", buffer, sizeof(buffer)))
                tag = buffer;

            ImGui::PopItemWidth();
            ImGui::SameLine();
            ImGui::PushItemWidth(-1.0f);

            if (ImGui::Button("Push Component"))
                ImGui::OpenPopup("PushComponent");

            if (ImGui::BeginPopup("PushComponent")) {
                if (ImGui::MenuItem("Camera")) {
                    m_SelectedEntity.PushComponent<CameraComponent>();
                    ImGui::CloseCurrentPopup();
                }

                if (ImGui::MenuItem("SpriteRendererComponent")) {
                    m_SelectedEntity.PushComponent<SpriteRendererComponent>();
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }

            ImGui::PopItemWidth();
        }

        DrawComponent<TransformComponent>("TransformComponent", false, [&](TransformComponent& component) {
            if (auto translation = component.GetTranslation(); ImGui::DragFloat3("Translation", glm::value_ptr(translation), 0.1f))
                component.SetTranslation(translation);

            if (auto rotation = glm::degrees(component.GetRotation()); ImGui::DragFloat3("Rotation", glm::value_ptr(rotation), 0.1f))
                component.SetRotation(glm::radians(rotation));

            if (auto scale = component.GetScale(); ImGui::DragFloat3("Scale", glm::value_ptr(scale), 0.1f))
                component.SetScale(scale);
        });

        DrawComponent<CameraComponent>("CameraComponent", true, [&](CameraComponent& component) {
            auto& camera = component.Camera;

            ImGui::Checkbox("Primary", &component.IsPrimary);

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

                    if (ImGui::DragFloat("Size", &size, 0.1f))
                        camera.SetOrthographicSize(size);

                    if (ImGui::DragFloat("Near", &near, 0.1f))
                        camera.SetOrthographicNear(near);

                    if (ImGui::DragFloat("Far", &far, 0.1f))
                        camera.SetOrthographicFar(far);

                    ImGui::Checkbox("Fixes Aspect Ratio", &component.HasFixedAspectRatio);

                    break;
                }
            }
        });

        DrawComponent<SpriteRendererComponent>("SpriteRendererComponent", true, [&](SpriteRendererComponent& component) {
            auto& color = component.Color;

            ImGui::ColorEdit4("Color", glm::value_ptr(color));
        });
    }

} // namespace Ziben