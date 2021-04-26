#include "MenuLayer.hpp"

#include <imgui.h>

#include "Application/SandboxApplication.hpp"
#include "Scene/DiffuseScene.hpp"
#include "Scene/ADSScene.hpp"
#include "Scene/Scene2D.hpp"
#include "Scene/CubeScene.hpp"

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

        if (ImGui::Button("2D")) {
            if (!sceneManager.HasActiveScene() || !dynamic_cast<Scene2D*>(sceneManager.GetActiveScene()))
                SandboxApplication::Get().GetSceneManager().PushScene(new Scene2D);
        }

        if (ImGui::Button("Cube")) {
            if (!sceneManager.HasActiveScene() || !dynamic_cast<CubeScene*>(sceneManager.GetActiveScene()))
                SandboxApplication::Get().GetSceneManager().PushScene(new CubeScene);
        }
    }

    ImGui::End();
}