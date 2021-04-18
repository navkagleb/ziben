#include "Application.hpp"

#include "Ziben/System/Log.hpp"

namespace Ziben {

    Application* Application::s_Instance = nullptr;

    Application::Application(std::string title, int width, int height)
        : m_Window(new Window(std::move(title), width, height))
        , m_SceneManager(new SceneManager) {

        if (s_Instance)
            ZIBEN_CORE_ERROR("Application::s_Instance is not nullptr!");

        s_Instance = this;
    }

    Application::~Application() {
        delete m_Window;
        delete m_SceneManager;
    }

    void Application::Run() {
        while (m_Window->IsOpen()) {
            m_Window->OnEventUpdate();

            m_SceneManager->GetActiveScene()->OnUpdate(0.0f);
            m_SceneManager->GetActiveScene()->OnRender();
            m_SceneManager->GetActiveScene()->OnImGuiRender();

            m_Window->SwapBuffers();
        }
    }
}

// Entry Point!
int main(int argc, char** argv) {
    Ziben::Log::Create();

    ZIBEN_CORE_INFO("Ziben Core Log!");
    ZIBEN_INFO("Ziben Client Log!");

    auto application = ::CreateApplication(argc, argv);
    application->Run();

    delete application;

    Ziben::Log::Destroy();
}