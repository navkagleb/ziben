#include "Application/Application.hpp"

Ziben::Application* Ziben::CreateApplication(int argc, char** argv) {
    (void)argc;
    (void)argv;

    return new Sandbox::Application("SandboxApplication", 1280, 720);
}