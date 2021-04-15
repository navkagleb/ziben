#pragma once

#include <glm/vec2.hpp>

#include "Ziben/Core/ButtonCodes.hpp"
#include "Ziben/Core/KeyCodes.hpp"

namespace Ziben {

    class Input {
    public:
        // Mouse
        static bool IsButtonPressed(ButtonCode buttonCode);

        static glm::vec<2, int> GetMousePosition();
        static inline int GetMouseX() { return GetMousePosition().x; };
        static inline int GetMouseY() { return GetMousePosition().y; };

        // Keyboard
        static bool IsKeyPressed(KeyCode keyCode);

    }; // class Input

} // namespace Ziben