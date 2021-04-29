#include "Ziben/Application.hpp"

#include "Ziben/System/Log.hpp"
#include "Ziben/Scene/ImGuiLayer.hpp"
#include "Ziben/Renderer/Renderer.hpp"
#include "Ziben/Window/EventDispatcher.hpp"

#include "Utility/Profiling/ProfilingEngine.hpp"

namespace Ziben {

    Application* Application::s_Instance = nullptr;

    Application::Application(std::string title, int width, int height)
        : m_Window(CreateScope<Window>(std::move(title), width, height))
        , m_SceneManager(CreateScope<SceneManager>())
        , m_LayerStack(CreateScope<LayerStack>())
        , m_ImGuiLayer(new ImGuiLayer)
        , m_IsMinimized(false) {

        if (s_Instance)
            ZIBEN_CORE_ERROR("Application::s_Instance is not nullptr!");

        s_Instance = this;

        m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });

        Renderer::Init();

        m_LayerStack->PushOverlay(m_ImGuiLayer);
    }

    void Application::Run() {
        while (m_Window->IsOpen()) {
            m_TimeStep.Update(static_cast<float>(glfwGetTime()));

            if (!m_IsMinimized)
                for (Layer* layer : *m_LayerStack)
                    layer->OnUpdate(m_TimeStep);

            ImGuiLayer::Begin();

            if (m_SceneManager->HasActiveScene()) {
                m_SceneManager->GetActiveScene()->OnUpdate(m_TimeStep);
                m_SceneManager->GetActiveScene()->OnRender();
                m_SceneManager->GetActiveScene()->OnImGuiRender();
            }

            for (Layer* layer : *m_LayerStack)
                layer->OnImGuiRender();

            ImGuiLayer::End();

            m_Window->OnUpdate();
        }
    }

    void Application::PushLayer(Layer* layer) {
        m_LayerStack->PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer) {
        m_LayerStack->PushOverlay(layer);
    }

    void Application::PopLayer(Layer* layer) {
        m_LayerStack->PopLayer(layer);
    }

    void Application::PopOverlay(Layer* layer) {
        m_LayerStack->PopOverlay(layer);
    }

    void Application::OnEvent(Event& event) {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowClosedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowClosed));
        dispatcher.Dispatch<WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
        dispatcher.Dispatch<WindowMinimizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowMinimized));

        if (m_SceneManager->HasActiveScene())
            m_SceneManager->GetActiveScene()->OnEvent(event);


        for (auto it = m_LayerStack->ReverseBegin(); it != m_LayerStack->ReverseEnd(); ++it) {
            if (event.IsHandled())
                break;

            (*it)->OnEvent(event);
        }
    }

    bool Application::OnWindowClosed(WindowClosedEvent& event) {
        return true;
    }

    bool Application::OnWindowResized(WindowResizedEvent& event) {
        m_IsMinimized = event.GetWidth() == 0 || event.GetHeight() == 0;

        Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

        return false;
    }

    bool Application::OnWindowMinimized(WindowMinimizedEvent& event) {
        m_IsMinimized = !m_IsMinimized;
        return false;
    }
}

// Entry Point!
int main(int argc, char** argv) {
    Ziben::Log::Create();

#ifdef ZIBEN_PROFILING
    Ziben::Profiling::ProfilingEngine::Create();
#endif

    ZIBEN_CORE_INFO("Ziben Core Log!");
    ZIBEN_INFO("Ziben Client Log!");

    auto application = ::Ziben::CreateApplication(argc, argv);
    application->Run();

    delete application;

#ifdef ZIBEN_PROFILING
    Ziben::Profiling::ProfilingEngine::Destroy();
#endif

    Ziben::Log::Destroy();
}