#pragma once

#include <Ziben/Window/Application.hpp>

class SandboxApplication : public Ziben::Application {
public:
    SandboxApplication(std::string title, int width, int height);
    ~SandboxApplication() override = default;

}; // class SandboxApplication