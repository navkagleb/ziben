#include "MenuLayer.hpp"

#include <imgui.h>

#include "Application/SandboxApplication.hpp"
#include "Scene/DiffuseScene.hpp"
#include "Scene/ADSScene.hpp"

MenuLayer::MenuLayer()
    : Ziben::Layer("MenuLayer") {}

void MenuLayer::OnImGuiRender() {
    auto& sceneManager = SandboxApplication::Get().GetSceneManager();

    ImGui::Begin("Menu");

    {
        if (ImGui::Button("Diffuse")) {
            if (!sceneManager.HasActiveScene() || !dynamic_cast<DiffuseScene*>(sceneManager.GetActiveScene()))
                SandboxApplication::Get().GetSceneManager().PushScene(new DiffuseScene);
        }

        if (ImGui::Button("ADS")) {
            if (!sceneManager.HasActiveScene() || !dynamic_cast<ADSScene*>(sceneManager.GetActiveScene()))
                SandboxApplication::Get().GetSceneManager().PushScene(new ADSScene);
        }
    }

    ImGui::End();
}