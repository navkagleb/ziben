#include "Application.hpp"

#include "Ziben/System/Log.hpp"

namespace Ziben {

    Application* Application::s_Instance = nullptr;

    Application::Application(std::string title, int width, int height)
        : m_Window(new Window(std::move(title), width, height)) {

        if (s_Instance)
            ZIBEN_CORE_ERROR("Application::s_Instance is not nullptr!");

        s_Instance = this;
    }

    Application::~Application() {
        delete m_Window;
    }

    void Application::Run() {
        while (m_Window->IsOpen()) {
            m_Window->OnEventUpdate();

            m_Window->SwapBuffers();
        }
    }
}