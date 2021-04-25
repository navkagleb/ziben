#pragma once

#include "Ziben/Window/Event.hpp"
#include "Ziben/Window/TimeStep.hpp"

namespace Ziben {

    class Layer {
    public:
        explicit Layer(std::string name = "Layer");
        virtual ~Layer() = default;

        virtual void OnEvent(Event& event) {}
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(const TimeStep& ts) {}
        virtual void OnImGuiRender() {}

    protected:
        std::string m_Name;

    }; // class Layer

} // namespace Ziben