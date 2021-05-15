#include "Renderer2D.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include "RenderCommand.hpp"
#include "EditorCamera.hpp"
#include "Ziben/Scene/Component.hpp"

namespace Ziben {

    void Renderer2D::Init() {
        ZIBEN_PROFILE_FUNCTION();

        // Quad VertexBuffer
        GetData().QuadVertexBuffer = VertexBuffer::Create(s_MaxVertexCount * sizeof(QuadVertex));
        GetData().QuadVertexBuffer->SetLayout({
            { ShaderData::Type::Float3, "VertexPosition" },
            { ShaderData::Type::Float4, "Color"          },
            { ShaderData::Type::Float2, "TexCoord"       },
            { ShaderData::Type::Float,  "TexIndex"       },
            { ShaderData::Type::Float,  "TilingFactor"   },
            { ShaderData::Type::Int,    "EntityHandle"   }
        });

        GetData().QuadVertexBufferBase = new QuadVertex[s_MaxVertexCount];

        // Quad IndexBuffer
        auto* quadIndices = new IndexType[s_MaxIndexCount];

        for (IndexType offset = 0, i = 0; i < s_MaxIndexCount; i += 6) {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }

        auto quadIndexBuffer = IndexBuffer::Create(quadIndices, s_MaxIndexCount);

        delete[] quadIndices;

        // Quad VertexArray
        GetData().QuadVertexArray = VertexArray::Create();
        GetData().QuadVertexArray->PushVertexBuffer(GetData().QuadVertexBuffer);
        GetData().QuadVertexArray->SetIndexBuffer(quadIndexBuffer);

        // White Texture
        uint32_t whiteTextureData = 0xffffffff;

        GetData().WhiteTexture = Texture2D::Create(1, 1);
        GetData().WhiteTexture->SetData(&whiteTextureData, sizeof(whiteTextureData));

        // Texture Shader
        std::array<int, s_MaxTextureSlots> samples = { 0 };
        std::iota(samples.begin(), samples.end(), 0);

        Shader::Bind(GetData().TextureShader = Shader::Create("Assets/Shaders/TextureShader.glsl"));
        GetData().TextureShader->SetUniform("u_Textures", samples.data(), samples.size());

        // TextureSlots
        GetData().TextureSlots.front() = GetData().WhiteTexture;
    }

    void Renderer2D::Shutdown() {
        ZIBEN_PROFILE_FUNCTION();

        delete[] GetData().QuadVertexBufferBase;
    }

    void Renderer2D::BeginScene(const Camera& camera, const glm::mat4& transform) {
        ZIBEN_PROFILE_FUNCTION();

        Shader::Bind(GetData().TextureShader);
        GetData().TextureShader->SetUniform(
            "u_ViewProjectionMatrix",
            camera.GetProjectionMatrix() * glm::inverse(transform)
        );

        StartBatch();
    }

