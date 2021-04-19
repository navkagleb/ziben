#include "SandboxApplication.hpp"

#include "Scene/DiffuseScene.hpp"

SandboxApplication::SandboxApplication(std::string title, int width, int height)
    : Ziben::Application(std::move(title), width, height) {

    GetSceneManager().PushScene(new DiffuseScene);
}