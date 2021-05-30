#pragma once

#include <glm/glm.hpp>

#include "VertexArray.hpp"

namespace Ziben {

    class RenderCommand {
    public:
        static void Init();
        static void SetViewport(int x, int y, int width, int height);

        static void SetClearColor(const glm::vec4& color);
        static void Clear();

        static void DrawIndexed(const Ref<VertexArray>& vertexArray, std::size_t indexCount = 0);

    }; // class RenderCommand

} // namespace RenderCommand