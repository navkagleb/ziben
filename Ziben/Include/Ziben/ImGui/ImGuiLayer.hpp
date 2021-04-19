#pragma once

#include "Ziben/Scene/Layer.hpp"

namespace Ziben {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnEvent(Event& event) override;
        void OnAttach() override;
        void OnDetach() override;
        void OnImGuiRender() override;

        static void Begin();
        static void End();

        void BlockEvents(bool block);

    private:
        bool m_IsBlockedEvents;
    }; // class ImGuiLayer

} // namespace Ziben