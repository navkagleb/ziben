#include "EditorLayer.hpp"

#include <imgui.h>

#include <ImGuizmo.h>

#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>
#include <Ziben/Scene/Component.hpp>
#include <Ziben/Scene/SceneSerializer.hpp>
#include <Ziben/System/FileDialogs.hpp>

#include "ZibenEditor.hpp"

namespace Ziben {

    static bool DecomposeTransform(
        const glm::mat4& transform,
        glm::vec3& translation,
        glm::vec3& rotation,
        glm::vec3& scale
    ) {
        // From glm::decompose in matrix_decompose.inl

        using namespace glm;
        using T = float;

        mat4 LocalMatrix(transform);

        // Normalize the matrix.
        if (epsilonEqual(LocalMatrix[3][3], static_cast<float>(0), epsilon<T>()))
            return false;

        // First, isolate perspective.  This is the messiest.
        if (epsilonNotEqual(LocalMatrix[0][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[1][3], static_cast<T>(0), epsilon<T>()) ||
            epsilonNotEqual(LocalMatrix[2][3], static_cast<T>(0), epsilon<T>())
        ) {
            // Clear the perspective partition
            LocalMatrix[0][3] = LocalMatrix[1][3] = LocalMatrix[2][3] = static_cast<T>(0);
            LocalMatrix[3][3] = static_cast<T>(1);
        }

        // Next take care of translation (easy).
        translation = vec3(LocalMatrix[3]);
        LocalMatrix[3] = vec4(0, 0, 0, LocalMatrix[3].w);

        vec3 Row[3], Pdum3;

        // Now get scale and shear.
        for (length_t i = 0; i < 3; ++i)
            for (length_t j = 0; j < 3; ++j)
                Row[i][j] = LocalMatrix[i][j];

        // Compute X scale factor and normalize first row.
        scale.x = length(Row[0]);
        Row[0]  = detail::scale(Row[0], static_cast<T>(1));
        scale.y = length(Row[1]);
        Row[1]  = detail::scale(Row[1], static_cast<T>(1));
        scale.z = length(Row[2]);
        Row[2]  = detail::scale(Row[2], static_cast<T>(1));

        // At this point, the matrix (in rows[]) is orthonormal.
        // Check for a coordinate system flip.  If the determinant
        // is -1, then negate the matrix and the scaling factors.
    #if 0
        Pdum3 = cross(Row[1], Row[2]); // v3Cross(row[1], row[2], Pdum3);
        if (dot(Row[0], Pdum3) < 0)
        {
            for (length_t i = 0; i < 3; i++)
            {
                scale[i] *= static_cast<T>(-1);
                Row[i] *= static_cast<T>(-1);
            }
        }
    #endif

        rotation.y = asin(-Row[0][2]);
        if (cos(rotation.y) != 0) {
            rotation.x = atan2(Row[1][2], Row[2][2]);
            rotation.z = atan2(Row[0][1], Row[0][0]);
        } else {
            rotation.x = atan2(-Row[2][0], Row[1][1]);
            rotation.z = 0;
        }

        return true;
    }

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
        , m_EditorCamera(30.0f, 1.778f, 0.1f, 1000.0f)
        , m_ViewportSize(0.0f)
        , m_IsViewportFocused(false)
        , m_IsViewportHovered(false)
        , m_GuizmoType(-1) {}

    void EditorLayer::OnAttach() {
        ZIBEN_PROFILE_FUNCTION();

        // Init FrameBuffer
        FrameBufferSpecification specification;

        specification.Attachments = {
            { FrameBufferTextureFormat::RGBA8      },
            { FrameBufferTextureFormat::RedInteger },
            { FrameBufferTextureFormat::Depth      }
        };

        specification.Width  = 1280;
        specification.Height = 720;

        m_FrameBuffer = FrameBuffer::Create(std::move(specification));
        m_ActiveScene = CreateRef<Scene>("ActiveScene");

#if 0
        m_Square = m_ActiveScene->CreateEntity("Square");
        m_Square.PushComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.7f, 1.0f));

        m_Rect = m_ActiveScene->CreateEntity("Rect");
        m_Rect.PushComponent<SpriteRendererComponent>(glm::vec4(0.3f, 0.8f, 0.4f, 1.0f));

