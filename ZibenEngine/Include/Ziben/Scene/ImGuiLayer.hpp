#pragma once

#include <imgui.h>

#include "Layer.hpp"

namespace Ziben {

    class ImGuiLayer : public Layer {
    public:
        ImGuiLayer();
        ~ImGuiLayer() override = default;

        void OnEvent(Event& event) override;
        void OnAttach() override;
        void OnDetach() override;

        static void Begin();
        static void End();

        void BlockEvents(bool isBlockedEvents);

    private:
        bool m_IsBlockedEvents;

    }; // class ImGuiLayer

} // namespace Ziben