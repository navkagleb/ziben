#include "Ziben/Application.hpp"

#include "Ziben/System/Log.hpp"
#include "Ziben/Scene/ImGuiLayer.hpp"

namespace Ziben {

    Application* Application::s_Instance = nullptr;

    Application::Application(std::string title, int width, int height)
        : m_Window(new Window(std::move(title), width, height))
        , m_SceneManager(new SceneManager)
        , m_LayerStack(new LayerStack)
        , m_ImGuiLayer(new ImGuiLayer) {

        if (s_Instance)
            ZIBEN_CORE_ERROR("Application::s_Instance is not nullptr!");

        s_Instance = this;

        m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });
        m_LayerStack->PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        delete m_Window;
        delete m_SceneManager;
    }

    void Application::Run() {
        glClearColor(0.3f, 0.3f, 0.4f, 0.5f);

        while (m_Window->IsOpen()) {
            m_TimeStep.Update(static_cast<float>(glfwGetTime()));

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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

    void Application::OnEvent(Event& event) {
        if (m_SceneManager->HasActiveScene())
            m_SceneManager->GetActiveScene()->OnEvent(event);

        for (auto it = m_LayerStack->ReverseBegin(); it != m_LayerStack->ReverseEnd(); ++it) {
            if (event.IsHandled())
                break;

            (*it)->OnEvent(event);
        }
    }
}

// Entry Point!
int main(int argc, char** argv) {
    Ziben::Log::Create();

    ZIBEN_CORE_INFO("Ziben Core Log!");
    ZIBEN_INFO("Ziben Client Log!");

    auto application = ::Ziben::CreateApplication(argc, argv);
    application->Run();

    delete application;

    Ziben::Log::Destroy();
}