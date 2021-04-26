#pragma once

#include <glm/glm.hpp>

#include "VertexArray.hpp"

namespace Ziben {

    class RenderCommand {
    public:
        static void Init();

        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void DrawIndexed(const Ref<VertexArray>& vertexArray);

    }; // class RenderCommand

} // namespace RenderCommand