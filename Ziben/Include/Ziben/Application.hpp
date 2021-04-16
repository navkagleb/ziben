#pragma once

#include "Ziben/System/Window.hpp"

namespace Ziben {

    class Application {
    public:
        static Application& Get() { return *s_Instance; }

    public:
        Application(std::string title, int width, int height);
        virtual ~Application();

        [[nodiscard]] const Window& GetWindow() const { return *m_Window; }

        void Run();

    private:
        static Application* s_Instance;

    protected:
        Window* m_Window;

    }; // class Application

} // namespace Ziben

// To be defined in Client!
Ziben::Application* CreateApplication();