#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.hpp"

namespace Ziben {

    struct Renderer2DStorage {

    };

    void Renderer2D::Init() {
        ZIBEN_PROFILE_FUNCTION();

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

        uint32_t whiteTextureData = 0xffffffff;

        GetStorage()->WhiteTexture = Texture2D::Create(1, 1);
        GetStorage()->WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

        GetStorage()->TextureShader   = Shader::Create("Assets/Shaders/TextureShader.glsl");
        Shader::Bind(GetStorage()->TextureShader);
        GetStorage()->TextureShader->SetUniform("u_Texture", 0);
    }

    void Renderer2D::Shutdown() {
        ZIBEN_PROFILE_FUNCTION();

        GetStorage().reset();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        ZIBEN_PROFILE_FUNCTION();

        Shader::Bind(GetStorage()->TextureShader);
        GetStorage()->TextureShader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
    }

    void Renderer2D::EndScene() {
        ZIBEN_PROFILE_FUNCTION();
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, GetStorage()->WhiteTexture, color);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(position, size, GetStorage()->WhiteTexture, color);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, glm::vec4(1.0f));
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
        DrawQuad(position, size, texture, glm::vec4(1.0f));
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor) {

        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor) {

        ZIBEN_PROFILE_FUNCTION();

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        Shader::Bind(GetStorage()->TextureShader);
        Texture2D::Bind(texture);

        GetStorage()->TextureShader->SetUniform("u_Transform", translation * scaling);
        GetStorage()->TextureShader->SetUniform("u_TilingFactor", 1.0f);
        GetStorage()->TextureShader->SetUniform("u_Color", tintColor);

        VertexArray::Bind(GetStorage()->QuadVertexArray);
        RenderCommand::DrawIndexed(GetStorage()->QuadVertexArray);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor) {

        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor) {

        ZIBEN_PROFILE_FUNCTION();

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        Shader::Bind(GetStorage()->TextureShader);
        Texture2D::Bind(texture);

        GetStorage()->TextureShader->SetUniform("u_Transform", translation * scaling);
        GetStorage()->TextureShader->SetUniform("u_TilingFactor", tilingFactor);
        GetStorage()->TextureShader->SetUniform("u_Color", glm::vec4(1.0f));

        VertexArray::Bind(GetStorage()->QuadVertexArray);
        RenderCommand::DrawIndexed(GetStorage()->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float            angle,
        const glm::vec4& color) {

        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, GetStorage()->WhiteTexture, color);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3& position,
        const glm::vec2& size,
        float            angle,
        const glm::vec4& color) {

        DrawRotatedQuad(position, size, angle, GetStorage()->WhiteTexture, color);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture) {

        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, glm::vec4(1.0f));
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture) {

        DrawRotatedQuad(position, size, angle, texture, glm::vec4(1.0f));
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor) {

        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tintColor);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor) {

        ZIBEN_PROFILE_FUNCTION();

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation    = glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f });
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        Shader::Bind(GetStorage()->TextureShader);
        Texture2D::Bind(texture);

        GetStorage()->TextureShader->SetUniform("u_Transform", translation * rotation * scaling);
        GetStorage()->TextureShader->SetUniform("u_TilingFactor", 1.0f);
        GetStorage()->TextureShader->SetUniform("u_Color", tintColor);

        VertexArray::Bind(GetStorage()->QuadVertexArray);
        RenderCommand::DrawIndexed(GetStorage()->QuadVertexArray);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        float                 tilingFactor) {

        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tilingFactor);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        float                 tilingFactor) {

        ZIBEN_PROFILE_FUNCTION();

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation    = glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f });
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });

        Shader::Bind(GetStorage()->TextureShader);
        Texture2D::Bind(texture);

        GetStorage()->TextureShader->SetUniform("u_Transform", translation * rotation * scaling);
        GetStorage()->TextureShader->SetUniform("u_TilingFactor", tilingFactor);
        GetStorage()->TextureShader->SetUniform("u_Color", glm::vec4(1.0f));

        VertexArray::Bind(GetStorage()->QuadVertexArray);
        RenderCommand::DrawIndexed(GetStorage()->QuadVertexArray);
    }

    Scope<Renderer2D::Renderer2DStorage>& Renderer2D::GetStorage() {
        static auto storage = CreateScope<Renderer2D::Renderer2DStorage>();
        return storage;
    }

} // namespace Ziben