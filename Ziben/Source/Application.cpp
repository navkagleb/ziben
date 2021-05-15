#include "Ziben/Application.hpp"

#include "Ziben/Utility/Random.hpp"
#include "Ziben/System/Log.hpp"
#include "Ziben/Scene/ImGuiLayer.hpp"
#include "Ziben/Renderer/Renderer.hpp"
#include "Ziben/Window/EventDispatcher.hpp"

namespace Ziben {

    Application* Application::s_Instance = nullptr;

    Application::Application(std::string title, int width, int height)
        : m_Window(Window::Create(std::move(title), width, height))
        , m_SceneManager(CreateScope<SceneManager>())
        , m_LayerStack(CreateScope<LayerStack>())
        , m_ImGuiLayer(new ImGuiLayer)
        , m_IsMinimized(false) {

        ZIBEN_PROFILE_FUNCTION();

        if (s_Instance)
            ZIBEN_CORE_ERROR("Application::s_Instance is not nullptr!");

        s_Instance = this;

        m_Window->SetEventCallback([this](Event& event) { OnEvent(event); });

        Renderer::Init();

        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application() {
        ZIBEN_PROFILE_FUNCTION();

        Renderer::Shutdown();
    }

    void Application::Run() {
        ZIBEN_PROFILE_FUNCTION();

        while (m_Window->IsOpen()) {
            ZIBEN_PROFILE_SCOPE("RunLoop");

            m_TimeStep.Update(static_cast<float>(glfwGetTime()));

            if (!m_IsMinimized) {
                {
                    ZIBEN_PROFILE_SCOPE("LayerStack OnUpdate");

                    for (Layer *layer : *m_LayerStack) {
                        layer->OnUpdate(m_TimeStep);
                        layer->OnRender();
                    }
                }

                ImGuiLayer::Begin();
                {

    //                if (m_SceneManager->HasActiveScene()) {
    //                    m_SceneManager->GetActiveScene()->OnUpdate(m_TimeStep);
    //                    m_SceneManager->GetActiveScene()->OnRender();
    //                    m_SceneManager->GetActiveScene()->OnImGuiRender();
    //                }

                    {
                        ZIBEN_PROFILE_SCOPE("LayerStack OnImGuiRender");

                        for (Layer *layer : *m_LayerStack)
                            layer->OnImGuiRender();
                    }
                }
                ImGuiLayer::End();
            }

            m_Window->OnUpdate();
        }
    }

    void Application::Close() {
        m_Window->Close();
    }

    void Application::BlockImGuiEvents(bool blockImGuiEvents) {
        m_ImGuiLayer->BlockEvents(blockImGuiEvents);
    }

    void Application::PushLayer(Layer* layer) {
        ZIBEN_PROFILE_FUNCTION();

        m_LayerStack->PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* layer) {
        ZIBEN_PROFILE_FUNCTION();

        m_LayerStack->PushOverlay(layer);
        layer->OnAttach();
    }

    void Application::PopLayer(Layer* layer) {
        ZIBEN_PROFILE_FUNCTION();

        m_LayerStack->PopLayer(layer);
        layer->OnDetach();
    }

    void Application::PopOverlay(Layer* layer) {
        ZIBEN_PROFILE_FUNCTION();

        m_LayerStack->PopOverlay(layer);
        layer->OnDetach();
    }

    void Application::OnEvent(Event& event) {
        ZIBEN_PROFILE_FUNCTION();

        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowClosedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowClosed));
        dispatcher.Dispatch<WindowResizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowResized));
        dispatcher.Dispatch<WindowMinimizedEvent>(ZIBEN_BIND_EVENT_FUNC(OnWindowMinimized));

//        if (m_SceneManager->HasActiveScene())
//            m_SceneManager->GetActiveScene()->OnEvent(event);

        for (auto it = m_LayerStack->ReverseBegin(); it != m_LayerStack->ReverseEnd(); ++it) {
            if (event.IsHandled())
                break;

            (*it)->OnEvent(event);
        }
    }

    bool Application::OnWindowClosed(WindowClosedEvent& event) {
        ZIBEN_CORE_ERROR("WindowClosedEvent");
        return true;
    }

    bool Application::OnWindowResized(WindowResizedEvent& event) {
        ZIBEN_PROFILE_FUNCTION();

        m_IsMinimized = event.GetWidth() == 0 || event.GetHeight() == 0;

        Renderer::OnWindowResized(event.GetWidth(), event.GetHeight());

        return false;
    }

    bool Application::OnWindowMinimized(WindowMinimizedEvent& event) {
        m_IsMinimized = true;
        return false;
    }

} // namespace Ziben

// Entry Point!
int main(int argc, char** argv) {
    Ziben::Log::Create();
    Ziben::Profile::ProfileEngine::Create();
    Ziben::Random::Create();

    // Startup
    ZIBEN_PROFILE_BEGIN_SESSION("Startup", "ZibenProfile_Startup.json");
    auto application = ::Ziben::CreateApplication(argc, argv);
    ZIBEN_PROFILE_END_SESSION();

    // Runtime
    ZIBEN_PROFILE_BEGIN_SESSION("Runtime", "ZibenProfile_Runtime.json");
    application->Run();
    ZIBEN_PROFILE_END_SESSION();

    // Shutdown
    ZIBEN_PROFILE_BEGIN_SESSION("Shutdown", "ZibenProfile_Shutdown.json");
    delete application;
    ZIBEN_PROFILE_END_SESSION();

//    Ziben::Profile::ProfileEngine::Destroy();
//    Ziben::Log::Destroy();
}