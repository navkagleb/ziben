#include "Renderer.hpp"

#include "RenderCommand.hpp"
#include "Renderer2D.hpp"

namespace Ziben {

    void Renderer::Init() {
        ZIBEN_PROFILE_FUNCTION();

        RenderCommand::Init();
        Renderer2D::Init();
    }

    void Renderer::Shutdown() {
        Renderer2D::Shutdown();
    }

    void Renderer::OnWindowResized(int width, int height) {
        RenderCommand::SetViewport(0, 0, width, height);
    }

    void Renderer::BeginScene(const Camera& camera) {
//        GetStorage().ViewProjectionMatrix = camera.GetViewProjectionMatri();
    }

    void Renderer::BeginScene(const OrthographicCamera& camera) {
        GetStorage().ViewProjectionMatrix = camera.GetViewProjectionMatrix();
    }

    void Renderer::EndScene() {

    }

    void Renderer::Submit(const Ref<Shader>& shader, const Ref<VertexArray>& vertexArray, const glm::mat4& transform) {
        Shader::Bind(shader);
        shader->SetUniform("u_ViewProjectionMatrix", GetStorage().ViewProjectionMatrix);
        shader->SetUniform("u_Transform", transform);

        VertexArray::Bind(vertexArray);
        RenderCommand::DrawIndexed(vertexArray);
    }

    Renderer::RendererStorage& Renderer::GetStorage() {
        static RendererStorage storage;
        return storage;
    }

} // namespace Ziben