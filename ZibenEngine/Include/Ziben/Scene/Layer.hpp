#pragma once

#include "Ziben/Window/Event.hpp"
#include "Ziben/Window/TimeStep.hpp"

namespace Ziben {

    class Layer {
    public:
        explicit Layer(std::string name = "Layer");
        virtual ~Layer() noexcept = default;

        [[nodiscard]] const std::string& GetName() const { return m_Name; }

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(const TimeStep& ts) {}
        virtual void OnRender() {}
        virtual void OnImGuiRender() {}

    protected:
        std::string m_Name;

    }; // class Layers

} // namespace Ziben