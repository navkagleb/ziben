#include "Input.hpp"

#include "Application.hpp"

namespace Ziben {

    bool Input::IsButtonPressed(ButtonCode buttonCode) {
        return glfwGetMouseButton((GLFWwindow*)Application::Get().GetWindow(), buttonCode) == GLFW_PRESS;
    }

    glm::vec<2, int> Input::GetMousePosition() {
        glm::vec<2, int> result(0);

        glfwGetCursorPos(
            (GLFWwindow*)Application::Get().GetWindow(),
            reinterpret_cast<double*>(&result.x),
            reinterpret_cast<double*>(&result.y)
        );

        return result;
    }

    bool Input::IsKeyPressed(KeyCode keyCode) {
        return glfwGetKey((GLFWwindow*)Application::Get().GetWindow(), keyCode) == GLFW_PRESS;
    }

} // namespace Ziben