#include "Application.hpp"

#include "Layers/Sandbox2D.hpp"
#include "Layers/SortLayer.hpp"

namespace Sandbox {

    Application::Application(std::string title, int width, int height)
        : Ziben::Application(std::move(title), width, height) {

//    PushLayer(new MenuLayer);
//    PushLayer(new Layer2D);
//        PushLayer(new Sandbox2D);
        PushLayer(new SortLayer);
    }

} // namespace Sandbox