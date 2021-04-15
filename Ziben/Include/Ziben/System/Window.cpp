#include "Window.hpp"

#include "Ziben/Event/MouseEvent.hpp"
#include "Ziben/Event/KeyEvent.hpp"
#include "Ziben/Event/WindowEvent.hpp"

namespace Ziben {

    void Window_Impl::SetEventFunc(const EventFunc& eventFunc) {
        m_EventFunc = eventFunc;
    }

    void Window_Impl::SetEventFunc(EventFunc&& eventFunc) {
        m_EventFunc = std::move(eventFunc);
    }

    void Window_Impl::Hide() {
        glfwHideWindow(m_Handle);
    }

    void Window_Impl::Show() {
        glfwShowWindow(m_Handle);
    }

    void Window_Impl::OnUpdate() {
        glfwPollEvents();
        glfwSwapBuffers(m_Handle);
    }

    void Window_Impl::Close() {
        glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
        WindowCloseCallback(m_Handle);
    }

    void Window_Impl::MouseMovedCallback(HandleType* handle, double x, double y) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::MouseMovedCallback: m_EventFunc is not init!");

        MouseMovedEvent event(static_cast<int>(x), static_cast<int>(y));
        instance.m_EventFunc(event);
    }

    void Window_Impl::MouseScrolledCallback(HandleType* handle, double offsetX, double offsetY) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::MouseScrolledCallback: m_EventFunc is not init!");

        MouseScrolledEvent event(static_cast<int>(offsetX), static_cast<int>(offsetY));
        instance.m_EventFunc(event);
    }

    void Window_Impl::MouseButtonCallback(HandleType* handle, int button, int action, int mods) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::MouseButtonCallback: m_EventFunc is not init!");

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(button);
                instance.m_EventFunc(event);
                break;
            }

            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(button);
                instance.m_EventFunc(event);
            }

            default: break;
        }
    }

    void Window_Impl::KeyCallback(HandleType* handle, int key, int scancode, int action, int mods) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::KeyCallback: m_EventFunc is not init!");

        static KeyCode prevKey = Key::Unknown;
        static int     count;

        if (prevKey != key)
            count = 1;

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(key, count);
                instance.m_EventFunc(event);
                break;
            }

            case GLFW_RELEASE: {
                KeyReleasedEvent event(key);
                instance.m_EventFunc(event);
                break;
            }


            case GLFW_REPEAT: {
                ++count;
                KeyPressedEvent event(key, count);
                instance.m_EventFunc(event);
                break;
            }

            default: break;
        }

        prevKey = key;
    }

    void Window_Impl::TypedCallback(HandleType* handle, unsigned int key) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::TypedCallback: m_EventFunc is not init!");

        KeyTypedEvent event(key);
        instance.m_EventFunc(event);
    }

    void Window_Impl::WindowResizeCallback(HandleType* handle, int width, int height) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::WindowResizeCallback: m_EventFunc is not init!");

        instance.m_Width  = width;
        instance.m_Height = height;

        WindowResizeEvent event(width, height);
        instance.m_EventFunc(event);
    }

    void Window_Impl::WindowCloseCallback(HandleType* handle) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::WindowCloseCallback: m_EventFunc is not init!");

        WindowCloseEvent event;
        instance.m_EventFunc(event);
    }

    void Window_Impl::WindowMaximizedCallback(HandleType* handle, int maximized) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::WindowMaximizedCallback: m_EventFunc is not init!");

        if (maximized == GLFW_TRUE) {
            WindowMaximizedEvent event;
            instance.m_EventFunc(event);
        }
    }

    void Window_Impl::WindowMinimizedCallback(HandleType* handle, int minimized) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::WindowMinimizedCallback: m_EventFunc is not init!");

        if (minimized == GLFW_TRUE) {
            WindowMinimizedEvent event;
            instance.m_EventFunc(event);
        }
    }

    void Window_Impl::WindowFocusCallback(HandleType* handle, int focused) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::WindowFocusCallback: m_EventFunc is not init!");

        if (focused == GLFW_TRUE) {
            WindowFocusedEvent event;
            instance.m_EventFunc(event);
        } else {
            WindowUnfocusedEvent event;
            instance.m_EventFunc(event);
        }
    }

    void Window_Impl::WindowHoverCallback(HandleType *handle, int hovered) {
        auto& instance = *(Window_Impl*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventFunc)
            throw std::runtime_error("Ziben::Window_Impl::WindowFocusCallback: m_EventFunc is not init!");

        if (hovered == GLFW_TRUE) {
            WindowCursorEnteredEvent event;
            instance.m_EventFunc(event);
        } else {
            WindowCursorLeftEvent event;
            instance.m_EventFunc(event);
        }
    }

    Window_Impl::Window_Impl(std::string title, int width, int height)
        : m_Title(std::move(title))
        , m_Width(width)
        , m_Height(height)
        , m_IsVerticalSync(false)
        , m_Handle(nullptr) {

        if (!glfwInit())
            throw std::runtime_error("Ziben::Window_Impl::Ctor: glfw crash in init!");

        m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);

        if (!m_Handle)
            throw std::runtime_error("Ziben::Window_Impl::Ctor: m_Handle is nullptr!");

        glfwMakeContextCurrent(m_Handle);
        glfwSetWindowUserPointer(m_Handle, this);

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
        glfwSetWindowContentScaleCallback(m_Handle, [](HandleType* handle, float xscale, float yscale) {
            std::cout << "fuck this shit" << std::endl;
        });
    }

    Window_Impl::~Window_Impl() {
        glfwDestroyWindow(m_Handle);
        glfwTerminate();
    }

} // namespace Ziben