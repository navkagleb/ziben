#include "ADSScene.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <Ziben/System/Log.hpp>
#include <Ziben/Window/EventDispatcher.hpp>
#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Window/MouseEvent.hpp>

#include <imgui.h>

ADSScene::ADSScene()
    : Ziben::Scene("ADSScene")
    , m_Shader(Ziben::Shader::Create())
    , m_Torus(0.7f, 0.3f, 150, 150)
    , m_Angle(0.0f) {

    glEnable(GL_DEPTH_TEST);

    m_Shader->Compile("Media/ADS.vert");
    m_Shader->Compile("Media/ADS.frag");

    Ziben::Shader::Bind(*m_Shader);

    m_View = glm::lookAt(
        glm::vec3(0.0f, 0.0f, 2.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f)
    );

    m_Projection = glm::mat4(1.0f);
    m_WorldLight = glm::vec4(5.0f, 5.0f, 2.0f, 1.0f);

    m_Light.Position     = m_View * m_WorldLight;
    m_Light.La           = glm::vec3(0.4f, 0.4f, 0.4f);
    m_Light.Ld           = glm::vec3(1.0f, 1.0f, 1.0f);
    m_Light.Ls           = glm::vec3(1.0f, 1.0f, 1.0f);

    m_Material.Ka        = glm::vec3(0.9f, 0.5f, 0.3f);
    m_Material.Kd        = glm::vec3(0.9f, 0.5f, 0.3f);
    m_Material.Ks        = glm::vec3(0.8f, 0.8f, 0.8f);
    m_Material.Shininess = 100.0f;

    m_Shader->SetUniform("u_Light.Position",     m_Light.Position);
    m_Shader->SetUniform("u_Light.La",           m_Light.La);
    m_Shader->SetUniform("u_Light.Ld",           m_Light.Ld);
    m_Shader->SetUniform("u_Light.Ls",           m_Light.Ls);
    m_Shader->SetUniform("u_Material.Ka",        m_Material.Ka);
    m_Shader->SetUniform("u_Material.Kd",        m_Material.Kd);
    m_Shader->SetUniform("u_Material.Ks",        m_Material.Ks);
    m_Shader->SetUniform("u_Material.Shininess", m_Material.Shininess);

    m_Angles = glm::vec3(0.0f, 35.0f, 0.0f);
    m_Model  = glm::mat4(1.0f);
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angle), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(-35.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(35.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    m_Projection = glm::perspective(glm::radians(70.0f), (float)1280 / 720, 0.3f, 100.0f);
}

void ADSScene::OnEvent(Ziben::Event& event) {
    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::WindowFocusedEvent>([this](Ziben::WindowFocusedEvent& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::WindowCloseEvent>([this](Ziben::WindowCloseEvent& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::MouseButtonPressedEvent>([this](auto& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });
}

void ADSScene::OnUpdate(const Ziben::TimeStep& timeStep) {
    m_Light.Position     = m_View * m_WorldLight;

    if (m_Angles.x >= 360.0f)
        m_Angles.x = 0.0f;
    m_Angles.x += 10.0f * (float)timeStep;

    if (m_Angles.y >= 360.0f)
        m_Angles.y = 0.0f;
    m_Angles.y += 30.0f * (float)timeStep;

    m_Model  = glm::mat4(1.0f);
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angles.x), glm::vec3(1.0f, 0.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angles.y), glm::vec3(0.0f, 1.0f, 0.0f));
    m_Model  = glm::rotate(m_Model, glm::radians(m_Angles.z), glm::vec3(0.0f, 0.0f, 1.0f));
}

void ADSScene::OnRender() const {
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

    m_Shader->SetUniform("u_MVP", m_Projection * modelView);

    m_Shader->SetUniform("u_Light.Position",     m_Light.Position);
    m_Shader->SetUniform("u_Light.La",           m_Light.La);
    m_Shader->SetUniform("u_Light.Ld",           m_Light.Ld);
    m_Shader->SetUniform("u_Light.Ls",           m_Light.Ls);
    m_Shader->SetUniform("u_Material.Ka",        m_Material.Ka);
    m_Shader->SetUniform("u_Material.Kd",        m_Material.Kd);
    m_Shader->SetUniform("u_Material.Ks",        m_Material.Ks);
    m_Shader->SetUniform("u_Material.Shininess", m_Material.Shininess);

    m_Torus.OnRender();
}

void ADSScene::OnImGuiRender() {
    ImGui::Begin("ADSScene Scene");

    {
        ImGui::SliderFloat4("WorldLight", &m_WorldLight[0], 0, 20);
        ImGui::NewLine();
        ImGui::SliderFloat3("Light.La", &m_Light.La[0], 0.0f, 1.0f);
        ImGui::SliderFloat3("Light.Ld", &m_Light.Ld[0], 0.0f, 1.0f);
        ImGui::SliderFloat3("Light.Ls", &m_Light.Ls[0], 0.0f, 1.0f);
        ImGui::NewLine();
        ImGui::SliderFloat3("Material.Ka", &m_Material.Ka[0], 0.0f, 1.0f);
        ImGui::SliderFloat3("Material.Kd", &m_Material.Kd[0], 0.0f, 1.0f);
        ImGui::SliderFloat3("Material.Ks", &m_Material.Ks[0], 0.0f, 1.0f);
        ImGui::SliderFloat("Material.Shininess", &m_Material.Shininess, 0.0f, 200.0f);
    }

    ImGui::End();
}