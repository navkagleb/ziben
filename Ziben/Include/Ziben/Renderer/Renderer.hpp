#pragma once

#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "RenderCommand.hpp"

namespace Ziben {

    class Renderer {
    public:
        static void Init();
        static void OnWindowResized(int width, int height);

        static void BeginScene(Camera& camera);
        static void EndScene();

        static void Submit(
            const Ref<Shader>&      shader,
            const Ref<VertexArray>& vertexArray,
            const glm::mat4&        transform = glm::mat4(1.0f)
        );

    private:
        struct RendererData {
            glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        };

    private:
        static RendererData& GetData();

    }; // class Renderer

} // namespace Ziben