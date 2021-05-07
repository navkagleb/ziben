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

        m_FrameBuffer = FrameBuffer::Create(specification);
        m_ActiveScene = CreateRef<Scene>("ActiveScene");

        m_Square = m_ActiveScene->CreateEntity("Square");
        m_Square.PushComponent<SpriteRendererComponent>(glm::vec4(0.2f, 0.3f, 0.7f, 1.0f));

        m_Rect = m_ActiveScene->CreateEntity("Rect");
        m_Rect.PushComponent<SpriteRendererComponent>(glm::vec4(0.3f, 0.8f, 0.4f, 1.0f));

        auto& rectTransformComponent = m_Rect.GetComponent<TransformComponent>();

        rectTransformComponent.SetX(1.5f);
        rectTransformComponent.SetScaleY(3.0f);

        m_Camera = m_ActiveScene->CreateEntity("Camera");
        m_Camera.PushComponent<CameraComponent>(true);

        m_ClipSpaceCamera = m_ActiveScene->CreateEntity("ClipSpace Camera");
        m_ClipSpaceCamera.PushComponent<CameraComponent>(false);

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

        m_Camera.PushComponent<NativeScriptComponent>().Bind<CameraController>();
    }

    void EditorLayer::OnDetach() {

    }

    void EditorLayer::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressedEvent>(ZIBEN_BIND_EVENT_FUNC(OnKeyPressed));
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
            ))
        {
            m_FrameBuffer->Resize(m_ViewportSize.x, m_ViewportSize.y);

            m_ActiveScene->OnViewportResize(m_ViewportSize.x, m_ViewportSize.y);
        }

        m_ActiveScene->OnUpdate(ts);

        // Render
        Renderer2D::ResetStatistics();
        FrameBuffer::Bind(m_FrameBuffer);
        RenderCommand::SetClearColor({ 0.16f, 0.16f, 0.16f, 0.9f });
        RenderCommand::Clear();

        m_ActiveScene->OnRender();

        FrameBuffer::Unbind();

        m_SceneHierarchyPanel.SetScene(m_ActiveScene);
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
            ImVec2 viewportSize = ImGui::GetContentRegionAvail();

            m_ViewportIsFocused = ImGui::IsWindowFocused();
            m_ViewportIsHovered = ImGui::IsWindowHovered();
            m_ViewportSize      = { viewportSize.x, viewportSize.y };

            ZibenEditor::Get().BlockImGuiEvents(!m_ViewportIsFocused || !m_ViewportIsHovered);

            ImGui::Image((void*)(uint64_t)m_FrameBuffer->GetColorAttachmentHandle(), viewportSize, ImVec2(0, 1), ImVec2(1, 0));
        }

        ImGui::End();
        ImGui::PopStyleVar();

        ImGui::End();
    }

    bool EditorLayer::OnKeyPressed(KeyPressedEvent& event) {

        return true;
    }

    bool EditorLayer::OnWindowMinimized(WindowMinimizedEvent& event) {
        m_ViewportSize = glm::vec2(0.0f);

        return true;
    }

} // namespace Ziben