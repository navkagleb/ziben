#pragma once

#include <glm/glm.hpp>

#include "Texture.hpp"

namespace Ziben {

    class SubTexture2D {
    public:
        static Ref<SubTexture2D> CreateFromCoords(
            const Ref<Texture2D>& texture,
            const glm::vec2&      position,
            const glm::vec2&      cellSize,
            const glm::vec2&      spriteSize = { 1, 1 }
        );

    public:
        SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

        [[nodiscard]] inline const Ref<Texture2D>& GetTexture() const { return m_Texture; }
        [[nodiscard]] inline const glm::vec2* GetTexCoords() const { return m_TexCoords.data(); }

    private:
        const Ref<Texture2D>&    m_Texture;
        std::array<glm::vec2, 4> m_TexCoords;

    }; // class SubTexture2D

} // namespace Ziben