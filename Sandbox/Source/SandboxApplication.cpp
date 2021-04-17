#include "SandboxApplication.hpp"

#include <Ziben/System/Log.hpp>

#include <Ziben/Event/EventDispatcher.hpp>
#include <Ziben/Event/WindowEvent.hpp>

SandboxApplication::SandboxApplication(std::string title, int width, int height)
    : Ziben::Application(std::move(title), width, height) {

    m_Window->SetEventFunc([this](auto& event) { OnEvent(std::forward<decltype(event)>(event)); });
}

void SandboxApplication::OnEvent(Ziben::Event& event) {
    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::WindowFocusedEvent>([this](auto& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::WindowCloseEvent>([this](auto& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });
}