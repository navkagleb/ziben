#include "Sandbox2D.hpp"

#include <imgui.h>

#include <glm/gtc/type_ptr.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Renderer/RenderCommand.hpp>
#include <Ziben/Renderer/Renderer2D.hpp>

#include "Application/SandboxApplication.hpp"

Sandbox2D::Sandbox2D()
    : Ziben::Layer("Sandbox2D")
    , m_CameraController(1280.0f / 720.0f)
    , m_SquareColor(0.2f, 0.3f, 0.8f, 1.0f)
    , m_SquareAngle(0.0f)
    , m_ColorDirection(1.0f) {}

void Sandbox2D::OnAttach() {
    m_Texture = Ziben::Texture2D::Create("Assets/Textures/CheckerBoard.png");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnEvent(Ziben::Event& event) {
    m_CameraController.OnEvent(event);
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

        Ziben::RenderCommand::SetClearColor({ 0.11f, 0.11f, 0.11f, 0.5f });
        Ziben::RenderCommand::Clear();
    }

    {
        ZIBEN_PROFILE_SCOPE("Sandbox Render Draw");

        Ziben::Renderer2D::BeginScene(m_CameraController.GetCamera());

        Ziben::Renderer2D::DrawQuad({ -1.0f, 0.0f }, { 0.8f, 0.8f }, m_SquareColor);
        Ziben::Renderer2D::DrawQuad({ 0.5f, -0.5f }, { 0.5f, 0.8f }, { 0.8f, 0.4f, 0.3f, 1.0f });
        Ziben::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, { 10.0f, 10.0f }, m_Texture, 10.0f);
        Ziben::Renderer2D::DrawRotatedQuad({ 0.5f, 0.3f }, { 0.3, 0.3f }, glm::radians(45.0f), { 0.2f, 0.8f, 0.3f, 0.6f });
        Ziben::Renderer2D::DrawRotatedQuad({ -2.0f, 0.0f, 0.1f }, { 1.0f, 1.0f }, glm::radians(m_SquareAngle), m_Texture, 20.0f);

        for (int i = -10; i < 10; ++i) {
            for (int j = -10; j < 10; ++j) {
                glm::vec2 position = { static_cast<float>(j) / 2.0f + 0.25f, static_cast<float>(i) / 2.0f + 0.25f };
                glm::vec4 color = { static_cast<float>(j + 5) / 10.0f, 0.4f, static_cast<float>(i + 5) / 10.0f, 0.7f };
                Ziben::Renderer2D::DrawQuad(position, { 0.45f, 0.45f }, color);
            }
        }

        Ziben::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender() {
    ZIBEN_PROFILE_FUNCTION();

    ImGui::Begin("Scene2D");

    {

        const auto& statistics = Ziben::Renderer2D::GetStatistics();

        ImGui::Text("Renderer2D Statistics: ");
        ImGui::Text("Draw Calls: %d",   statistics.DrawCalls);
        ImGui::Text("Quad Count: %d",   statistics.QuadCount);
        ImGui::Text("Vertex Count: %d", statistics.QuadCount * 4);
        ImGui::Text("Index Count: %d",  statistics.QuadCount * 6);

        ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
    }

    ImGui::End();
}