#include "SandboxApplication.hpp"

#include "Layer/MenuLayer.hpp"

SandboxApplication::SandboxApplication(std::string title, int width, int height)
    : Ziben::Application(std::move(title), width, height) {

    GetLayerStack().PushLayer(new MenuLayer);
}