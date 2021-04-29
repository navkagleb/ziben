#pragma once

#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCameraController.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>
#include <Ziben/Renderer/Texture.hpp>

class Sandbox2D : public Ziben::Layer {
public:
    Sandbox2D();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Ziben::Event& event) override;
    void OnUpdate(const Ziben::TimeStep& ts) override;
    void OnImGuiRender() override;

private:
    Ziben::OrthographicCameraController m_CameraController;
    Ziben::Ref<Ziben::Texture2D>        m_Texture;

    glm::vec4                           m_SquareColor;

}; // class Layer2D;