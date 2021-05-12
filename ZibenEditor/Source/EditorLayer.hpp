#pragma once

#include <glm/glm.hpp>

#include <Ziben/Scene/Layer.hpp>
#include <Ziben/Renderer/FrameBuffer.hpp>
#include <Ziben/Window/KeyEvent.hpp>
#include <Ziben/Window/WindowEvent.hpp>
#include <Ziben/Window/MouseEvent.hpp>
#include <Ziben/Scene/Scene.hpp>
#include <Ziben/Scene/Entity.hpp>
#include <Ziben/Renderer/EditorCamera.hpp>

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
        bool OnMouseButtonPressed(MouseButtonPressedEvent& event);
        bool OnWindowMinimized(WindowMinimizedEvent& event);

        bool CanPick();

        void NewScene();
        void OpenScene();
        void SaveSceneAs();

    private:
        Ref<FrameBuffer>             m_FrameBuffer;
        Ref<Scene>                   m_ActiveScene;

        Entity                       m_HoveredEntity;

        EditorCamera                 m_EditorCamera;

        glm::vec<2, uint32_t>        m_ViewportSize;
        std::array<glm::vec2, 2>     m_ViewportBounds;
        bool                         m_IsViewportFocused;
        bool                         m_IsViewportHovered;

        int                          m_GuizmoType;

        SceneHierarchyPanel          m_SceneHierarchyPanel;

    }; // class EditorLayer

} // namespace Ziben