#pragma once

#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "RenderCommand.hpp"

namespace Ziben {

    class Renderer {
    public:
        static void Init();

        static void BeginScene(Camera& camera);
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