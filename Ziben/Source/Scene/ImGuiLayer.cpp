#include "ImGuiLayer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include <ImGuizmo.h>

#include "Ziben/Application.hpp"

namespace Ziben {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
        , m_IsBlockedEvents(true) {}

    void ImGuiLayer::OnEvent(Event& event) {
        ImGuiIO& io = ImGui::GetIO();

        if (m_IsBlockedEvents) {
            event.m_IsHandled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
            event.m_IsHandled |= event.IsInCategory(EventCategoryMouse)    & io.WantCaptureMouse;
        }
    }

    void ImGuiLayer::OnAttach() {
        ZIBEN_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
//        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
        io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows

        ImFontConfig config;
        config.OversampleH = 3;

        io.FontDefault = io.Fonts->AddFontFromFileTTF("Assets/Fonts/OpenSans-Regular.ttf", 20.0f, &config);

//        ImGui::StyleColorsDark();
        ImGui::StyleColorsClassic();

        auto& application = Application::Get();
        auto* window      = (GLFWwindow*)application.GetWindow();

        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 410");
    }

    void ImGuiLayer::OnDetach() {
        ZIBEN_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }

    void ImGuiLayer::Begin() {
        ZIBEN_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
        ImGuizmo::BeginFrame();
    }

    void ImGuiLayer::End() {
        ZIBEN_PROFILE_FUNCTION();

        ImGuiIO& io     = ImGui::GetIO();
        Window&  window = Application::Get().GetWindow();

        io.DisplaySize = ImVec2(static_cast<float>(window.GetWidth()), static_cast<float>(window.GetHeight()));

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();

            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
    }

    void ImGuiLayer::BlockEvents(bool isBlockedEvents) {
        m_IsBlockedEvents = isBlockedEvents;
    }

} // namespace Ziben