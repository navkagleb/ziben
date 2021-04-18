#include "SandboxApplication.hpp"

#include <Ziben/System/Log.hpp>

#include <Ziben/Event/EventDispatcher.hpp>
#include <Ziben/Event/WindowEvent.hpp>
#include <Ziben/Event/MouseEvent.hpp>

#include "DiffuseScene.hpp"

SandboxApplication::SandboxApplication(std::string title, int width, int height)
    : Ziben::Application(std::move(title), width, height) {

//    glewExperimental = GL_TRUE;
//
//    if (!glewInit())
//        throw std::runtime_error("Ziben::Window::Ctor: glew crash in init!");

    GetWindow().SetEventFunc([this](auto& event) { OnEvent(std::forward<decltype(event)>(event)); });
    std::cout << "Before Diffuse Scene" << std::endl;
    auto diffuseScene = new DiffuseScene;
    std::cout << "_---" << std::endl;
    GetSceneManager().PushScene(diffuseScene);
    std::cout << "After Diffuse Scene" << std::endl;
}

void SandboxApplication::OnEvent(Ziben::Event& event) {
    Ziben::EventDispatcher dispatcher(event);

    dispatcher.Dispatch<Ziben::WindowFocusedEvent>([this](Ziben::WindowFocusedEvent& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::WindowCloseEvent>([this](Ziben::WindowCloseEvent& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });

    dispatcher.Dispatch<Ziben::MouseButtonPressedEvent>([this](auto& event) {
        ZIBEN_INFO("{0}", event.ToString());
        return true;
    });
}