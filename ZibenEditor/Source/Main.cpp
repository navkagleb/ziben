#include "ZibenEditor.hpp"

Ziben::Application* Ziben::CreateApplication(int /* argc */, char** /* argv */) {
    return new ZibenEditor;
}