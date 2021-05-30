#include "Input.hpp"

namespace Ziben {

    GLFWwindow* Input::s_Handle = nullptr;

    void Input::Init(GLFWwindow* handle) {
        s_Handle = handle;
    }

    bool Input::IsButtonPressed(ButtonCode buttonCode) {
        return glfwGetMouseButton(s_Handle, buttonCode) == GLFW_PRESS;
    }

    bool Input::IsButtonPressed(std::initializer_list<ButtonCode> buttonCodes) {
        return std::any_of(buttonCodes.begin(), buttonCodes.end(), [](ButtonCode buttonCode) {
            return glfwGetMouseButton(s_Handle, buttonCode) == GLFW_PRESS;
        });
    }

    bool Input::IsKeyPressed(KeyCode keyCode) {
        return glfwGetKey(s_Handle, keyCode) == GLFW_PRESS;
    }

    bool Input::IsKeyPressed(std::initializer_list<KeyCode> keyCodes) {
        return std::any_of(keyCodes.begin(), keyCodes.end(), [](KeyCode keyCode) {
            return glfwGetKey(s_Handle, keyCode) == GLFW_PRESS;
        });
    }

} // namespace Ziben