        auto& rectTransformComponent = m_Rect.GetComponent<TransformComponent>();

        rectTransformComponent.SetX(1.5f);
        rectTransformComponent.SetScaleY(3.0f);

        m_CameraA = m_ActiveScene->CreateEntity("Camera A");
        m_CameraA.PushComponent<CameraComponent>().IsPrimary = true;

        m_CameraB = m_ActiveScene->CreateEntity("Camera B");
        m_CameraB.PushComponent<CameraComponent>();

        class CameraController : public ScriptableEntity {
        public:
            void OnUpdate(const TimeStep& ts) override {
                static float speed = 5.0f;

                auto& tc = GetComponent<TransformComponent>();

                if (Input::IsKeyPressed(Key::A))
                    tc.SetX(tc.GetX() - speed * (float)ts);
                if (Input::IsKeyPressed(Key::D))
                    tc.SetX(tc.GetX() + speed * (float)ts);
                if (Input::IsKeyPressed(Key::S))
                    tc.SetY(tc.GetY() - speed * (float)ts);
                if (Input::IsKeyPressed(Key::W))
                    tc.SetY(tc.GetY() + speed * (float)ts);
            }
        };

        m_CameraA.PushComponent<NativeScriptComponent>().Bind<CameraController>();
#endif
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnEvent(Event& event) {
        m_EditorCamera.OnEvent(event);

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(ZIBEN_BIND_EVENT_FUNC(OnKeyPressed));
        dispatcher.Dispatch<MouseButtonPressedEvent>(ZIBEN_BIND_EVENT_FUNC(OnMouseButtonPressed));
        dispatcher.Dispatch<WindowMinimizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowMinimized));
    }

    void EditorLayer::OnUpdate(const TimeStep& ts) {
        ZIBEN_PROFILE_FUNCTION();

        // Resize
        if (auto specification = m_FrameBuffer->GetSpecification();
            m_ViewportSize.x > 0 &&
            m_ViewportSize.y > 0 && (
                specification.Width  != m_ViewportSize.x ||
                specification.Height != m_ViewportSize.y
            )
        ) {
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);
            m_EditorCamera.SetViewportSize(static_cast<float>(m_ViewportSize.x), static_cast<float>(m_ViewportSize.y));
            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        m_EditorCamera.OnUpdate(ts);

        m_ActiveScene->OnUpdateEditor(ts, m_EditorCamera);

        // Render
        Renderer2D::ResetStatistics();
        FrameBuffer::Bind(m_FrameBuffer);
        RenderCommand::SetClearColor({ 0.16f, 0.16f, 0.16f, 0.9f });
        RenderCommand::Clear();

        // Clear entity Id attachment to -1
        m_FrameBuffer->ClearColorAttachment(1, -1);

        m_ActiveScene->OnRenderEditor(m_EditorCamera);

        auto [mouseX, mouseY] = ImGui::GetMousePos();
        mouseX -= m_ViewportBounds[0].x;
        mouseY  = static_cast<float>(m_ViewportSize.y) - (mouseY - m_ViewportBounds[0].y);

        glm::vec2 viewportSize = { m_ViewportBounds[1] - m_ViewportBounds[0] };

        if (mouseX >= 0.0f && mouseY >= 0.0f && mouseX < viewportSize.x && mouseY < viewportSize.y) {
            int pixelData = m_FrameBuffer->ReadPixel(1, static_cast<int>(mouseX), static_cast<int>(mouseY));

            m_HoveredEntity = pixelData == -1 ? Entity::Null : Entity((entt::entity)pixelData, m_ActiveScene.get());
        }

        FrameBuffer::Unbind();
    }

    void EditorLayer::OnImGuiRender() {
        ZIBEN_PROFILE_FUNCTION();

        static bool dockspaceOpen = true;
        static bool opt_fullscreen = true;
        static bool opt_padding = false;
        static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

        // We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
        // because it would be confusing to have two docking targets within each others.
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
        if (opt_fullscreen)
        {
            const ImGuiViewport* viewport = ImGui::GetMainViewport();
            ImGui::SetNextWindowPos(viewport->WorkPos);
            ImGui::SetNextWindowSize(viewport->WorkSize);
            ImGui::SetNextWindowViewport(viewport->ID);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
            ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
            window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
            window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
        }
        else
        {
            dockspace_flags &= ~ImGuiDockNodeFlags_PassthruCentralNode;
        }

        // When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background
        // and handle the pass-thru hole, so we ask Begin() to not render a background.
        if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
            window_flags |= ImGuiWindowFlags_NoBackground;

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        if (!opt_padding)
            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

        ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
        {
            if (!opt_padding)
                ImGui::PopStyleVar();

            if (opt_fullscreen)
                ImGui::PopStyleVar(2);

            // DockSpace
            ImGuiIO& io = ImGui::GetIO();
            ImGuiStyle& style = ImGui::GetStyle();

            float windowMinSizeX = style.WindowMinSize.x;
            style.WindowMinSize.x = 370.0f;

            if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
                ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
                ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
            }

            style.WindowMinSize.x = windowMinSizeX;

            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("File")) {
                    if (ImGui::MenuItem("New", "Ctrl+N"))
                        NewScene();

                    if (ImGui::MenuItem("Open...", "Ctrl+O"))
                        OpenScene();

                    if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S"))
                        SaveSceneAs();

                    if (ImGui::MenuItem("Exit", ""))
                        ZibenEditor::Get().Close();

                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            m_SceneHierarchyPanel.OnImGuiRender();

            ImGui::Begin("Settings");
            {
                const auto& statistics = Renderer2D::GetStatistics();

                ImGui::Text("Editor");

                std::string name = m_HoveredEntity ? m_HoveredEntity.GetComponent<TagComponent>().Tag : "None";
                ImGui::Text("Hovered Entity: %s", name.c_str());

                ImGui::Separator();
                ImGui::Text("Renderer2D Statistics: ");
                ImGui::Text("Draw Calls: %d",   statistics.DrawCalls);
                ImGui::Text("Quad Count: %d",   statistics.QuadCount);
                ImGui::Text("Vertex Count: %d", statistics.QuadCount * 4);
                ImGui::Text("Index Count: %d",  statistics.QuadCount * 6);

                ImGui::Separator();
                ImGui::Text("Application");

                auto& window = ZibenEditor::Get().GetWindow();

                if (bool isVerticalSync = window.IsVerticalSync(); ImGui::Checkbox("IsVerticalSync", &isVerticalSync))
                    ZibenEditor::Get().GetWindow().SetVerticalSync(isVerticalSync);

                ImGui::Text("FrameTime: %0.3f", 1000.0f / ImGui::GetIO().Framerate);
                ImGui::Text("FrameRate: %0.1f", ImGui::GetIO().Framerate);
            }
            ImGui::End();

            ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
            ImGui::Begin("Viewport");
            {
                // Includes tab bar
                auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
                auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
                auto viewportOffset    = ImGui::GetWindowPos();

                m_ViewportBounds[0]    = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
                m_ViewportBounds[1]    = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

                m_IsViewportFocused    = ImGui::IsWindowFocused();
                m_IsViewportHovered    = ImGui::IsWindowHovered();

                ZibenEditor::Get().BlockImGuiEvents(!m_IsViewportFocused && !m_IsViewportHovered);

                auto viewportPanelSize = ImGui::GetContentRegionAvail();
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };

                ImGui::Image(
                    reinterpret_cast<void*>(m_FrameBuffer->GetColorAttachmentHandle(0)),
                    viewportPanelSize,
                    ImVec2(0, 1),
                    ImVec2(1, 0)
                );

                // Gizmos
                if (Entity selectedEntity = m_SceneHierarchyPanel.GetSelectedEntity();
                    selectedEntity && m_GuizmoType != -1
                ) {
                    ImGuizmo::SetOrthographic(false);
                    ImGuizmo::SetDrawlist();

                    ImGuizmo::SetRect(
                        m_ViewportBounds[0].x,
                        m_ViewportBounds[0].y,
                        m_ViewportBounds[1].x - m_ViewportBounds[0].x,
                        m_ViewportBounds[1].y - m_ViewportBounds[0].y
                    );


                    // Editor Camera
                    const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();
                    const glm::mat4& cameraView       = m_EditorCamera.GetViewMatrix();

                    // Runtime Camera
                    // auto        cameraEntity       = m_ActiveScene->GetPrimaryCameraEntity();
                    // const auto& transformComponent = cameraEntity.GetComponent<TransformComponent>();
                    // const auto& cameraComponent    = cameraEntity.GetComponent<CameraComponent>();

                    // Entity
                    auto&     entityTransformComponent = selectedEntity.GetComponent<TransformComponent>();
                    glm::mat4 entityTransform          = entityTransformComponent.GetTransform();

                    // Snapping
                    bool  snap      = Input::IsKeyPressed(Key::LeftControl);
                    float snapValue = 0.0f;

                    switch (m_GuizmoType) {
                        case ImGuizmo::OPERATION::TRANSLATE:
                        case ImGuizmo::OPERATION::SCALE:     snapValue = 0.5f;  break;
                        case ImGuizmo::OPERATION::ROTATE:    snapValue = 45.0f; break;
                        default:                                                break;
                    }

                    float snapValues[3] = { snapValue, snapValue, snapValue };

                    ImGuizmo::Manipulate(
                        glm::value_ptr(cameraView),
                        glm::value_ptr(cameraProjection),
                        static_cast<ImGuizmo::OPERATION>(m_GuizmoType),
                        ImGuizmo::LOCAL,
                        glm::value_ptr(entityTransform),
                        nullptr,
                        snap ? snapValues : nullptr
                    );

                    if (ImGuizmo::IsUsing()) {
                        glm::vec3 translation(0.0f);
                        glm::vec3 rotation(0.0f);
                        glm::vec3 scale(1.0f);

                        DecomposeTransform(entityTransform, translation, rotation, scale);

                        glm::vec3 deltaRotation = rotation - entityTransformComponent.GetRotation();

                        entityTransformComponent.SetTranslation(translation);
                        entityTransformComponent.SetRotation(entityTransformComponent.GetRotation() + deltaRotation);
                        entityTransformComponent.SetScale(scale);
                    }
                }
            }
            ImGui::End();
            ImGui::PopStyleVar();
        }
        ImGui::End();
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
        if (event.GetCount() > 1)
            return false;

        bool control = Input::IsKeyPressed({ Key::LeftControl, Key::RightControl });
        bool shift   = Input::IsKeyPressed({ Key::LeftShift, Key::RightShift });

        switch (event.GetKeyCode()) {
            case Key::N: {
                if (control)
                    NewScene();

                break;
            }

            case Key::O: {
                if (control)
                    OpenScene();

                break;
            }

            case Key::S: {
                if (control && shift)
                    SaveSceneAs();

                break;
            }

            // Gizmos
            case Key::Q: m_GuizmoType = -1;                             break;
            case Key::W: m_GuizmoType = ImGuizmo::OPERATION::TRANSLATE; break;
            case Key::E: m_GuizmoType = ImGuizmo::OPERATION::ROTATE;    break;
            case Key::R: m_GuizmoType = ImGuizmo::OPERATION::SCALE;     break;

            default: break;
        }

        return true;
    }

    bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& event) {
        if (event.GetButtonCode() == Button::Left)
            if (CanPick())
                m_SceneHierarchyPanel.SetSelectedEntity(m_HoveredEntity);

        return false;
    }

    bool EditorLayer::OnWindowMinimized(WindowMinimizedEvent& event) {
        m_ViewportSize = glm::vec2(0.0f);

        return true;
    }

    bool EditorLayer::CanPick() {
        return m_IsViewportHovered && !ImGuizmo::IsOver() && !m_EditorCamera.IsActive();
    }

    void EditorLayer::NewScene() {
        m_ActiveScene = CreateRef<Scene>("New");
        m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

        m_SceneHierarchyPanel.SetScene(m_ActiveScene);
    }

    void EditorLayer::OpenScene() {
        std::string filepath = FileDialogs::OpenFile("Ziben Scene (*.ziben)\0*.ziben\0");

        if (!filepath.empty()) {
            m_ActiveScene = CreateRef<Scene>("New");
            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);

            m_SceneHierarchyPanel.SetScene(m_ActiveScene);

            SceneSerializer serializer(m_ActiveScene);
            serializer.Deserialize(filepath);
        }
    }

    void EditorLayer::SaveSceneAs() {
        std::string filepath = FileDialogs::SaveFile("Ziben Scene (*.ziben)\0*.ziben\0");

        if (!filepath.empty()) {
            SceneSerializer serializer(m_ActiveScene);
            serializer.Serialize(filepath);
        }
    }

} // namespace Ziben