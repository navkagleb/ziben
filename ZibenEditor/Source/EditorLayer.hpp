#pragma once

#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/OrthographicCameraController.hpp>
#include <Ziben/Renderer/Shader.hpp>
#include <Ziben/Renderer/VertexArray.hpp>
#include <Ziben/Renderer/Texture.hpp>
#include <Ziben/Renderer/SubTexture2D.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Scene/Entity.hpp>

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
        Ref<Scene>                   m_ActiveScene;

        Entity                       m_Square;
        Entity                       m_Camera;
        Entity                       m_ClipSpaceCamera;
        bool                         m_IsClipSpaceCamera;

        glm::vec<2, uint32_t>        m_ViewportSize;
        bool                         m_ViewportIsFocused;
        bool                         m_ViewportIsHovered;

    }; // class EditorLayer

} // namespace Ziben