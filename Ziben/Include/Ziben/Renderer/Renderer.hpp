#pragma once

#include "OrthographicCamera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "RenderCommand.hpp"

namespace Ziben {

    class Renderer {
    public:
        static void BeginScene(OrthographicCamera& camera);
        static void EndScene();

        static void Submit(
            const Ref<Shader>&      shader,
            const Ref<VertexArray>& vertexArray,
            const glm::mat4&        transform = glm::mat4(1.0f)
        );

    private:
        static glm::mat4 s_ViewProjectionMatrix;

    }; // class Renderer

} // namespace Ziben