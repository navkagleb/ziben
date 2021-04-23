#include "RenderCommand.hpp"

namespace Ziben {

    void RenderCommand::SetClearColor(const glm::vec4& color) {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RenderCommand::Clear() {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void RenderCommand::DrawIndexed(const VertexArray* vertexArray) {
        glDrawElements(
            GL_TRIANGLES,
            static_cast<GLsizei>(vertexArray->GetIndexBuffer().GetCount()),
            GL_UNSIGNED_INT,
            nullptr
        );
    }

} // namespace Ziben