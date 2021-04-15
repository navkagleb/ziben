#include "Input.hpp"

#include "Window.hpp"

namespace Ziben {

    bool Input::IsButtonPressed(ButtonCode buttonCode) {
        return glfwGetMouseButton(Window::GetRef().GetHandle(), buttonCode) == GLFW_PRESS;
    }

    glm::vec<2, int> Input::GetMousePosition() {
        glm::vec<2, int> result(0);

        glfwGetCursorPos(
            Window::GetRef().GetHandle(),
            reinterpret_cast<double *>(&result.x),
            reinterpret_cast<double *>(&result.y)
        );

        return result;
    }

    bool Input::IsKeyPressed(KeyCode keyCode) {
        return glfwGetKey(Window::GetRef().GetHandle(), keyCode) == GLFW_PRESS;
    }

} // namespace Ziben