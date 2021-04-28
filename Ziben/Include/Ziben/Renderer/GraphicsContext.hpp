#pragma once

#include "Ziben/Utility/Reference.hpp"

struct GLFWwindow;

namespace Ziben {

    class GraphicsContext {
    public:
        static Scope<GraphicsContext> Create(GLFWwindow* handle);

    public:
        explicit GraphicsContext(GLFWwindow* handle);
        ~GraphicsContext() = default;

        void Init();
        void SwapBuffers();

    private:
        GLFWwindow* m_Handle;

    }; // class GraphicsContext

} // namespace Ziben