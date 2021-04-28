#include "SandboxApplication.hpp"

#include "Layer/MenuLayer.hpp"
#include "Layer/Layer2D.hpp"
#include "Layer/Sandbox2D.hpp"

SandboxApplication::SandboxApplication(std::string title, int width, int height)
    : Ziben::Application(std::move(title), width, height) {

    PushLayer(new MenuLayer);
//    PushLayer(new Layer2D);
    PushLayer(new Sandbox2D);
}