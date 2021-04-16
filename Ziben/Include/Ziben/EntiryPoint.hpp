#pragma once

#include "Ziben/Application.hpp"
#include "Ziben/System/Log.hpp"

int main(int argc, char* argv[]) {
    Ziben::Log::Create();

    ZIBEN_CORE_INFO("Ziben Core Log!");
    ZIBEN_INFO("Ziben Client Log!");

    auto application = ::CreateApplication();
    application->Run();

    delete application;

    Ziben::Log::Destroy();
}