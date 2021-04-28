#include "Renderer2D.hpp"

#include "RenderCommand.hpp"

namespace Ziben {

    struct Renderer2DStorage {

    };

    void Renderer2D::Init() {
        GetStorage()->QuadVertexArray = VertexArray::Create();

        float positions[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
            -0.5f,  0.5f, 0.0f
        };

        IndexType indices[] = {
            0, 1, 2,
            2, 3, 0
        };

        auto vertexBuffer = VertexBuffer::Create(positions, sizeof(positions ));
        vertexBuffer->SetLayout({
            { ShaderData::Type::Float3, "VertexPosition" }
        });

        auto indexBuffer = IndexBuffer::Create(indices, sizeof(indices) / sizeof(HandleType));

        GetStorage()->QuadVertexArray = VertexArray::Create();
        GetStorage()->QuadVertexArray->PushVertexBuffer(vertexBuffer);
        GetStorage()->QuadVertexArray->SetIndexBuffer(indexBuffer);

        GetStorage()->FlatColorShader = Shader::Create("Assets/Shaders/FlatColorShader.glsl");
    }

    void Renderer2D::Shutdown() {
        GetStorage().reset();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        auto& shader = GetStorage()->FlatColorShader;

        Shader::Bind(shader);
        shader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
        shader->SetUniform("u_Transform", glm::mat4(1.0f));
    }

    void Renderer2D::EndScene() {

    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        auto& shader      = GetStorage()->FlatColorShader;
        auto& vertexArray = GetStorage()->QuadVertexArray;

        Shader::Bind(shader);
        shader->SetUniform("u_Color", color);

        VertexArray::Bind(vertexArray);
        RenderCommand::DrawIndexed(vertexArray);
    }

    Scope<Renderer2D::Renderer2DStorage>& Renderer2D::GetStorage() {
        static auto storage = CreateScope<Renderer2D::Renderer2DStorage>();
        return storage;
    }

} // namespace Ziben