#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.hpp"

namespace Ziben {

    struct Renderer2DStorage {

    };

    void Renderer2D::Init() {
        GetStorage()->QuadVertexArray = VertexArray::Create();

        float positions[] = {
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
             0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
            -0.5f,  0.5f, 0.0f, 0.0f, 1.0f
        };

        IndexType indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        auto vertexBuffer = VertexBuffer::Create(positions, sizeof(positions ));
        vertexBuffer->SetLayout({
            { ShaderData::Type::Float3, "VertexPosition" },
            { ShaderData::Type::Float2, "TexCoord"       },
        });

        auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(HandleType));

        GetStorage()->QuadVertexArray = VertexArray::Create();
        GetStorage()->QuadVertexArray->PushVertexBuffer(vertexBuffer);
        GetStorage()->QuadVertexArray->SetIndexBuffer(indexBuffer);

        GetStorage()->FlatColorShader = Shader::Create("Assets/Shaders/FlatColorShader.glsl");

        GetStorage()->TextureShader   = Shader::Create("Assets/Shaders/TextureShader.glsl");
        Shader::Bind(GetStorage()->TextureShader);
        GetStorage()->TextureShader->SetUniform("u_Texture", 0);
    }

    void Renderer2D::Shutdown() {
        GetStorage().reset();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        Shader::Bind(GetStorage()->FlatColorShader);
        GetStorage()->FlatColorShader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

        Shader::Bind(GetStorage()->TextureShader);
        GetStorage()->TextureShader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {

    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        auto& shader          = GetStorage()->FlatColorShader;
        auto& vertexArray     = GetStorage()->QuadVertexArray;

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        Shader::Bind(shader);
        shader->SetUniform("u_Color", color);
        shader->SetUniform("u_Transform", translation * scaling);

        VertexArray::Bind(vertexArray);
        RenderCommand::DrawIndexed(vertexArray);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
        auto& shader          = GetStorage()->TextureShader;
        auto& vertexArray     = GetStorage()->QuadVertexArray;

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        Shader::Bind(shader);
        shader->SetUniform("u_Transform", translation * scaling);

        Texture2D::Bind(texture);

        VertexArray::Bind(vertexArray);
        RenderCommand::DrawIndexed(vertexArray);
    }

    Scope<Renderer2D::Renderer2DStorage>& Renderer2D::GetStorage() {
        static auto storage = CreateScope<Renderer2D::Renderer2DStorage>();
        return storage;
    }

} // namespace Ziben