#pragma once

namespace Ziben {

    class Scene {
    public:
        explicit Scene(std::string name);
        virtual ~Scene();

        virtual void OnUpdate(float dt) {}
        virtual void OnRender() const {}
        virtual void OnImGuiRender() {}

    private:
        std::string m_Name;

    }; // class Scene

} // namespace Ziben