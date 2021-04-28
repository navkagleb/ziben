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
    , m_SquareColor(0.2f, 0.3f, 0.8f, 1.0f) {}

void Sandbox2D::OnAttach() {
    float positions[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    Ziben::IndexType indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    auto vertexBuffer = Ziben::VertexBuffer::Create(positions, sizeof(positions ));
    vertexBuffer->SetLayout({
                                { Ziben::ShaderData::Type::Float3, "VertexPosition" }
                            });

    auto indexBuffer = Ziben::IndexBuffer::Create(indices, sizeof(indices) / sizeof(Ziben::HandleType));

    m_VertexArray = Ziben::VertexArray::Create();
    m_VertexArray->PushVertexBuffer(vertexBuffer);
    m_VertexArray->SetIndexBuffer(indexBuffer);

    m_Shader = Ziben::Shader::Create("Assets/Shaders/FlatColorShader.glsl");
}

void Sandbox2D::OnDetach() {

}

void Sandbox2D::OnEvent(Ziben::Event& event) {
    m_CameraController.OnEvent(event);
}

void Sandbox2D::OnUpdate(const Ziben::TimeStep& ts) {
    // Update
    m_CameraController.OnUpdate(ts);

    // Render
    Ziben::RenderCommand::SetClearColor({ 0.11f, 0.11f, 0.11f, 0.5f });
    Ziben::RenderCommand::Clear();

    Ziben::Renderer2D::BeginScene(m_CameraController.GetCamera());

    {
        Ziben::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });
    }

    Ziben::Renderer2D::EndScene();
}

void Sandbox2D::OnImGuiRender() {
    ImGui::Begin("Scene2D");

    {
        ImGui::ColorEdit4("SquareColor", glm::value_ptr(m_SquareColor));
    }

    ImGui::End();
}