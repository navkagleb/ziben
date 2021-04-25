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
    Ziben::VertexArray*       m_TriangleVertexArray;
    Ziben::VertexArray*       m_SquareVertexArray;
    Ziben::Shader*            m_Shader;
    Ziben::OrthographicCamera m_Camera;

    glm::vec3                 m_Position;

}; // class Scene2D