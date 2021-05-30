#pragma once

#include "Ziben/Window/Window.hpp"
#include "Ziben/Window/TimeStep.hpp"
#include "Ziben/Window/WindowEvent.hpp"
#include "Ziben/Scene/SceneManager.hpp"
#include "Ziben/Scene/LayerStack.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace Ziben {

    class ImGuiLayer;

    class Application {
    public:
        static Application& Get() { return *s_Instance; }

    protected:
        Application(std::string title, int width, int height);

    public:
        virtual ~Application();

        [[nodiscard]] inline Window& GetWindow() { return *m_Window; }
        [[nodiscard]] inline SceneManager& GetSceneManager() { return *m_SceneManager; }
        [[nodiscard]] inline LayerStack& GetLayerStack() { return *m_LayerStack; }

        void Run();
        void Close();

        void BlockImGuiEvents(bool blockImGuiEvents);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* layer);
        void PopLayer(Layer* layer);
        void PopOverlay(Layer* layer);

    private:
        static Application* s_Instance;

    private:
        void OnEvent(Event& event);
        bool OnWindowClosed(WindowClosedEvent& event);
        bool OnWindowResized(WindowResizedEvent& event);
        bool OnWindowMinimized(WindowMinimizedEvent& event);


    private:
        Scope<Window>       m_Window;
        Scope<SceneManager> m_SceneManager;
        Scope<LayerStack>   m_LayerStack;
        ImGuiLayer*         m_ImGuiLayer;
        TimeStep            m_TimeStep;
        bool                m_IsMinimized;

    }; // class Application

    // Must be defined in Client!
    Ziben::Application* CreateApplication(int argc, char** argv);

} // namespace Ziben

// Entry Point!
int main(int argc, char** argv);