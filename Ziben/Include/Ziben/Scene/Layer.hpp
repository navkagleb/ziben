#pragma once

namespace Ziben {

    class Event;

    class Layer {
    public:
        explicit Layer(std::string name = "Layer");
        virtual ~Layer() = default;

        virtual void OnEvent(Event& event) {}
        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnImGuiRender() {}

    protected:
        std::string m_Name;

    }; // class Layer

} // namespace Ziben