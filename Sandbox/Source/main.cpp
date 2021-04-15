#include <iostream>

#include <Ziben/System/Window.hpp>
#include <Ziben/System/Input.hpp>

#include <Ziben/Event/EventDispatcher.hpp>
#include <Ziben/Event/WindowEvent.hpp>
#include <Ziben/Event/MouseEvent.hpp>
#include <Ziben/Event/KeyEvent.hpp>

int main() {
    auto& window = Ziben::Window::Create("Ziben GameEngine", 1280, 720);

    window.SetEventFunc([&](Ziben::Event& event) {
        if (event.IsInCategory(Ziben::EventCategoryWindow))
            std::cout << event.ToString() << std::endl;

        Ziben::EventDispatcher dispatcher(event);

        dispatcher.Dispatch<Ziben::KeyPressedEvent>([&](auto& event) {
            if (event.GetKeyCode() == Ziben::Key::Escape)
                window.Close();

            return true;
        });
    });

    while (window.IsOpen()) {

        if (Ziben::Input::IsKeyPressed(Ziben::Key::E))
            std::cout << "Event!" << std::endl;

        window.OnUpdate();
    }

    Ziben::Window::Destroy();

    return 0;
}