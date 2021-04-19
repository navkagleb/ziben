#pragma once

#include "Ziben/Core/TimeStep.hpp"

namespace Ziben {

    class Event;

    class Layer {
    public:
        explicit Layer(std::string name = "Layer");
        virtual ~Layer() = default;

        virtual void OnEvent(Event& event) {}
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnImGuiRender() {}

    protected:
        std::string m_Name;

    }; // class Layer

} // namespace Ziben