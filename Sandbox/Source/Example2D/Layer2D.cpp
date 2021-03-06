#include "Layer2D.hpp"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include "Ziben/Renderer/RenderCommand.hpp"
#include <Ziben/Renderer/Renderer.hpp>

#include "Application.hpp"

Layer2D::Layer2D()
    : Ziben::Layer("Scene2D")
    , m_CameraController(1280.0f / 720.0f)
    , m_Shader(Ziben::Shader::Create("Assets/Shaders/TextureShader.glsl"))
    , m_SquareColor(0.2f, 0.3f, 0.8f) {

    float trianglePositions[] = {
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
    };

    Ziben::IndexType triangleIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    float squarePositions[] = {
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f,
        0.5f,  0.5f, 0.0f,
        0.5f, -0.5,  0.0f
    };

    Ziben::IndexType squareIndices[] = {
        0, 1, 2,
        2, 3, 0
    };

    Ziben::Shader::Bind(m_Shader);

    auto triangleVertexBuffer = Ziben::VertexBuffer::Create(trianglePositions, sizeof(trianglePositions));
    triangleVertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" },
        { Ziben::ShaderData::Type::Float4, "Color"          },
        { Ziben::ShaderData::Type::Float2, "TexCoord"       },
        { Ziben::ShaderData::Type::Float,  "TexIndex"       },
        { Ziben::ShaderData::Type::Float,  "TilingFactor"   },
        { Ziben::ShaderData::Type::Int,    "EntityHandle"   }
    });

    auto triangleIndexBuffer = Ziben::IndexBuffer::Create(triangleIndices, sizeof(triangleIndices) / sizeof(Ziben::HandleType));

    m_TriangleVertexArray = Ziben::VertexArray::Create();
    m_TriangleVertexArray->PushVertexBuffer(triangleVertexBuffer);
    m_TriangleVertexArray->SetIndexBuffer(triangleIndexBuffer);

    auto squareVertexBuffer = Ziben::VertexBuffer::Create(squarePositions, sizeof(squarePositions ));
    squareVertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" },
        { Ziben::ShaderData::Type::Float4, "Color"          },
        { Ziben::ShaderData::Type::Float2, "TexCoord"       },
        { Ziben::ShaderData::Type::Float,  "TexIndex"       },
        { Ziben::ShaderData::Type::Float,  "TilingFactor"   },
        { Ziben::ShaderData::Type::Int,    "EntityHandle"   }
    });

    auto squareIndexBuffer = Ziben::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(Ziben::HandleType));

    m_SquareVertexArray = Ziben::VertexArray::Create();
    m_SquareVertexArray->PushVertexBuffer(squareVertexBuffer);
    m_SquareVertexArray->SetIndexBuffer(squareIndexBuffer);

    Ziben::Texture2D::Bind(m_CheckerBoardTexture = Ziben::Texture2D::Create("Assets/Textures/CheckerBoard.png"));
    m_Shader->SetUniform("u_Texture", 0);

    Ziben::Texture2D::Bind(m_ChernoTexture = Ziben::Texture2D::Create("Assets/Textures/ChernoLogo.png"));
    m_Shader->SetUniform("u_Texture", 0);
}

void Layer2D::OnEvent(Ziben::Event& event) {
    m_CameraController.OnEvent(event);

    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::KeyPressedEvent>([&](Ziben::KeyPressedEvent& event) {
        auto& window = Sandbox::Application::Get().GetWindow();

        if (event.GetKeyCode() == Ziben::Key::R)
            window.SetVerticalSync(!window.IsVerticalSync());

        return true;
    });
}

void Layer2D::OnUpdate(const Ziben::TimeStep& ts) {
    m_CameraController.OnUpdate(ts);
}

void Layer2D::OnRender() {
    Ziben::RenderCommand::SetClearColor({ 0.11f, 0.11f, 0.11f, 0.5f });
    Ziben::RenderCommand::Clear();

    Ziben::Renderer::BeginScene(m_CameraController.GetCamera());
    {
        static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

        Ziben::Shader::Bind(m_Shader);

        for (int i = 0; i < 40; ++i) {
            for (int j = 0; j < 40; ++j) {
                glm::vec3 position  = { static_cast<float>(i) * 0.11f, static_cast<float>(j) * 0.11f, 0.0f };
                glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) * scale;

                Ziben::Renderer::Submit(m_Shader, m_SquareVertexArray, transform);
            }
        }

        Ziben::Texture2D::Bind(m_CheckerBoardTexture);
        Ziben::Renderer::Submit(m_Shader, m_TriangleVertexArray);

        Ziben::Texture2D::Bind(m_ChernoTexture);
        Ziben::Renderer::Submit(m_Shader, m_TriangleVertexArray);
    }
    Ziben::Renderer::EndScene();
}

void Layer2D::OnImGuiRender() {
    ImGui::Begin("Scene2D");
    {
        ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_SquareColor));
    }
    ImGui::End();
}