#pragma once

#include "Ziben/Window/TimeStep.hpp"
#include "Ziben/Window/Event.hpp"

namespace Ziben {

    class Scene {
    public:
        explicit Scene(std::string name);
        virtual ~Scene() = default;

        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(const TimeStep& timeStep) {}
        virtual void OnRender() const {}
        virtual void OnImGuiRender() {}

    private:
        std::string m_Name;

    }; // class Scene

} // namespace Ziben