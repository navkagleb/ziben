#pragma once

#include "OrthographicCamera.hpp"
#include "Camera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"
#include "SubTexture2D.hpp"

namespace Ziben {

    class EditorCamera;
    class SpriteRendererComponent;

    class Renderer2D {
    public:
        struct QuadVertex {
            glm::vec3 Position     = glm::vec3(0.0f);
            glm::vec4 Color        = glm::vec4(0.0f);
            glm::vec2 TexCoord     = glm::vec2(0.0f);
            float     TexIndex     = 0.0f;
            float     TilingFactor = 1.0f;

            // Editor only!
            int       EntityHandle = -1;
        };

    public:
        static void Init();
        static void Shutdown();

        static void BeginScene(const Camera& camera, const glm::mat4& transform);
        static void BeginScene(const EditorCamera& camera);
        static void BeginScene(const OrthographicCamera& camera);
        static void EndScene();

        static void Flush();

        // Primitives
        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, float tilingFactor);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, float tilingFactor);

        static void DrawQuad(const glm::vec2& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor);
        static void DrawQuad(const glm::vec3& position, const glm::vec2& size, const Ref<SubTexture2D>& subTexture, const glm::vec4& tintColor, float tilingFactor);

        static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture);
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, const glm::vec4& tintColor);
        static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture, float tilingFactor);
            static void DrawQuad(const glm::mat4& transform, const Ref<Texture2D>& texture,  const glm::vec4& tintColor, float tilingFactor, int entityHandle = -1);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec4& tintColor);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec4& tintColor);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, float tilingFactor);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, float tilingFactor);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec4& tintColor, float tilingFactor);

        static void DrawSprite(const glm::mat4& transform, const SpriteRendererComponent& spriteRendererComponent, int entityHandle);

    public:
        struct Statistics {
            uint32_t DrawCalls = 0;
            uint32_t QuadCount = 0;
        };

        static Statistics& GetStatistics();

        static void ResetStatistics();

    private:
        static constexpr uint32_t                 s_MaxQuadCount        = 20'000;
        static constexpr uint32_t                 s_MaxVertexCount      = s_MaxQuadCount * 4;
        static constexpr uint32_t                 s_MaxIndexCount       = s_MaxQuadCount * 6;
        static constexpr uint32_t                 s_MaxTextureSlots     = 32; // TODO: RenderCaps

        static constexpr std::array<glm::vec4, 4> s_QuadVertexPositions = {
            glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
            glm::vec4( 0.5f, -0.5f, 0.0f, 1.0f),
            glm::vec4( 0.5f,  0.5f, 0.0f, 1.0f),
            glm::vec4(-0.5f,  0.5f, 0.0f, 1.0f)
        };

        static constexpr std::array<glm::vec2, 4> s_QuadTexCoords       = {
            glm::vec2(0.0f,  0.0f),
            glm::vec2(1.0f,  0.0f),
            glm::vec2(1.0f,  1.0f),
            glm::vec2(0.0f,  1.0f)
        };

    private:
        struct Data {
            Ref<VertexArray>                              QuadVertexArray;
            Ref<VertexBuffer>                             QuadVertexBuffer;
            Ref<Shader>                                   TextureShader;
            Ref<Texture2D>                                WhiteTexture;

            uint32_t                                      QuadIndexCount          = 0;
            QuadVertex*                                   QuadVertexBufferBase    = nullptr;
            QuadVertex*                                   QuadVertexBufferPointer = nullptr;

            std::array<Ref<Texture2D>, s_MaxTextureSlots> TextureSlots            = { nullptr };
            uint32_t                                      TextureSlotIndex        = 1; // 0 - WhiteTexture
        };

    private:
        static Data& GetData();

        static void StartBatch();
        static void NextBatch();

    }; // class Renderer2D

} // namespace Ziben