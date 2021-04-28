#pragma once

#include <Ziben/Utility/Reference.hpp>
#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Renderer/OrthographicCameraController.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>
#include <Ziben/Renderer/Texture.hpp>

class Scene2D : public Ziben::Scene {
public:
    Scene2D();

    void OnEvent(Ziben::Event& event) override;
    void OnUpdate(const Ziben::TimeStep& timeStep) override;
    void OnRender() override;
    void OnImGuiRender() override;

private:
    Ziben::Ref<Ziben::VertexArray>      m_TriangleVertexArray;
    Ziben::Ref<Ziben::VertexArray>      m_SquareVertexArray;
    Ziben::Ref<Ziben::Shader>           m_Shader;
    Ziben::Ref<Ziben::Texture2D>        m_CheckerBoardTexture;
    Ziben::Ref<Ziben::Texture2D>        m_ChernoTexture;
    Ziben::OrthographicCameraController m_CameraController;

    glm::vec3                           m_Position;
    glm::vec3                           m_SquareColor;

}; // class Scene2D