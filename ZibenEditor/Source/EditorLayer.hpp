#pragma once

#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCameraController.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>
#include <Ziben/Renderer/Texture.hpp>
#include <Ziben/Renderer/SubTexture2D.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>
#include <Ziben/Window/KeyEvent.hpp>

namespace Ziben {

    class EditorLayer : public Layer {
    public:
        EditorLayer();

        void OnAttach() override;
        void OnDetach() override;
        void OnEvent(Event& event) override;
        void OnUpdate(const TimeStep& ts) override;
        void OnImGuiRender() override;

    private:
        bool OnKeyPressed(KeyPressedEvent& event);
        bool OnWindowMinimized(WindowMinimizedEvent& event);

    private:
        OrthographicCameraController m_CameraController;
        Ref<FrameBuffer>             m_FrameBuffer;
        Ref<Texture2D>               m_CheckerBoardTexture;
        Ref<Texture2D>               m_SpriteSheetTexture;
        Ref<SubTexture2D>            m_Tree;

        glm::vec4                    m_SquareColor;
        float                        m_SquareAngle;

        glm::vec2                    m_ViewportSize;
        bool                         m_ViewportIsFocused;
        bool                         m_ViewportIsHovered;

    }; // class EditorLayer

} // namespace Ziben