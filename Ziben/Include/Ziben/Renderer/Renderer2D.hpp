#pragma once

#include "OrthographicCamera.hpp"
#include "VertexArray.hpp"
#include "Shader.hpp"
#include "Texture.hpp"

namespace Ziben {

    class Renderer2D {
    public:
        struct QuadVertex {
            glm::vec3 Position;
            glm::vec4 Color;
            glm::vec2 TexCoord;
        };

    public:
        static void Init();
        static void Shutdown();

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

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const glm::vec4& color);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const glm::vec4& color);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec4& tintColor);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, const glm::vec4& tintColor);

        static void DrawRotatedQuad(const glm::vec2& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, float tilingFactor);
        static void DrawRotatedQuad(const glm::vec3& position, const glm::vec2& size, float angle, const Ref<Texture2D>& texture, float tilingFactor);

    private:
        struct Storage {
            static constexpr uint32_t MaxQuadCount   = 10'000;
            static constexpr uint32_t MaxVertexCount = MaxQuadCount * 4;
            static constexpr uint32_t MaxIndexCount  = MaxQuadCount * 6;

            Ref<VertexArray>  QuadVertexArray;
            Ref<VertexBuffer> QuadVertexBuffer;
            Ref<Shader>       TextureShader;
            Ref<Texture2D>    WhiteTexture;

            uint32_t          QuadIndexCount          = 0;
            QuadVertex*       QuadVertexBufferBase    = nullptr;
            QuadVertex*       QuadVertexBufferPointer = nullptr;
        };

    private:
        static Storage& GetStorage();

    }; // class Renderer2D

} // namespace Ziben