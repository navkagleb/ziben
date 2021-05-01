#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Event.hpp"
#include "Ziben/Utility/Reference.hpp"
#include "Ziben/Renderer/GraphicsContext.hpp"

namespace Ziben {

    class Window {
    public:
        using EventCallback = std::function<void(Event& event)>;
        using HandleType    = GLFWwindow;

    public:
        friend class Input;

    public:
        static Scope<Window> Create(std::string title, int width, int height);

    public:
        Window(std::string title, int width, int height);
        ~Window();

        [[nodiscard]] inline const std::string& GetTitle() const { return m_Title; }
        [[nodiscard]] inline int GetWidth() const { return m_Width; }
        [[nodiscard]] inline int GetHeight() const { return m_Height; }
        [[nodiscard]] inline bool IsVerticalSync() const { return m_IsVerticalSync; }
        [[nodiscard]] inline bool IsOpen() const { return !glfwWindowShouldClose(m_Handle); }
        [[nodiscard]] inline bool IsVisible() const { return glfwGetWindowAttrib(m_Handle, GLFW_VISIBLE); }

        void SetVerticalSync(bool enabled);
        void SetEventCallback(const EventCallback& eventFunc);
        void SetEventCallback(EventCallback&& eventFunc);

        void Hide();
        void Show();

        void OnUpdate();
        void Close();

    public:
        explicit inline operator HandleType*() const { return m_Handle; }

    private:
        static void MouseMovedCallback(HandleType* handle, double x, double y);
        static void MouseScrolledCallback(HandleType* handle, double offsetX, double offsetY);
        static void MouseButtonCallback(HandleType* handle, int button, int action, int mods);
        static void KeyCallback(HandleType* handle, int key, int scancode, int action, int mods);
        static void TypedCallback(HandleType* handle, unsigned int key);
        static void WindowResizeCallback(HandleType* handle, int width, int height);
        static void WindowCloseCallback(HandleType* handle);
        static void WindowMaximizedCallback(HandleType* handle, int maximized);
        static void WindowMinimizedCallback(HandleType* handle, int minimized);
        static void WindowFocusCallback(HandleType* handle, int focused);
        static void WindowHoverCallback(HandleType* handle, int hovered);

    private:
        std::string            m_Title;
        int                    m_Width;
        int                    m_Height;
        bool                   m_IsVerticalSync;
        HandleType*            m_Handle;
        Scope<GraphicsContext> m_Context;
        EventCallback          m_EventCallback;

    }; // class Window

} // namespace Ziben