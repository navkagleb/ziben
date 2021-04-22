#pragma once

struct GLFWwindow;

namespace Ziben {

    class GraphicsContext {
    public:
        static GraphicsContext* Create(GLFWwindow* handle);

    public:
        explicit GraphicsContext(GLFWwindow* handle);
        ~GraphicsContext() = default;

        void Init();
        void SwapBuffers();

    private:
        GLFWwindow* m_Handle;

    }; // class GraphicsContext

} // namespace Ziben