#include "ImGuiLayer.hpp"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "Ziben/Application.hpp"

namespace Ziben {

    ImGuiLayer::ImGuiLayer()
        : Layer("ImGuiLayer")
        , m_IsBlockedEvents(false) {}

    void ImGuiLayer::OnEvent(Event& event) {
        ImGuiIO& io = ImGui::GetIO();

//        if (!m_IsBlockedEvents) {
//            event.m_IsHandled |= event.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;
//            event.m_IsHandled |= event.IsInCategory(EventCategoryMouse)    & io.WantCaptureMouse;
//        }
    }

    void ImGuiLayer::OnAttach() {
        ZIBEN_PROFILE_FUNCTION();

        IMGUI_CHECKVERSION();
        ImGui::CreateContext();

        ImGuiIO& io = ImGui::GetIO(); (void)io;

        io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       // Enable Keyboard Controls
        io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;        // Enable Gamepad Controls

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

    void ImGuiLayer::OnImGuiRender() {
        ImGui::Begin("FPS");

        {
            ImGui::Text(
                "Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate,
                ImGui::GetIO().Framerate
            );
        }

        ImGui::End();
    }

    void ImGuiLayer::Begin() {
        ZIBEN_PROFILE_FUNCTION();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void ImGuiLayer::End() {
        ZIBEN_PROFILE_FUNCTION();

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void ImGuiLayer::BlockEvents(bool block) {
        m_IsBlockedEvents = block;
    }

} // namespace Ziben