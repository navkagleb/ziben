#include "GraphicsContext.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Ziben/System/Log.hpp"

namespace Ziben {

     Scope<GraphicsContext> GraphicsContext::Create(GLFWwindow* handle) {
        return CreateScope<GraphicsContext>(handle);
    }

    GraphicsContext::GraphicsContext(GLFWwindow* handle)
        : m_Handle(handle) {}

    void GraphicsContext::Init() {
        ZIBEN_PROFILE_FUNCTION();

        glfwMakeContextCurrent(m_Handle);

        glewExperimental = GL_TRUE;

        if (glewInit() != GLEW_OK)
            throw std::runtime_error("Ziben::GraphicsContext::Init: glew crash in init!");

        const uint8_t* renderer    = glGetString(GL_RENDERER);
        const uint8_t* vendor      = glGetString(GL_VENDOR);
        const uint8_t* version     = glGetString(GL_VERSION);
        const uint8_t* glslVersion = glGetString(GL_SHADING_LANGUAGE_VERSION);

        int            major;
        int            minor;

        glGetIntegerv(GL_MAJOR_VERSION, &major);
        glGetIntegerv(GL_MINOR_VERSION, &minor);

        ZIBEN_CORE_WARN("-----------------------------------------------------------------------------");
        ZIBEN_CORE_WARN("Vendor:       {0}",     vendor      );
        ZIBEN_CORE_WARN("Renderer:     {0}",     renderer    );
        ZIBEN_CORE_WARN("Version:      {0}",     version     );
        ZIBEN_CORE_WARN("Version:      {0}.{1}", major, minor);
        ZIBEN_CORE_WARN("GLSL version: {0}",     glslVersion );
        ZIBEN_CORE_WARN("-----------------------------------------------------------------------------");
    }

    void GraphicsContext::SwapBuffers() {
        ZIBEN_PROFILE_FUNCTION();

        glfwSwapBuffers(m_Handle);
    }

} // namespace Ziben