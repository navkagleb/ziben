#pragma once

#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Renderer/OrthographicCamera.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>

class Scene2D : public Ziben::Scene {
public:
    Scene2D();

    void OnEvent(Ziben::Event& event) override;
    void OnUpdate(const Ziben::TimeStep& timeStep) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    Ziben::VertexBuffer*      m_VertexBuffer;
    Ziben::IndexBuffer*       m_IndexBuffer;
    Ziben::VertexArray*       m_VertexArray;
    Ziben::Shader*            m_Shader;
    Ziben::OrthographicCamera m_Camera;
    glm::vec3                 m_CameraPosition;

}; // class Scene2D