#pragma once

namespace Ziben {

    class Event;

    class Scene {
    public:
        explicit Scene(std::string name);
        virtual ~Scene();

        virtual void OnEvent(Event& event) {}
        virtual void OnUpdate(float dt) {}
        virtual void OnRender() const {}
        virtual void OnImGuiRender() {}

    private:
        std::string m_Name;

    }; // class Scene

} // namespace Ziben