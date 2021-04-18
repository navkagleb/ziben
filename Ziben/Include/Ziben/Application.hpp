#pragma once

#include "Ziben/System/Window.hpp"
#include "Ziben/Scene/SceneManager.hpp"

namespace Ziben {

    class Application {
    public:
        static Application& Get() { return *s_Instance; }

    public:
        Application(std::string title, int width, int height);
        virtual ~Application();

        [[nodiscard]] inline Window& GetWindow() { return *m_Window; }
        [[nodiscard]] inline SceneManager& GetSceneManager() { return *m_SceneManager; }

        void Run();

    private:
        static Application* s_Instance;

    private:
        Window*             m_Window;
        SceneManager*       m_SceneManager;

    }; // class Application

} // namespace Ziben

// Must be defined in Client!
Ziben::Application* CreateApplication(int argc, char** argv);

// Entry Point!
int main(int argc, char** argv);