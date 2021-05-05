#include "EditorLayer.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>
#include <Ziben/Scene/Component.hpp>

#include "ZibenEditor.hpp"

namespace Ziben {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
        , m_CameraController(1280.0f / 720.0f)
        , m_IsClipSpaceCamera(false)
        , m_ViewportSize(0.0f)
        , m_ViewportIsFocused(false)
        , m_ViewportIsHovered(false) {}

    void EditorLayer::OnAttach() {
        ZIBEN_PROFILE_FUNCTION();

        // Init FrameBuffer
        FrameBufferSpecification specification;

        specification.Width  = 1280;
        specification.Height = 720;

        m_FrameBuffer                = FrameBuffer::Create(specification);

        // Init Texture
        m_CheckerBoardTexture        = Texture2D::Create("Assets/Textures/CheckerBoard.png");
        m_SpriteSheetTexture         = Texture2D::Create("Assets/Textures/SpriteSheet.png");
        m_Tree                       = SubTexture2D::CreateFromCoords(m_SpriteSheetTexture, { 0, 1 }, { 128, 128 }, { 1, 2 });

        m_ActiveScene = CreateRef<Scene>("ActiveScene");

        m_Square = m_ActiveScene->CreateEntity("Square");
        m_Square.PushComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.7f, 1.0f));

        m_Camera = m_ActiveScene->CreateEntity("Camera");
        m_Camera.PushComponent<CameraComponent>(glm::ortho(-16.0f, 16.0f, -9.0f, 9.0f, -1.0f, 1.0f), true);

        m_ClipSpaceCamera = m_ActiveScene->CreateEntity("ClipSpace Camera");
        m_ClipSpaceCamera.PushComponent<CameraComponent>(glm::ortho(-1.0f, 1.0f, -1.0f, 1.0f, -1.0f, 1.0f), false);
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnEvent(Event& event) {
        m_CameraController.OnEvent(event);

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(ZIBEN_BIND_EVENT_FUNC(OnKeyPressed));
        dispatcher.Dispatch<WindowMinimizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowMinimized));
    }

    void EditorLayer::OnUpdate(const TimeStep& ts) {
        ZIBEN_PROFILE_FUNCTION();

        // Update
        if (m_ViewportIsFocused)
            m_CameraController.OnUpdate(ts);

        // Render
        Renderer2D::ResetStatistics();
        FrameBuffer::Bind(m_FrameBuffer);
        RenderCommand::SetClearColor({ 0.11f, 0.11f, 0.11f, 0.5f });
        RenderCommand::Clear();

        m_ActiveScene->OnRender();

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

        if (!opt_padding)
            ImGui::PopStyleVar();

        if (opt_fullscreen)
            ImGui::PopStyleVar(2);

        // DockSpace
        ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable) {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
        }

        if (ImGui::BeginMenuBar()) {
            if (ImGui::BeginMenu("File")) {
                if (ImGui::MenuItem("Exit", ""))
                    ZibenEditor::Get().Close();

                ImGui::EndMenu();
            }

            ImGui::EndMenuBar();
        }

        ImGui::Begin("Settings");

        {
            const auto& statistics = Renderer2D::GetStatistics();

            ImGui::Text("Renderer2D Statistics: ");
            ImGui::Text("Draw Calls: %d",   statistics.DrawCalls);
            ImGui::Text("Quad Count: %d",   statistics.QuadCount);
            ImGui::Text("Vertex Count: %d", statistics.QuadCount * 4);
            ImGui::Text("Index Count: %d",  statistics.QuadCount * 6);

            if (m_Square) {
                ImGui::Separator();
                ImGui::Text("%s", m_Square.GetComponent<TagComponent>().GetTag().c_str());
                ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_Square.GetComponent<SpriteRendererComponent>().GetColor()));
                ImGui::Separator();
            }

            ImGui::DragFloat3("Camera Position", glm::value_ptr((glm::mat4&)m_Camera.GetComponent<TransformComponent>().GetTransform()[3]));

            if (ImGui::Checkbox("ClipSpace Camera", &m_IsClipSpaceCamera)) {
                m_Camera.GetComponent<CameraComponent>().SetPrimary(!m_IsClipSpaceCamera);
                m_ClipSpaceCamera.GetComponent<CameraComponent>().SetPrimary(m_IsClipSpaceCamera);
            }
        }

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin("Viewport");

        {
            m_ViewportIsFocused = ImGui::IsWindowFocused();
            m_ViewportIsHovered = ImGui::IsWindowHovered();

            ZibenEditor::Get().BlockImGuiEvents(!m_ViewportIsFocused || !m_ViewportIsHovered);

            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            if (m_ViewportSize != *(glm::vec2*)&viewportSize && viewportSize.x > 0 && viewportSize.y > 0) {
                m_ViewportSize = { viewportSize.x, viewportSize.y };

                m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));
                m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            }

            uintmax_t textureHandle = m_FrameBuffer->GetColorAttachmentHandle();
            ImGui::Image((void*)textureHandle, viewportSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {
        if (event.GetKeyCode() == Key::V) {
            auto& window = ZibenEditor::Get().GetWindow();

            window.SetVerticalSync(!window.IsVerticalSync());
        }

        return true;
    }

    bool EditorLayer::OnWindowMinimized(WindowMinimizedEvent& event) {
        m_ViewportSize = glm::vec2(0.0f);

        return true;
    }

} // namespace Ziben