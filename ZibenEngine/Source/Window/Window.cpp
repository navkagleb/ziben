#include "Window.hpp"

#include "Input.hpp"
#include "MouseEvent.hpp"
#include "KeyEvent.hpp"
#include "WindowEvent.hpp"

#include "Ziben/System/FileDialogs.hpp"

namespace Ziben {

    Scope<Window> Window::Create(std::string title, int width, int height) {
        return CreateScope<Window>(std::move(title), width, height);
    }

    Window::Window(std::string title, int width, int height)
        : m_Title(std::move(title))
        , m_Width(width)
        , m_Height(height)
        , m_IsVerticalSync(false)
        , m_Handle(nullptr) {

        ZIBEN_PROFILE_FUNCTION();

        // Init GLFW
        {
            ZIBEN_PROFILE_SCOPE("glfwInit");

            if (!glfwInit())
                throw std::runtime_error("Ziben::Window::Ctor: glfw crash in init!");
        }

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4                       );
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6                       );
        glfwWindowHint(GLFW_OPENGL_PROFILE,        GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE                 );
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,  GL_TRUE                 );

        {
            ZIBEN_PROFILE_SCOPE("glfwCreateWindow");

            m_Handle = glfwCreateWindow(m_Width, m_Height, m_Title.c_str(), nullptr, nullptr);
        }

        if (!m_Handle)
            throw std::runtime_error("Ziben::Window::Ctor: m_Handle is nullptr!");

        // Init glew
        m_Context = GraphicsContext::Create(m_Handle);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Handle, this);
        SetVerticalSync(true);

        // Init input
        Input::Init(m_Handle);

        // Init FileDialogs
        FileDialogs::Init(m_Handle);

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
        ZIBEN_PROFILE_FUNCTION();

        glfwDestroyWindow(m_Handle);
        glfwTerminate();
    }

    void Window::SetVerticalSync(bool enabled) {
        ZIBEN_PROFILE_FUNCTION();

        glfwSwapInterval(m_IsVerticalSync = enabled);
    }

    void Window::SetEventCallback(const EventCallback& eventCallback) {
        m_EventCallback = eventCallback;
    }

    void Window::SetEventCallback(EventCallback&& eventCallback) {
        m_EventCallback = std::move(eventCallback);
    }

    void Window::Hide() {
        glfwHideWindow(m_Handle);
    }

    void Window::Show() {
        glfwShowWindow(m_Handle);
    }

    void Window::OnUpdate() {
        ZIBEN_PROFILE_FUNCTION();

        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void Window::Close() {
        ZIBEN_PROFILE_FUNCTION();

        glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
        WindowCloseCallback(m_Handle);
    }

    void Window::MouseMovedCallback(HandleType* handle, double x, double y) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::MouseMovedCallback: m_EventCallback is not init!");

        MouseMovedEvent event(static_cast<int>(x), static_cast<int>(y));
        instance.m_EventCallback(event);
    }

    void Window::MouseScrolledCallback(HandleType* handle, double offsetX, double offsetY) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::MouseScrolledCallback: m_EventCallback is not init!");

        MouseScrolledEvent event(static_cast<int>(offsetX), static_cast<int>(offsetY));
        instance.m_EventCallback(event);
    }

    void Window::MouseButtonCallback(HandleType* handle, int button, int action, int mods) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::MouseButtonCallback: m_EventCallback is not init!");

        switch (action) {
            case GLFW_PRESS: {
                MouseButtonPressedEvent event(static_cast<ButtonCode>(button));
                instance.m_EventCallback(event);
                break;
            }

            case GLFW_RELEASE: {
                MouseButtonReleasedEvent event(static_cast<ButtonCode>(button));
                instance.m_EventCallback(event);
            }

            default: break;
        }
    }

    void Window::KeyCallback(HandleType* handle, int key, int scancode, int action, int mods) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::KeyCallback: m_EventCallback is not init!");

        auto nativeKey = static_cast<KeyCode>(key);

        static KeyCode prevKey = Key::Unknown;
        static int     count;

        if (prevKey != nativeKey)
            count = 1;

        switch (action) {
            case GLFW_PRESS: {
                KeyPressedEvent event(nativeKey, count);
                instance.m_EventCallback(event);
                break;
            }

            case GLFW_RELEASE: {
                KeyReleasedEvent event(nativeKey);
                instance.m_EventCallback(event);
                break;
            }


            case GLFW_REPEAT: {
                ++count;
                KeyPressedEvent event(nativeKey, count);
                instance.m_EventCallback(event);
                break;
            }

            default: break;
        }

        prevKey = nativeKey;
    }

    void Window::TypedCallback(HandleType* handle, unsigned int key) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::TypedCallback: m_EventCallback is not init!");

        KeyTypedEvent event(static_cast<KeyCode>(key));
        instance.m_EventCallback(event);
    }

    void Window::WindowResizeCallback(HandleType* handle, int width, int height) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::WindowResizeCallback: m_EventCallback is not init!");

        instance.m_Width  = width;
        instance.m_Height = height;

        WindowResizedEvent event(width, height);
        instance.m_EventCallback(event);
    }

    void Window::WindowCloseCallback(HandleType* handle) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::WindowCloseCallback: m_EventCallback is not init!");

        WindowClosedEvent event;
        instance.m_EventCallback(event);
    }

    void Window::WindowMaximizedCallback(HandleType* handle, int maximized) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::WindowMaximizedCallback: m_EventCallback is not init!");

        if (maximized == GLFW_TRUE) {
            WindowMaximizedEvent event;
            instance.m_EventCallback(event);
        }
    }

    void Window::WindowMinimizedCallback(HandleType* handle, int minimized) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::WindowMinimizedCallback: m_EventCallback is not init!");

        if (minimized == GLFW_TRUE) {
            WindowMinimizedEvent event;
            instance.m_EventCallback(event);
        }
    }

    void Window::WindowFocusCallback(HandleType* handle, int focused) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::WindowFocusCallback: m_EventCallback is not init!");

        if (focused == GLFW_TRUE) {
            WindowFocusedEvent event;
            instance.m_EventCallback(event);
        } else {
            WindowUnfocusedEvent event;
            instance.m_EventCallback(event);
        }
    }

    void Window::WindowHoverCallback(HandleType *handle, int hovered) {
        auto& instance = *(Window*)glfwGetWindowUserPointer(handle);

        if (!instance.m_EventCallback)
            throw std::runtime_error("Ziben::Window::WindowFocusCallback: m_EventCallback is not init!");

        if (hovered == GLFW_TRUE) {
            WindowCursorEnteredEvent event;
            instance.m_EventCallback(event);
        } else {
            WindowCursorLeftEvent event;
            instance.m_EventCallback(event);
        }
    }

} // namespace Ziben