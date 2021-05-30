#include "RenderCommand.hpp"

namespace Ziben {

    namespace Internal {

        void DebugMessageCallback(
            uint32_t    source,
            uint32_t    type,
            uint32_t    id,
            uint32_t    severity,
            int         length,
            const char* message,
            const void* userParam
        ) {
            struct DebugMessageInfo {
                std::string Source;
                std::string Type;
                std::string Severity;
            };

            static DebugMessageInfo messageInfo;

            switch (source) {
                case GL_DEBUG_SOURCE_WINDOW_SYSTEM:     messageInfo.Source = "WindowSystem";   break;
                case GL_DEBUG_SOURCE_APPLICATION:       messageInfo.Source = "Application";    break;
                case GL_DEBUG_SOURCE_API:               messageInfo.Source = "OpenGL";         break;
                case GL_DEBUG_SOURCE_SHADER_COMPILER:   messageInfo.Source = "ShaderCompiler"; break;
                case GL_DEBUG_SOURCE_THIRD_PARTY:       messageInfo.Source = "3rdParty";       break;
                case GL_DEBUG_SOURCE_OTHER:             messageInfo.Source = "Other";          break;
                default:                                messageInfo.Source = "Unknown";        break;
            }

            switch (type) {
                case GL_DEBUG_TYPE_ERROR:               messageInfo.Type = "Error";            break;
                case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: messageInfo.Type = "Deprecated";       break;
                case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  messageInfo.Type = "Undefined";        break;
                case GL_DEBUG_TYPE_PORTABILITY:         messageInfo.Type = "Portability";      break;
                case GL_DEBUG_TYPE_PERFORMANCE:         messageInfo.Type = "Performance";      break;
                case GL_DEBUG_TYPE_MARKER:              messageInfo.Type = "Marker";           break;
                case GL_DEBUG_TYPE_PUSH_GROUP:          messageInfo.Type = "PushGroup";        break;
                case GL_DEBUG_TYPE_POP_GROUP:           messageInfo.Type = "PopGroup";         break;
                case GL_DEBUG_TYPE_OTHER:               messageInfo.Type = "Other";            break;
                default:                                messageInfo.Type = "Unknown";          break;
            }

            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:            messageInfo.Severity = "High";         break;
                case GL_DEBUG_SEVERITY_MEDIUM:          messageInfo.Severity = "Medium";       break;
                case GL_DEBUG_SEVERITY_LOW:             messageInfo.Severity = "Low";          break;
                case GL_DEBUG_SEVERITY_NOTIFICATION:    messageInfo.Severity = "Notify";       break;
                default:                                messageInfo.Severity = "Unknown";      break;
            }

            const std::string pattern = "{0}:{1}[{2}]({3}): {4}";

            switch (severity) {
                case GL_DEBUG_SEVERITY_HIGH:         ZIBEN_CORE_CRITICAL(pattern, messageInfo.Source, messageInfo.Type, messageInfo.Severity, id, message); return;
                case GL_DEBUG_SEVERITY_MEDIUM:       ZIBEN_CORE_ERROR(   pattern, messageInfo.Source, messageInfo.Type, messageInfo.Severity, id, message); return;
                case GL_DEBUG_SEVERITY_LOW:          ZIBEN_CORE_WARN(    pattern, messageInfo.Source, messageInfo.Type, messageInfo.Severity, id, message); return;
                case GL_DEBUG_SEVERITY_NOTIFICATION: ZIBEN_CORE_TRACE(   pattern, messageInfo.Source, messageInfo.Type, messageInfo.Severity, id, message); return;
                default:                             return;
            }
        }

    } // namespace Internal

    void RenderCommand::Init() {
        ZIBEN_PROFILE_FUNCTION();

//    #ifdef ZIBEN_DEBUG
        glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_FALSE);
		glDebugMessageCallback(Internal::DebugMessageCallback, nullptr);
//    #endif

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        glEnable(GL_DEPTH_TEST);
    }

    void RenderCommand::SetViewport(int x, int y, int width, int height) {
        glViewport(x, y, width, height);
    }

    void RenderCommand::SetClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::DrawIndexed(const Ref<VertexArray>& vertexArray, std::size_t indexCount) {
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount),
            GL_UNSIGNED_INT,
            nullptr
        );
    }

} // namespace Ziben