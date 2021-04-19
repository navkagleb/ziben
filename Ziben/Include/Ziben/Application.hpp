#pragma once

#include "Ziben/System/Window.hpp"
#include "Ziben/Scene/SceneManager.hpp"

namespace Ziben {

    class LayerStack;
    class ImGuiLayer;

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
        void OnEvent(Event& event);

    private:
        Window*             m_Window;
        SceneManager*       m_SceneManager;
        LayerStack*         m_LayerStack;
        ImGuiLayer*         m_ImGuiLayer;

    }; // class Application

    // Must be defined in Client!
    Ziben::Application* CreateApplication(int argc, char** argv);

} // namespace Ziben

// Entry Point!
int main(int argc, char** argv);