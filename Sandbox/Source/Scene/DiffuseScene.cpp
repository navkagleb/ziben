#include "DiffuseScene.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Window/MouseEvent.hpp>

#include <imgui.h>

DiffuseScene::DiffuseScene()
    : Ziben::Scene("DiffuseScene")
    , m_Torus(0.4f, 0.2f, 30, 100)
    , m_Shader(Ziben::Shader::Create("Assets/Shaders/Diffuse.glsl")) {

    glEnable(GL_DEPTH_TEST);

    Ziben::Shader::Bind(m_Shader);

    m_Angles = glm::vec3(0.0f, 35.0f, 0.0f);
    m_Model  = glm::mat4(1.0f);
    m_Model  = glm::rotate(m_Model, glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_View = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.2f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_Projection = glm::mat4(1.0f);
    m_LightPosition = glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);
    m_Kd            = glm::vec3(0.9f, 0.5f, 0.3f);
    m_Ld            = glm::vec3(1.0f, 1.0f, 1.0f);

    m_Shader->SetUniform("u_Kd", m_Kd);
    m_Shader->SetUniform("u_Ld", m_Ld);
    m_Shader->SetUniform("u_LightPosition", m_View * m_LightPosition);

    m_Projection = glm::perspective(glm::radians(70.0f), (float)1280/720, 0.3f, 100.0f);
}

void DiffuseScene::OnEvent(Ziben::Event& event) {
    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::WindowFocusedEvent>([this](Ziben::WindowFocusedEvent& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::WindowClosedEvent>([this](Ziben::WindowClosedEvent& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::MouseButtonPressedEvent>([this](auto& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });
}

void DiffuseScene::OnUpdate(const Ziben::TimeStep& ts) {
    if (m_Angles.x >= 360.0f)
        m_Angles.x = 0.0f;
    m_Angles.x += 10.0f * (float)ts;

    if (m_Angles.y >= 360.0f)
        m_Angles.y = 0.0f;
    m_Angles.y += 30.0f * (float)ts;

    m_Model  = glm::mat4(1.0f);
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void DiffuseScene::OnRender() {
    Ziben::Shader::Bind(m_Shader);

    glm::mat4 modelView = m_View * m_Model;

    m_Shader->SetUniform("u_ModelViewMatrix", modelView);
    m_Shader->SetUniform(
        "u_NormalMatrix",
        glm::mat3(
            glm::vec3(modelView[0]),
            glm::vec3(modelView[1]),
            glm::vec3(modelView[2])
        )
    );

    m_Shader->SetUniform("u_Kd", m_Kd);
    m_Shader->SetUniform("u_Ld", m_Ld);
    m_Shader->SetUniform("u_LightPosition", m_View * m_LightPosition);

    m_Shader->SetUniform("u_MVP", m_Projection * modelView);

    m_Torus.OnRender();
}

void DiffuseScene::OnImGuiRender() {
    ImGui::Begin("Diffuse Scene");

    {
        ImGui::SliderFloat4("LightPosition", &m_LightPosition[0], -100, 100);
        ImGui::SliderFloat3("Kd", &m_Kd[0], 0, 10);
        ImGui::SliderFloat3("Ld", &m_Ld[0], 0, 10);
        ImGui::SliderFloat3("Rotation", &m_Angles[0], 0, 360);
    }

    ImGui::End();
}