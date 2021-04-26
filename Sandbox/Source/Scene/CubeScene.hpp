#pragma once

#include <Ziben/Utility/Reference.hpp>
#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Renderer/PerspectiveCamera.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>

class CubeScene : public Ziben::Scene {
public:
    CubeScene();

    void OnEvent(Ziben::Event& event) override;
    void OnUpdate(const Ziben::TimeStep& timeStep) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    Ziben::Ref<Ziben::VertexArray> m_VertexArray;
    Ziben::Ref<Ziben::Shader>      m_Shader;
    Ziben::PerspectiveCamera       m_Camera;

    glm::vec3                      m_Color;

}; // class Scene2D