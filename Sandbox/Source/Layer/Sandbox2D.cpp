#include "Sandbox2D.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application/SandboxApplication.hpp"

Sandbox2D::Sandbox2D()
    : Ziben::Layer("Sandbox2D")
    , m_CameraController(1280.0f / 720.0f)
    , m_SquareColor(0.2f, 0.3f, 0.8f, 0.9f)
    , m_SquareAngle(0.0f)
    , m_ColorDirection(1.0f)
    , m_ParticleSystem(5'000) {}

void Sandbox2D::OnAttach() {
    ZIBEN_PROFILE_FUNCTION();

    // Init FrameBuffer
    Ziben::FrameBufferSpecification specification;

    specification.Width  = 1280;
    specification.Height = 720;

    m_FrameBuffer                = Ziben::FrameBuffer::Create(specification);

    // Init Texture
    m_CheckerBoardTexture        = Ziben::Texture2D::Create("Assets/Textures/CheckerBoard.png");
    m_SpriteSheetTexture         = Ziben::Texture2D::Create("Assets/Textures/SpriteSheet.png");
    m_Tree                       = Ziben::SubTexture2D::CreateFromCoords(m_SpriteSheetTexture, { 0, 1 }, { 128, 128 }, { 1, 2 });

    // Init Particle
    m_Particle.ColorBegin        = { 254 / 255.0f, 212 / 255.0f, 123 / 255.0f, 1.0f };
    m_Particle.ColorEnd          = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
    m_Particle.SizeBegin         = 0.5f, m_Particle.SizeVariation = 0.3f, m_Particle.SizeEnd = 0.0f;
    m_Particle.LifeTime          = 1.5f;
    m_Particle.Velocity          = { 0.0f, 0.0f };
    m_Particle.VelocityVariation = { 3.0f, 1.0f };
    m_Particle.Position          = { 0.0f, 0.0f };

    m_CameraController.SetZoomLevel(5.0f);
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnEvent(Ziben::Event& event) {
    m_CameraController.OnEvent(event);

    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::KeyPressedEvent>([&](Ziben::KeyPressedEvent& event) {
        auto& window = SandboxApplication::Get().GetWindow();

        if (event.GetKeyCode() == Ziben::Key::V) {
            window.SetVerticalSync(!window.IsVerticalSync());
        }

        return true;
    });
}

void Sandbox2D::OnUpdate(const Ziben::TimeStep& ts) {
    ZIBEN_PROFILE_FUNCTION();

    // Update
    m_CameraController.OnUpdate(ts);
    m_SquareAngle += 40.0f * (float)ts;

    m_SquareColor.g += m_ColorDirection * 0.3f * (float)ts;

    if (m_SquareColor.g > 1.0f || m_SquareColor.g < 0.0f)
        m_ColorDirection *= -1.0f;

    // Render
    Ziben::Renderer2D::ResetStatistics();

    {
        ZIBEN_PROFILE_SCOPE("Sandbox Renderer Prepare");

        Ziben::FrameBuffer::Bind(m_FrameBuffer);
        Ziben::RenderCommand::SetClearColor({ 0.11f, 0.11f, 0.11f, 0.5f });
        Ziben::RenderCommand::Clear();
    }

    {
        ZIBEN_PROFILE_SCOPE("Sandbox Render Draw");

        Ziben::Renderer2D::BeginScene(m_CameraController.GetCamera());

        // Background
        Ziben::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_CheckerBoardTexture, 10.0f);

        Ziben::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
        Ziben::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.8f }, { 0.8f, 0.4f, 0.3f, 1.0f });
        Ziben::Renderer2D::DrawRotatedQuad({ 0.5f, 0.3f }, { 0.3, 0.3f }, glm::radians(45.0f), { 0.2f, 0.8f, 0.3f, 0.6f });
        Ziben::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(m_SquareAngle), m_CheckerBoardTexture, 20.0f);

        for (int i = -10; i < 10; ++i) {
            for (int j = -10; j < 10; ++j) {
                glm::vec2 position = { static_cast<float>(j) / 2.0f + 0.25f, static_cast<float>(i) / 2.0f + 0.25f };
                glm::vec4 color = { static_cast<float>(j + 5) / 10.0f, 0.4f, static_cast<float>(i + 5) / 10.0f, 0.7f };
                Ziben::Renderer2D::DrawQuad(position, { 0.45f, 0.45f }, color);
            }
        }

        Ziben::Renderer2D::DrawQuad({ -2.0f, 2.0f, 0.1f }, { 1.0f, 2.0f }, m_Tree);

        Ziben::Renderer2D::EndScene();
    }

    {
        ZIBEN_PROFILE_SCOPE("Sandbox ParticleSystem");

        if (Ziben::Input::IsButtonPressed(Ziben::Button::Left))
        {
            auto position       = Ziben::Input::GetMousePosition<float>();
            auto width          = SandboxApplication::Get().GetWindow().GetWidth();
            auto height         = SandboxApplication::Get().GetWindow().GetHeight();
            auto bounds         = m_CameraController.GetBounds();
            auto cameraPosition = m_CameraController.GetCamera().GetPosition();

            position.x = position.x / static_cast<float>(width) * bounds.GetWidth() - bounds.GetWidth() * 0.5f;
            position.y = bounds.GetHeight() * 0.5f - (position.y / static_cast<float>(height)) * bounds.GetHeight();

            m_Particle.Position = { position.x + cameraPosition.x, position.y + cameraPosition.y };

            for (int i = 0; i < 6; ++i)
                m_ParticleSystem.Push(m_Particle);
        }

        m_ParticleSystem.OnUpdate(ts);
        m_ParticleSystem.OnRender(m_CameraController.GetCamera());

        Ziben::FrameBuffer::Unbind();
    }
}

void Sandbox2D::OnImGuiRender() {
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
                SandboxApplication::Get().Close();

            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Begin("Scene2D");

    {
        const auto& statistics = Ziben::Renderer2D::GetStatistics();

        ImGui::Text("Renderer2D Statistics: ");
        ImGui::Text("Draw Calls: %d",   statistics.DrawCalls);
        ImGui::Text("Quad Count: %d",   statistics.QuadCount);
        ImGui::Text("Vertex Count: %d", statistics.QuadCount * 4);
        ImGui::Text("Index Count: %d",  statistics.QuadCount * 6);

        ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
        ImGui::ColorEdit4("ParticleBeginColor", glm::value_ptr(m_Particle.ColorBegin));
        ImGui::ColorEdit4("ParticleEndColor", glm::value_ptr(m_Particle.ColorEnd));

        uintmax_t textureHandle = m_FrameBuffer->GetColorAttachmentHandle();
        ImGui::Image((void*)textureHandle, ImVec2(1280.0f, 720.0f));
    }

    ImGui::End();

    ImGui::End();
}