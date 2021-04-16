#pragma once

#include <Ziben/Application.hpp>

class SandboxApplication : public Ziben::Application {
public:
    SandboxApplication(std::string title, int width, int height);
    ~SandboxApplication() override = default;

    void OnEvent(Ziben::Event& event);

}; // class SandboxApplication