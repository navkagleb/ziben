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

        static void Submit(Shader* shader, const VertexArray* vertexArray);

    private:
        static glm::mat4 s_ViewProjectionMatrix;

    }; // class Renderer

} // namespace Ziben