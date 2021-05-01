#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.hpp"

#include "Ziben/System.hpp"

namespace Ziben {

    void Renderer2D::Init() {
        ZIBEN_PROFILE_FUNCTION();

        // Quad VertexBuffer
        GetStorage().QuadVertexBuffer = VertexBuffer::Create(Storage::MaxVertexCount * sizeof(QuadVertex));
        GetStorage().QuadVertexBuffer->SetLayout({
            { ShaderData::Type::Float3, "VertexPosition" },
            { ShaderData::Type::Float4, "Color"          },
            { ShaderData::Type::Float2, "TexCoord"       },
            { ShaderData::Type::Float,  "TexIndex"       },
            { ShaderData::Type::Float,  "TilingFactor"   }
        });

        GetStorage().QuadVertexBufferBase = new QuadVertex[Storage::MaxVertexCount];

        // Quad IndexBuffer
        auto* quadIndices = new IndexType[Storage::MaxIndexCount];

        for (IndexType offset = 0, i = 0; i < Storage::MaxIndexCount; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        auto quadIndexBuffer  = IndexBuffer::Create(quadIndices, Storage::MaxIndexCount);

        delete[] quadIndices;

        // Quad VertexArray
        GetStorage().QuadVertexArray = VertexArray::Create();
        GetStorage().QuadVertexArray->PushVertexBuffer(GetStorage().QuadVertexBuffer);
        GetStorage().QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

        // White Texture
        uint32_t whiteTextureData = 0xffffffff;

        GetStorage().WhiteTexture = Texture2D::Create(1, 1);
        GetStorage().WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

        // Texture Shader
        std::array<int, Storage::MaxTextureSlots> samples = { 0 };
        std::iota(samples.begin(), samples.end(), 0);

        Shader::Bind(GetStorage().TextureShader = Shader::Create("Assets/Shaders/TextureShader.glsl"));
        GetStorage().TextureShader->SetUniform("u_Textures", samples.data(), samples.size());

        // TextureSlots
        GetStorage().TextureSlots.front() = GetStorage().WhiteTexture;
    }

    void Renderer2D::Shutdown() {
        ZIBEN_PROFILE_FUNCTION();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        ZIBEN_PROFILE_FUNCTION();

        Shader::Bind(GetStorage().TextureShader);
        GetStorage().TextureShader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

        GetStorage().QuadIndexCount          = 0;
        GetStorage().TextureSlotIndex        = 1;
        GetStorage().QuadVertexBufferPointer = GetStorage().QuadVertexBufferBase;
    }

    void Renderer2D::EndScene() {
        ZIBEN_PROFILE_FUNCTION();

        GetStorage().QuadVertexBuffer->SetData(
            GetStorage().QuadVertexBufferBase,
            (uint8_t*)GetStorage().QuadVertexBufferPointer - (uint8_t*)GetStorage().QuadVertexBufferBase
        );

        Flush();
    }

    void Renderer2D::Flush() {
        for (uint32_t i = 0; i < GetStorage().TextureSlotIndex; ++i)
            Texture2D::Bind(GetStorage().TextureSlots[i], i);

        RenderCommand::DrawIndexed(GetStorage().QuadVertexArray, GetStorage().QuadIndexCount);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, GetStorage().WhiteTexture, color, 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(position, size, GetStorage().WhiteTexture, color, 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture) {
        DrawQuad(position, size, texture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor) {

        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor) {

        DrawQuad(position, size, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor) {

        DrawQuad({ position.x, position.y, 0.0f }, size, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor) {

        DrawQuad(position, size, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor,
        float                 tilingFactor) {

        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor,
        float                 tilingFactor) {

        float textureIndex = -1.0f;

        for (std::size_t i = 1; i < GetStorage().TextureSlotIndex; ++i) {
            if (*GetStorage().TextureSlots[i] == *texture) {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == -1.0f) {
            textureIndex                                             = static_cast<float>(GetStorage().TextureSlotIndex);
            GetStorage().TextureSlots[GetStorage().TextureSlotIndex] = texture;
            GetStorage().TextureSlotIndex++;
        }

        GetStorage().QuadVertexBufferPointer->Position     = position;
        GetStorage().QuadVertexBufferPointer->Color        = tintColor;
        GetStorage().QuadVertexBufferPointer->TexCoord     = { 0.0f, 0.0f };
        GetStorage().QuadVertexBufferPointer->TexIndex     = textureIndex;
        GetStorage().QuadVertexBufferPointer->TilingFactor = tilingFactor;
        GetStorage().QuadVertexBufferPointer++;

        GetStorage().QuadVertexBufferPointer->Position     = { position.x + size.x, position.y, position.z };
        GetStorage().QuadVertexBufferPointer->Color        = tintColor;
        GetStorage().QuadVertexBufferPointer->TexCoord     = { 1.0f, 0.0f };
        GetStorage().QuadVertexBufferPointer->TexIndex     = textureIndex;
        GetStorage().QuadVertexBufferPointer->TilingFactor = tilingFactor;
        GetStorage().QuadVertexBufferPointer++;

        GetStorage().QuadVertexBufferPointer->Position     = { position.x + size.x, position.y + size.y, position.z };
        GetStorage().QuadVertexBufferPointer->Color        = tintColor;
        GetStorage().QuadVertexBufferPointer->TexCoord     = { 1.0f, 1.0f };
        GetStorage().QuadVertexBufferPointer->TexIndex     = textureIndex;
        GetStorage().QuadVertexBufferPointer->TilingFactor = tilingFactor;
        GetStorage().QuadVertexBufferPointer++;

        GetStorage().QuadVertexBufferPointer->Position     = { position.x, position.y + size.y, position.z };
        GetStorage().QuadVertexBufferPointer->Color        = tintColor;
        GetStorage().QuadVertexBufferPointer->TexCoord     = { 0.0f, 1.0f };
        GetStorage().QuadVertexBufferPointer->TexIndex     = textureIndex;
        GetStorage().QuadVertexBufferPointer->TilingFactor = tilingFactor;
        GetStorage().QuadVertexBufferPointer++;

        GetStorage().QuadIndexCount += 6;
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float            angle,
        const glm::vec4& color) {

        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, GetStorage().WhiteTexture, color);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3& position,
        const glm::vec2& size,
        float            angle,
        const glm::vec4& color) {

        DrawRotatedQuad(position, size, angle, GetStorage().WhiteTexture, color);
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

        Shader::Bind(GetStorage().TextureShader);
        Texture2D::Bind(texture);

        GetStorage().TextureShader->SetUniform("u_Transform", translation * rotation * scaling);
        GetStorage().TextureShader->SetUniform("u_TilingFactor", 1.0f);
        GetStorage().TextureShader->SetUniform("u_Color", tintColor);

        VertexArray::Bind(GetStorage().QuadVertexArray);
        RenderCommand::DrawIndexed(GetStorage().QuadVertexArray);
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

        Shader::Bind(GetStorage().TextureShader);
        Texture2D::Bind(texture);

        GetStorage().TextureShader->SetUniform("u_Transform", translation * rotation * scaling);
        GetStorage().TextureShader->SetUniform("u_TilingFactor", tilingFactor);
        GetStorage().TextureShader->SetUniform("u_Color", glm::vec4(1.0f));

        VertexArray::Bind(GetStorage().QuadVertexArray);
        RenderCommand::DrawIndexed(GetStorage().QuadVertexArray);
    }

    Renderer2D::Storage& Renderer2D::GetStorage() {
        static Renderer2D::Storage storage;
        return storage;
    }

} // namespace Ziben