#pragma once

#include "Camera.hpp"
#include "OrthographicCamera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"

namespace Ziben {

    class Renderer {
    public:
        static void Init();
        static void Shutdown();

        static void OnWindowResized(int width, int height);

        static void BeginScene(const Camera& camera);
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void Submit(
            const Ref<Shader>&      shader,
            const Ref<VertexArray>& vertexArray,
            const glm::mat4&        transform = glm::mat4(1.0f)
        );

    private:
        struct RendererStorage {
            glm::mat4 ViewProjectionMatrix = glm::mat4(1.0f);
        };

    private:
        static RendererStorage& GetStorage();

    }; // class Renderer

} // namespace Ziben