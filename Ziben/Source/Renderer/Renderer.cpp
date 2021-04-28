#include "Renderer.hpp"

namespace Ziben {

    void Renderer::Init() {
        RenderCommand::Init();
    }

    void Renderer::OnWindowResized(int width, int height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(Camera& camera) {
        GetData().ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
        Shader::Bind(shader);
        shader->SetUniform("u_ViewProjectionMatrix", GetData().ViewProjectionMatrix);
        shader->SetUniform("u_Transform", transform);

        VertexArray::Bind(vertexArray);
        RenderCommand::DrawIndexed(vertexArray);
    }

    Renderer::RendererData& Renderer::GetData() {
        static RendererData data;
        return data;
    }

} // namespace Ziben