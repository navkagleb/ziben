#pragma once

#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCameraController.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>
#include <Ziben/Renderer/Texture.hpp>
#include <Ziben/Renderer/SubTexture2D.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>

#include "Entities/ParticleSystem.hpp"

class Sandbox2D : public Ziben::Layer {
public:
    Sandbox2D();

    void OnAttach() override;
    void OnDetach() override;
    void OnEvent(Ziben::Event& event) override;
    void OnUpdate(const Ziben::TimeStep& ts) override;
    void OnImGuiRender() override;

private:
    Ziben::OrthographicCameraController                       m_CameraController;
    Ziben::Ref<Ziben::Texture2D>                              m_CheckerBoardTexture;
    Ziben::Ref<Ziben::Texture2D>                              m_SpriteSheetTexture;

    Ziben::Ref<Ziben::SubTexture2D>                           m_Bush;
    Ziben::Ref<Ziben::SubTexture2D>                           m_Tree;

    glm::vec4                                                 m_SquareColor;
    float                                                     m_SquareAngle;
    float                                                     m_ColorDirection;

    ParticleSystem                                            m_ParticleSystem;
    ParticleProps                                             m_Particle;

    std::unordered_map<char, Ziben::Ref<Ziben::SubTexture2D>> m_Tiles;

}; // class Layer2D;