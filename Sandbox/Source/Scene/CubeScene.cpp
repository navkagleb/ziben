#include "CubeScene.hpp"

#include <imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/Input.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Window/MouseEvent.hpp>
#include <Ziben/Renderer/Renderer.hpp>

#include "Application/SandboxApplication.hpp"

static bool firstMouse = true;
static float lastX = 0.0f;
static float lastY = 0.0f;

CubeScene::CubeScene()
    : Ziben::Scene("CubeScene")
    , m_Camera(45.0f, (float)SandboxApplication::Get().GetWindow().GetWidth() / (float)SandboxApplication::Get().GetWindow().GetHeight(), 0.03f, 100.0f)
    , m_Shader(Ziben::Shader::Create())
    , m_Color(0.2f, 0.3f, 0.8f){

    glEnable(GL_DEPTH_TEST);

    m_Camera.SetPosition({ 0.0f, 0.0f, 4.0f });
    m_Camera.SetFront({ 0.0f, 0.0f, -1.0f });

    float positions[] = {
        // front
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,

        // back
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0
    };

    Ziben::IndexType indices[] = {
        // front
        0, 1, 2,
        2, 3, 0,

        // right
        1, 5, 6,
        6, 2, 1,

        // back
        7, 6, 5,
        5, 4, 7,

        // left
        4, 0, 3,
        3, 7, 4,

        // bottom
        4, 5, 1,
        1, 0, 4,

        // top
        3, 2, 6,
        6, 7, 3
    };

    m_Shader->Compile("Media/Basic.vert");
    m_Shader->Compile("Media/Basic.frag");

    Ziben::Shader::Bind(m_Shader);

    auto triangleVertexBuffer = Ziben::VertexBuffer::Create(positions, 3 * 4 * 2 * sizeof(float));
    triangleVertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" }
    });

    auto triangleIndexBuffer = Ziben::IndexBuffer::Create(indices, 3 * 2 * 6);

    m_VertexArray = Ziben::VertexArray::Create();
    m_VertexArray->PushVertexBuffer(triangleVertexBuffer);
    m_VertexArray->SetIndexBuffer(triangleIndexBuffer);
}

void CubeScene::OnEvent(Ziben::Event& event) {
    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::KeyPressedEvent>([&](Ziben::KeyPressedEvent& event) {
        auto& window = SandboxApplication::Get().GetWindow();

        if (event.GetKeyCode() == Ziben::Key::R)
            window.SetVerticalSync(!window.IsVerticalSync());

        return true;
    });

    dispatcher.Dispatch<Ziben::MouseScrolledEvent>([&](Ziben::MouseScrolledEvent& event) {
        ZIBEN_INFO("{0}", event.GetOffsetY());

        if (m_Camera.GetFov() >= 1.0f && m_Camera.GetFov() <= 45.0f)
            m_Camera.SetFov(m_Camera.GetFov() - (float)event.GetOffsetY());

        if (m_Camera.GetFov() <= 1.0f)
            m_Camera.SetFov(1.0f);

        if (m_Camera.GetFov() >= 45.0f)
            m_Camera.SetFov(45.0f);

        ZIBEN_WARN("{0}", m_Camera.GetFov());

        return true;
    });

    dispatcher.Dispatch<Ziben::MouseMovedEvent>([&](Ziben::MouseMovedEvent& event) {
        if (firstMouse) {
            lastX = event.GetX();
            lastY = event.GetY();
            firstMouse = false;
        }

        float xoffset = event.GetX() - lastX;
        float yoffset = event.GetY() - lastY;

        lastX = event.GetX();
        lastY = event.GetY();

        float sensitivity = 0.1f;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        m_Camera.SetYaw(m_Camera.GetYaw() + xoffset);
        m_Camera.SetPitch(m_Camera.GetPitch() + yoffset);

        if (m_Camera.GetPitch() > 89.0f)
            m_Camera.SetPitch(89.0f);

        if (m_Camera.GetPitch() < -89.0f)
            m_Camera.SetPitch(-89.0f);

        m_Camera.SetFront(glm::normalize(glm::vec3(
            glm::cos(glm::radians(m_Camera.GetYaw())) * glm::cos(glm::radians(m_Camera.GetPitch())),
            glm::sin(glm::radians(m_Camera.GetPitch())),
            glm::sin(glm::radians(m_Camera.GetYaw())) * glm::cos(glm::radians(m_Camera.GetPitch()))
        )));

        return true;
    });
}

void CubeScene::OnUpdate(const Ziben::TimeStep& ts) {
    // Camera
    if (Ziben::Input::IsKeyPressed({ Ziben::Key::A, Ziben::Key::Left }))
        m_Camera.MoveLeft(2.0f * (float)ts);
    else if (Ziben::Input::IsKeyPressed({ Ziben::Key::D, Ziben::Key::Right }))
        m_Camera.MoveRight(2.0f * (float)ts);

    if (Ziben::Input::IsKeyPressed({ Ziben::Key::S, Ziben::Key::Down }))
        m_Camera.MoveDown(2.0f * (float)ts);
    else if (Ziben::Input::IsKeyPressed({ Ziben::Key::W, Ziben::Key::Up }))
        m_Camera.MoveUp(2.0f * (float)ts);

//    m_Camera.SetPosition({ glm::sin(glfwGetTime()) * 4.0f, glm::sin(glfwGetTime()), glm::cos(glfwGetTime()) * 4.0f });
}

void CubeScene::OnRender() {
    Ziben::Renderer::BeginScene(m_Camera);

    {
        Ziben::Shader::Bind(m_Shader);
        m_Shader->SetUniform("u_Color", m_Color);
        Ziben::Renderer::Submit(m_Shader, m_VertexArray);
    }

    Ziben::Renderer::EndScene();
}

void CubeScene::OnImGuiRender() {
    ImGui::Begin("CubeScene");

    {
        ImGui::ColorEdit3("SquareColor", glm::value_ptr(m_Color));
    }

    ImGui::End();
}