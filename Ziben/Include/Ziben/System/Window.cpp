#include "Window.hpp"

#include "Ziben/Event/MouseEvent.hpp"
#include "Ziben/Event/KeyEvent.hpp"
#include "Ziben/Event/WindowEvent.hpp"

namespace Ziben {

    Window::Window(std::string title, int width, int height)
        : m_Title(std::move(title))
        , m_Width(width)
        , m_Height(height)
        , m_IsVerticalSync(false)
        , m_Handle(nullptr) {

        if (!glfwInit())
            throw std::runtime_error("Ziben::Window::Ctor: glfw crash in init!");

        m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

        if (!m_Handle)
            throw std::runtime_error("Ziben::Window::Ctor: m_Handle is nullptr!");

        glfwMakeContextCurrent(m_Handle);
        glfwSetWindowUserPointer(m_Handle, this);

        if (glewInit() != GLEW_OK)
            throw std::runtime_error("Ziben::Window::Ctor: glew crash in init!");

        // Set glfw callbacks
        glfwSetCursorPosCallback(     m_Handle, MouseMovedCallback     );
        glfwSetScrollCallback(        m_Handle, MouseScrolledCallback  );
        glfwSetMouseButtonCallback(   m_Handle, MouseButtonCallback    );
        glfwSetKeyCallback(           m_Handle, KeyCallback            );
        glfwSetCharCallback(          m_Handle, TypedCallback          );
        glfwSetWindowSizeCallback(    m_Handle, WindowResizeCallback   );
        glfwSetWindowCloseCallback(   m_Handle, WindowCloseCallback    );
        glfwSetWindowMaximizeCallback(m_Handle, WindowMaximizedCallback);
        glfwSetWindowIconifyCallback( m_Handle, WindowMinimizedCallback);
        glfwSetWindowFocusCallback(   m_Handle, WindowFocusCallback    );
        glfwSetCursorEnterCallback(   m_Handle, WindowHoverCallback    );
    }

    Window::~Window() {
        glfwDestroyWindow(m_Handle);
        glfwTerminate();
    }

    void Window::SetEventFunc(const EventFunc& eventFunc) {
        m_EventFunc = eventFunc;
    }

    void Window::SetEventFunc(EventFunc&& eventFunc) {
        m_EventFunc = std::move(eventFunc);
    }

    void Window::Hide() {
        glfwHideWindow(m_Handle);
    }

    void Window::Show() {
        glfwShowWindow(m_Handle);
    }

    void Window::OnEventUpdate() {
        glfwPollEvents();
    }

    void Window::SwapBuffers() {
        glfwSwapBuffers(m_Handle);
    }

    void Window::Close() {
        glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
        WindowCloseCallback(m_Handle);
    }

    void Window::MouseMovedCallback(HandleType* handle, double x, double y) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::MouseMovedCallback: m_EventFunc is not init!");

        MouseMovedEvent event(static_cast<int>(x), static_cast<int>(y));
        instance.m_EventFunc(event);
    }

    void Window::MouseScrolledCallback(HandleType* handle, double offsetX, double offsetY) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::MouseScrolledCallback: m_EventFunc is not init!");

        MouseScrolledEvent event(static_cast<int>(offsetX), static_cast<int>(offsetY));
        instance.m_EventFunc(event);
    }

    void Window::MouseButtonCallback(HandleType* handle, int button, int action, int mods) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::MouseButtonCallback: m_EventFunc is not init!");

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(static_cast<ButtonCode>(button));
                instance.m_EventFunc(event);
                break;
            }

            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(static_cast<ButtonCode>(button));
                instance.m_EventFunc(event);
            }

            default: break;
        }
    }

    void Window::KeyCallback(HandleType* handle, int key, int scancode, int action, int mods) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::KeyCallback: m_EventFunc is not init!");

        auto nativeKey = static_cast<KeyCode>(key);

        static KeyCode prevKey = Key::Unknown;
        static int     count;

        if (prevKey != nativeKey)
            count = 1;

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(nativeKey, count);
                instance.m_EventFunc(event);
                break;
            }

            case GLFW_RELEASE: {
                KeyReleasedEvent event(nativeKey);
                instance.m_EventFunc(event);
                break;
            }


            case GLFW_REPEAT: {
                ++count;
                KeyPressedEvent event(nativeKey, count);
                instance.m_EventFunc(event);
                break;
            }

            default: break;
        }

        prevKey = nativeKey;
    }

    void Window::TypedCallback(HandleType* handle, unsigned int key) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::TypedCallback: m_EventFunc is not init!");

        KeyTypedEvent event(static_cast<KeyCode>(key));
        instance.m_EventFunc(event);
    }

    void Window::WindowResizeCallback(HandleType* handle, int width, int height) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::WindowResizeCallback: m_EventFunc is not init!");

        instance.m_Width  = width;
        instance.m_Height = height;

        WindowResizeEvent event(width, height);
        instance.m_EventFunc(event);
    }

    void Window::WindowCloseCallback(HandleType* handle) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::WindowCloseCallback: m_EventFunc is not init!");

        WindowCloseEvent event;
        instance.m_EventFunc(event);
    }

    void Window::WindowMaximizedCallback(HandleType* handle, int maximized) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::WindowMaximizedCallback: m_EventFunc is not init!");

        if (maximized == GLFW_TRUE) {
            WindowMaximizedEvent event;
            instance.m_EventFunc(event);
        }
    }

    void Window::WindowMinimizedCallback(HandleType* handle, int minimized) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::WindowMinimizedCallback: m_EventFunc is not init!");

        if (minimized == GLFW_TRUE) {
            WindowMinimizedEvent event;
            instance.m_EventFunc(event);
        }
    }

    void Window::WindowFocusCallback(HandleType* handle, int focused) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::WindowFocusCallback: m_EventFunc is not init!");

        if (focused == GLFW_TRUE) {
            WindowFocusedEvent event;
            instance.m_EventFunc(event);
        } else {
            WindowUnfocusedEvent event;
            instance.m_EventFunc(event);
        }
    }

    void Window::WindowHoverCallback(HandleType *handle, int hovered) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window::WindowFocusCallback: m_EventFunc is not init!");

        if (hovered == GLFW_TRUE) {
            WindowCursorEnteredEvent event;
            instance.m_EventFunc(event);
        } else {
            WindowCursorLeftEvent event;
            instance.m_EventFunc(event);
        }
    }

} // namespace Ziben