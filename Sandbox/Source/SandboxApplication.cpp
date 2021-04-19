#include "SandboxApplication.hpp"

#include "DiffuseScene.hpp"

SandboxApplication::SandboxApplication(std::string title, int width, int height)
    : Ziben::Application(std::move(title), width, height) {

    GetSceneManager().PushScene(new DiffuseScene);
}