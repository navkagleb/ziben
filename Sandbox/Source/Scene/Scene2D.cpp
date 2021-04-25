#include "Scene2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Renderer/Renderer.hpp>

#include "Application/SandboxApplication.hpp"

Scene2D::Scene2D()
    : Ziben::Scene("Scene2D")
    , m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    , m_Shader(Ziben::Shader::Create())
    , m_Position(0.0f) {

    float trianglePositions[] = {
        -0.5f, -0.5f, 0.0f,
         1.0f,  0.0f, 0.0f,

         0.0f,  0.5f, 0.0f,
         0.0f,  1.0f, 0.0f,

         0.5f, -0.5f, 0.0f,
         0.0f,  0.0f, 1.0f
    };

    Ziben::IndexType triangleIndices[] = { 0, 1, 2 };

    float squarePositions[] = {
        -0.5f, -0.5f, 0.0f,
         0.2f,  0.3f, 0.8f,

        -0.5f,  0.5f, 0.0f,
         0.2f,  0.3f, 0.8f,

         0.5f,  0.5f, 0.0f,
         0.2f,  0.3f, 0.8f,

         0.5f, -0.5,  0.0f,
         0.2f,  0.3f, 0.8f,
    };

    Ziben::IndexType squareIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    m_Shader->Compile("Media/Basic.vert");
    m_Shader->Compile("Media/Basic.frag");

    Ziben::Shader::Bind(*m_Shader);

    auto* triangleVertexBuffer = Ziben::VertexBuffer::Create(trianglePositions, 3 * 2 * 3 * sizeof(float));
    triangleVertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" },
        { Ziben::ShaderData::Type::Float3, "VertexColor" },
    });

    auto* triangleIndexBuffer = Ziben::IndexBuffer::Create(triangleIndices, 3);

    m_TriangleVertexArray = Ziben::VertexArray::Create();
    m_TriangleVertexArray->PushVertexBuffer(triangleVertexBuffer);
    m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

    auto* squareVertexBuffer = Ziben::VertexBuffer::Create(squarePositions, 4 * 2 * 3 * sizeof(float));
    squareVertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" },
        { Ziben::ShaderData::Type::Float3, "VertexColor" },
    });

    auto* squareIndexBuffer = Ziben::IndexBuffer::Create(squareIndices, 6);

    m_SquareVertexArray = Ziben::VertexArray::Create();
    m_SquareVertexArray->PushVertexBuffer(squareVertexBuffer);
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);
}

void Scene2D::OnEvent(Ziben::Event& event) {
    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::KeyPressedEvent>([&](Ziben::KeyPressedEvent& event) {
        auto& window = SandboxApplication::Get().GetWindow();

        if (event.GetKeyCode() == Ziben::Key::R)
            window.SetVerticalSync(!window.IsVerticalSync());

        return true;
    });
}

void Scene2D::OnUpdate(const Ziben::TimeStep& ts) {
    // Camera
    if (Ziben::Input::IsKeyPressed({ Ziben::Key::A, Ziben::Key::Left }))
        m_Camera.MoveLeft(2.0f * (float)ts);
    else if (Ziben::Input::IsKeyPressed({ Ziben::Key::D, Ziben::Key::Right }))
        m_Camera.MoveRight(2.0f * (float)ts);

    if (Ziben::Input::IsKeyPressed({ Ziben::Key::S, Ziben::Key::Down }))
        m_Camera.MoveDown(2.0f * (float)ts);
    else if (Ziben::Input::IsKeyPressed({ Ziben::Key::W, Ziben::Key::Up }))
        m_Camera.MoveUp(2.0f * (float)ts);

    if (Ziben::Input::IsKeyPressed(Ziben::Key::Q))
        m_Camera.RotateLeft(50.0f * (float)ts);
    else if (Ziben::Input::IsKeyPressed(Ziben::Key::E))
        m_Camera.RotateRight(50.0f * (float)ts);

    // Triangle
    if (Ziben::Input::IsKeyPressed(Ziben::Key::J))
        m_Position.x -= 2.5f * (float)ts;
    else if (Ziben::Input::IsKeyPressed(Ziben::Key::L))
        m_Position.x += 2.5f * (float)ts;

    if (Ziben::Input::IsKeyPressed(Ziben::Key::K))
        m_Position.y -= 2.5f * (float)ts;
    else if (Ziben::Input::IsKeyPressed(Ziben::Key::I))
        m_Position.y += 2.5f * (float)ts;
}

void Scene2D::OnRender() {
    Ziben::Renderer::BeginScene(m_Camera);

    {
        static glm::mat4 scale     = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 40; ++j) {
                glm::vec3 position  = { static_cast<float>(i) * 0.11f, static_cast<float>(j) * 0.11f, 0.0f };
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;

                Ziben::Renderer::Submit(m_Shader, m_SquareVertexArray, transform);
            }
        }

        Ziben::Renderer::Submit(m_Shader, m_TriangleVertexArray);
    }

    Ziben::Renderer::EndScene();
}

void Scene2D::OnImGuiRender() {

}