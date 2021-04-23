#pragma once

#include <glm/vec2.hpp>

#include "ButtonCodes.hpp"
#include "KeyCodes.hpp"

struct GLFWwindow;

namespace Ziben {

    class Input {
    public:
        static void Init(GLFWwindow* handle);

        // Mouse
        static bool IsButtonPressed(ButtonCode buttonCode);

        static glm::vec<2, int> GetMousePosition();
        static inline int GetMouseX() { return GetMousePosition().x; };
        static inline int GetMouseY() { return GetMousePosition().y; };

        // Keyboard
        static bool IsKeyPressed(KeyCode keyCode);

    private:
        static GLFWwindow* s_Handle;

    }; // class Input

} // namespace Ziben