#include "EditorLayer.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "ZibenEditor.hpp"

namespace Ziben {

    EditorLayer::EditorLayer()
        : Layer("EditorLayer")
        , m_CameraController(1280.0f / 720.0f)
        , m_SquareColor(0.2f, 0.3f, 0.8f, 0.9f)
        , m_SquareAngle(0.0f)
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

        m_CameraController.SetZoomLevel(5.0f);
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnEvent(Event& event) {
        m_CameraController.OnEvent(event);

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>([&](KeyPressedEvent& event) {
            auto& window = ZibenEditor::Get().GetWindow();

            if (event.GetKeyCode() == Key::V) {
                window.SetVerticalSync(!window.IsVerticalSync());
            }

            return true;
        });
    }

    void EditorLayer::OnUpdate(const TimeStep& ts) {
        ZIBEN_PROFILE_FUNCTION();

        // Update
        if (m_ViewportIsFocused)
            m_CameraController.OnUpdate(ts);

        m_SquareAngle += 40.0f * (float)ts;

        // Render
        Renderer2D::ResetStatistics();

        {
            ZIBEN_PROFILE_SCOPE("Sandbox Renderer Prepare");

            FrameBuffer::Bind(m_FrameBuffer);
            RenderCommand::SetClearColor({ 0.11f, 0.11f, 0.11f, 0.5f });
            RenderCommand::Clear();
        }

        {
            ZIBEN_PROFILE_SCOPE("Sandbox Render Draw");

            Renderer2D::BeginScene(m_CameraController.GetCamera());

            // Background
            Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerBoardTexture, 10.0f);

            Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
            Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.8f }, { 0.8f, 0.4f, 0.3f, 1.0f });
            Renderer2D::DrawRotatedQuad({ 0.5f, 0.3f }, { 0.3, 0.3f }, glm::radians(45.0f), { 0.2f, 0.8f, 0.3f, 0.6f });
            Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(m_SquareAngle), m_CheckerBoardTexture, 20.0f);

            for (int i = -10; i < 10; ++i) {
                for (int j = -10; j < 10; ++j) {
                    glm::vec2 position = { static_cast<float>(j) / 2.0f + 0.25f, static_cast<float>(i) / 2.0f + 0.25f };
                    glm::vec4 color = { static_cast<float>(j + 5) / 10.0f, 0.4f, static_cast<float>(i + 5) / 10.0f, 0.7f };
                    Renderer2D::DrawQuad(position, { 0.45f, 0.45f }, color);
                }
            }

            Renderer2D::DrawQuad({ -2.0f, 2.0f, 0.1f }, { 1.0f, 2.0f }, m_Tree);

            Renderer2D::EndScene();
            FrameBuffer::Unbind();
        }
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

            ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
        }

        ImGui::End();

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, { 0.0f, 0.0f });
        ImGui::Begin("Viewport");

        {
            m_ViewportIsFocused = ImGui::IsWindowFocused();
            m_ViewportIsHovered = ImGui::IsWindowHovered();

            ZibenEditor::Get().BlockImGuiEvents(!m_ViewportIsFocused || !m_ViewportIsHovered);

            ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

            if (m_ViewportSize != *(glm::vec2*)&viewportPanelSize) {
                m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };
                m_FrameBuffer->Resize(static_cast<uint32_t>(m_ViewportSize.x), static_cast<uint32_t>(m_ViewportSize.y));

                m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);
            }

            uintmax_t textureHandle = m_FrameBuffer->GetColorAttachmentHandle();
            ImGui::Image((void*)textureHandle, viewportPanelSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

} // namespace Ziben