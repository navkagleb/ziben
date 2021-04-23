#include "Scene2D.hpp"

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Renderer/Renderer.hpp>

Scene2D::Scene2D()
    : Ziben::Scene("Scene2D")
    , m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    , m_Shader(Ziben::Shader::Create())
    , m_CameraPosition(0.0f) {

    float positions[] = {
        -0.5f, -0.5f, 0.0f,
         1.0f,  0.0f, 0.0f,

         0.0f,  0.5f, 0.0f,
         0.0f,  1.0f, 0.0f,

         0.5f, -0.5f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    uint32_t indices[] = { 0, 1, 2 };

    m_Shader->Compile("Media/Basic.vert");
    m_Shader->Compile("Media/Basic.frag");

    Ziben::Shader::Bind(*m_Shader);

    m_VertexBuffer = Ziben::VertexBuffer::Create(positions, 3 * 2 * 3 * sizeof(float));
    m_VertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" },
        { Ziben::ShaderData::Type::Float3, "VertexColor" },
    });

    m_IndexBuffer = Ziben::IndexBuffer::Create(indices, 3);

    m_VertexArray = Ziben::VertexArray::Create();
    m_VertexArray->PushVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}

void Scene2D::OnEvent(Ziben::Event& event) {

}

void Scene2D::OnUpdate(const Ziben::TimeStep& timeStep) {
    if (Ziben::Input::IsKeyPressed({ Ziben::Key::A, Ziben::Key::Left }))
        m_Camera.MoveLeft(2.0f * (float)timeStep);

    if (Ziben::Input::IsKeyPressed({ Ziben::Key::D, Ziben::Key::Right }))
        m_Camera.MoveRight(2.0f * (float)timeStep);

    if (Ziben::Input::IsKeyPressed({ Ziben::Key::S, Ziben::Key::Down }))
        m_Camera.MoveDown(2.0f * (float)timeStep);

    if (Ziben::Input::IsKeyPressed({ Ziben::Key::W, Ziben::Key::Up }))
        m_Camera.MoveUp(2.0f * (float)timeStep);

    if (Ziben::Input::IsButtonPressed(Ziben::Button::Left))
        m_Camera.RotateLeft(20.0f * (float)timeStep);

    if (Ziben::Input::IsButtonPressed(Ziben::Button::Right))
        m_Camera.RotateRight(20.0f * (float)timeStep);
}

void Scene2D::OnRender() {
    Ziben::Renderer::BeginScene(m_Camera);

    Ziben::Renderer::Submit(m_Shader, m_VertexArray);

    Ziben::Renderer::EndScene();
}

void Scene2D::OnImGuiRender() {

}