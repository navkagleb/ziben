#pragma once

#include <GLFW/glfw3.h>

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
        static bool IsButtonPressed(std::initializer_list<ButtonCode> buttonCodes);

        template <typename T = int> static glm::vec<2, T> GetMousePosition();
        template <typename T = int> static inline int GetMouseX() { return GetMousePosition<T>().x; };
        template <typename T = int> static inline int GetMouseY() { return GetMousePosition<T>().y; };

        // Keyboard
        static bool IsKeyPressed(KeyCode keyCode);
        static bool IsKeyPressed(std::initializer_list<KeyCode> keyCodes);

    private:
        static GLFWwindow* s_Handle;

    }; // class Input

    template <typename T>
    glm::vec<2, T> Input::GetMousePosition() {
        double x;
        double y;

        glfwGetCursorPos(s_Handle, &x, &y);

        return { x, y };
    }

} // namespace Ziben