#include "Input.hpp"

#include <GLFW/glfw3.h>

namespace Ziben {

    GLFWwindow* Input::s_Handle = nullptr;

    void Input::Init(GLFWwindow* handle) {
        s_Handle = handle;
    }

    bool Input::IsButtonPressed(ButtonCode buttonCode) {
        return glfwGetMouseButton(s_Handle, buttonCode) == GLFW_PRESS;
    }

    glm::vec<2, int> Input::GetMousePosition() {
        glm::vec<2, int> result(0);

        glfwGetCursorPos(
            s_Handle,
            reinterpret_cast<double*>(&result.x),
            reinterpret_cast<double*>(&result.y)
        );

        return result;
    }

    bool Input::IsKeyPressed(KeyCode keyCode) {
        return glfwGetKey(s_Handle, keyCode) == GLFW_PRESS;
    }

} // namespace Ziben