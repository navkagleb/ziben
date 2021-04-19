#include "SandboxApplication.hpp"

Ziben::Application* Ziben::CreateApplication(int argc, char** argv) {
    return new SandboxApplication("SandboxApplication", 1280, 720);
}