#include <Ziben/EntiryPoint.hpp>

#include "SandboxApplication.hpp"

Ziben::Application* CreateApplication() {
    return new SandboxApplication("SandboxApplication", 1280, 720);
}