    void Renderer2D::BeginScene(const EditorCamera& camera) {
        ZIBEN_PROFILE_FUNCTION();

        Shader::Bind(GetData().TextureShader);
        GetData().TextureShader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void Renderer2D::BeginScene(const OrthographicCamera& camera) {
        ZIBEN_PROFILE_FUNCTION();

        Shader::Bind(GetData().TextureShader);
        GetData().TextureShader->SetUniform("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());

        StartBatch();
    }

    void Renderer2D::EndScene() {
        ZIBEN_PROFILE_FUNCTION();

        Flush();

        std::fill(GetData().QuadVertexBufferBase, GetData().QuadVertexBufferBase + s_MaxVertexCount, QuadVertex());
    }

    void Renderer2D::Flush() {
        if (GetData().QuadIndexCount == 0)
            return;

        GetData().QuadVertexBuffer->SetData(
            GetData().QuadVertexBufferBase,
            s_MaxVertexCount
//            (uint8_t*)GetData().QuadVertexBufferPointer - (uint8_t*)GetData().QuadVertexBufferBase
        );

        // Bind Textures
        for (uint32_t i = 0; i < GetData().TextureSlotIndex; ++i)
            Texture2D::Bind(GetData().TextureSlots[i], i);

        Shader::Bind(GetData().TextureShader);
        RenderCommand::DrawIndexed(GetData().QuadVertexArray, GetData().QuadIndexCount);

        ++GetStatistics().DrawCalls;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad({ position.x, position.y, 0.0f }, size, GetData().WhiteTexture, color, 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color) {
        DrawQuad(position, size, GetData().WhiteTexture, color, 1.0f);
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
        const glm::vec4&      tintColor
    ) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor
    ) {
        DrawQuad(position, size, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor
    ) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        float                 tilingFactor
    ) {
        DrawQuad(position, size, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor,
        float                 tilingFactor
    ) {
        DrawQuad({ position.x, position.y, 0.0f }, size, texture, tintColor, tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor,
        float                 tilingFactor
    ) {
        ZIBEN_PROFILE_FUNCTION();

        if (GetData().QuadIndexCount >= s_MaxIndexCount)
            NextBatch();

        uint32_t textureIndex;

        auto texturePosition = std::find_if(
            GetData().TextureSlots.begin(),
            GetData().TextureSlots.begin() + GetData().TextureSlotIndex,
            [&](const Ref<Texture2D>& textureSlot) {
                return *textureSlot == *texture;
            }
        );

        if (texturePosition != GetData().TextureSlots.begin() + GetData().TextureSlotIndex) {
            textureIndex = std::distance(GetData().TextureSlots.begin(), texturePosition);
        } else {
            textureIndex                                         = GetData().TextureSlotIndex;
            GetData().TextureSlots[GetData().TextureSlotIndex++] = texture;
        }

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        glm::mat4 transform   = translation * scaling;

        for (uint32_t i = 0; i < 4; ++i) {
            GetData().QuadVertexBufferPointer->Position     = transform * s_QuadVertexPositions[i];
            GetData().QuadVertexBufferPointer->Color        = tintColor;
            GetData().QuadVertexBufferPointer->TexCoord     = s_QuadTexCoords[i];
            GetData().QuadVertexBufferPointer->TexIndex     = static_cast<float>(textureIndex);
            GetData().QuadVertexBufferPointer->TilingFactor = tilingFactor;
            GetData().QuadVertexBufferPointer++;
        }

        GetData().QuadIndexCount += 6;

        ++GetStatistics().QuadCount;
    }

    void Renderer2D::DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture) {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture) {
        DrawQuad(position, size, subTexture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&         position,
        const glm::vec2&         size,
        const Ref<SubTexture2D>& subTexture,
        const glm::vec4&         tintColor
    ) {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&         position,
        const glm::vec2&         size,
        const Ref<SubTexture2D>& subTexture,
        const glm::vec4&         tintColor
    ) {
        DrawQuad(position, size, subTexture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&         position,
        const glm::vec2&         size,
        const Ref<SubTexture2D>& subTexture,
        float                    tilingFactor
    ) {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&         position,
        const glm::vec2&         size,
        const Ref<SubTexture2D>& subTexture,
        float                    tilingFactor
    ) {
        DrawQuad(position, size, subTexture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec2&         position,
        const glm::vec2&         size,
        const Ref<SubTexture2D>& subTexture,
        const glm::vec4&         tintColor,
        float                    tilingFactor
    ) {
        DrawQuad({ position.x, position.y, 0.0f }, size, subTexture, tintColor, tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::vec3&         position,
        const glm::vec2&         size,
        const Ref<SubTexture2D>& subTexture,
        const glm::vec4&         tintColor,
        float                    tilingFactor
    ) {
        ZIBEN_PROFILE_FUNCTION();

        if (GetData().QuadIndexCount >= s_MaxIndexCount)
            NextBatch();

        uint32_t textureIndex;

        auto texturePosition = std::find_if(
            GetData().TextureSlots.begin(),
            GetData().TextureSlots.begin() + GetData().TextureSlotIndex,
            [&](const Ref<Texture2D>& textureSlot) {
                return *textureSlot == *subTexture->GetTexture();
            }
        );

        if (texturePosition != GetData().TextureSlots.begin() + GetData().TextureSlotIndex) {
            textureIndex = std::distance(GetData().TextureSlots.begin(), texturePosition);
        } else {
            textureIndex                                         = GetData().TextureSlotIndex;
            GetData().TextureSlots[GetData().TextureSlotIndex++] = subTexture->GetTexture();
        }

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        glm::mat4 transform   = translation * scaling;

        for (uint32_t i = 0; i < 4; ++i) {
            GetData().QuadVertexBufferPointer->Position     = transform * s_QuadVertexPositions[i];
            GetData().QuadVertexBufferPointer->Color        = tintColor;
            GetData().QuadVertexBufferPointer->TexCoord     = subTexture->GetTexCoords()[i];
            GetData().QuadVertexBufferPointer->TexIndex     = static_cast<float>(textureIndex);
            GetData().QuadVertexBufferPointer->TilingFactor = tilingFactor;
            GetData().QuadVertexBufferPointer++;
        }

        GetData().QuadIndexCount += 6;

        ++GetStatistics().QuadCount;
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color) {
        DrawQuad(transform, GetData().WhiteTexture, color, 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture) {
        DrawQuad(transform, texture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor) {
        DrawQuad(transform, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor) {
        DrawQuad(transform, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawQuad(
        const glm::mat4& transform,
        const Ref<Texture2D>& texture,
        const glm::vec4& tintColor,
        float tilingFactor,
        int   entityHandle
    ) {
        ZIBEN_PROFILE_FUNCTION();

        if (GetData().QuadIndexCount >= s_MaxIndexCount)
            NextBatch();

        uint32_t textureIndex;

        auto texturePosition = std::find_if(
            GetData().TextureSlots.begin(),
            GetData().TextureSlots.begin() + GetData().TextureSlotIndex,
            [&](const Ref<Texture2D>& textureSlot) {
                return *textureSlot == *texture;
            }
        );

        if (texturePosition != GetData().TextureSlots.begin() + GetData().TextureSlotIndex) {
            textureIndex = std::distance(GetData().TextureSlots.begin(), texturePosition);
        } else {
            textureIndex                                         = GetData().TextureSlotIndex;
            GetData().TextureSlots[GetData().TextureSlotIndex++] = texture;
        }

        for (uint32_t i = 0; i < 4; ++i) {
            GetData().QuadVertexBufferPointer->Position     = transform * s_QuadVertexPositions[i];
            GetData().QuadVertexBufferPointer->Color        = tintColor;
            GetData().QuadVertexBufferPointer->TexCoord     = s_QuadTexCoords[i];
            GetData().QuadVertexBufferPointer->TexIndex     = static_cast<float>(textureIndex);
            GetData().QuadVertexBufferPointer->TilingFactor = tilingFactor;
            GetData().QuadVertexBufferPointer->EntityHandle = entityHandle;
            GetData().QuadVertexBufferPointer++;
        }

        GetData().QuadIndexCount += 6;

        ++GetStatistics().QuadCount;
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2& position,
        const glm::vec2& size,
        float            angle,
        const glm::vec4& color
    ) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, GetData().WhiteTexture, color, 1.0f);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3& position,
        const glm::vec2& size,
        float            angle,
        const glm::vec4& color
    ) {
        DrawRotatedQuad(position, size, angle, GetData().WhiteTexture, color, 1.0f);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture
    ) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture
    ) {
        DrawRotatedQuad(position, size, angle, texture, glm::vec4(1.0f), 1.0f);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor
    ) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor
    ) {
        DrawRotatedQuad(position, size, angle, texture, tintColor, 1.0f);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        float                 tilingFactor
    ) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        float                 tilingFactor
    ) {
        DrawRotatedQuad(position, size, angle, texture, glm::vec4(1.0f), tilingFactor);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec2&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor,
        float                 tilingFactor
    ) {
        DrawRotatedQuad({ position.x, position.y, 0.0f }, size, angle, texture, tintColor, tilingFactor);
    }

    void Renderer2D::DrawRotatedQuad(
        const glm::vec3&      position,
        const glm::vec2&      size,
        float                 angle,
        const Ref<Texture2D>& texture,
        const glm::vec4&      tintColor,
        float                 tilingFactor
    ) {
        ZIBEN_PROFILE_FUNCTION();

        if (GetData().QuadIndexCount >= s_MaxIndexCount)
            NextBatch();

        uint32_t textureIndex;

        auto texturePosition = std::find_if(
            GetData().TextureSlots.begin(),
            GetData().TextureSlots.begin() + GetData().TextureSlotIndex,
            [&](const Ref<Texture2D>& textureSlot) {
                return *textureSlot == *texture;
            }
        );

        if (texturePosition != GetData().TextureSlots.begin() + GetData().TextureSlotIndex) {
            textureIndex = std::distance(GetData().TextureSlots.begin(), texturePosition);
        } else {
            textureIndex                                         = GetData().TextureSlotIndex;
            GetData().TextureSlots[GetData().TextureSlotIndex++] = texture;
        }

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotation    = glm::rotate(glm::mat4(1.0f), angle, { 0.0f, 0.0f, 1.0f });
        glm::mat4 scaling     = glm::scale(glm::mat4(1.0f), { size.x, size.y, 1.0f });
        glm::mat4 transform   = translation * rotation * scaling;

        for (uint32_t i = 0; i < 4; ++i) {
            GetData().QuadVertexBufferPointer->Position     = transform * s_QuadVertexPositions[i];
            GetData().QuadVertexBufferPointer->Color        = tintColor;
            GetData().QuadVertexBufferPointer->TexCoord     = s_QuadTexCoords[i];
            GetData().QuadVertexBufferPointer->TexIndex     = static_cast<float>(textureIndex);
            GetData().QuadVertexBufferPointer->TilingFactor = tilingFactor;
            GetData().QuadVertexBufferPointer++;
        }

        GetData().QuadIndexCount += 6;

        ++GetStatistics().QuadCount;
    }

    void Renderer2D::DrawSprite(
        const glm::mat4&               transform,
        const SpriteRendererComponent& spriteRendererComponent,
        int                            entityHandle
    ) {
        DrawQuad(transform, GetData().WhiteTexture, spriteRendererComponent.Color, 1.0f, entityHandle);
    }

    Renderer2D::Statistics& Renderer2D::GetStatistics() {
        static Renderer2D::Statistics statistics;
        return statistics;
    }

    void Renderer2D::ResetStatistics() {
        GetStatistics().DrawCalls = 0;
        GetStatistics().QuadCount = 0;
    }

    Renderer2D::Data& Renderer2D::GetData() {
        static Renderer2D::Data data;
        return data;
    }

    void Renderer2D::StartBatch() {
        GetData().QuadIndexCount          = 0;
        GetData().TextureSlotIndex        = 1;
        GetData().QuadVertexBufferPointer = GetData().QuadVertexBufferBase;
    }

    void Renderer2D::NextBatch() {
        Flush();
        StartBatch();
    }

} // namespace Ziben