#pragma once

#include <Ziben/Application.hpp>

namespace Sandbox {

    class Application : public Ziben::Application {
    public:
        Application(std::string title, int width, int height);
        ~Application() override = default;

    }; // class SandboxApplication

} // namespace Sandbox