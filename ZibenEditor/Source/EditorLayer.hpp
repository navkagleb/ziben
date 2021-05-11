#pragma once

#include <glm/glm.hpp>

#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Scene/Entity.hpp>

#include "Panels/SceneHierarchyPanel.hpp"

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

        void NewScene();
        void OpenScene();
        void SaveSceneAs();

    private:
        Ref<FrameBuffer>             m_FrameBuffer;
        Ref<Scene>                   m_ActiveScene;

        Entity                       m_Square;
        Entity                       m_Rect;
        Entity                       m_CameraA;
        Entity                       m_CameraB;

        glm::vec<2, uint32_t>        m_ViewportSize;
        bool                         m_IsViewportFocused;
        bool                         m_IsViewportHovered;

        int                          m_GuizmoType;

        SceneHierarchyPanel          m_SceneHierarchyPanel;

    }; // class EditorLayer

} // namespace Ziben