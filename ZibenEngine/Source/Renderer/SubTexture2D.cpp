#include "SubTexture2D.hpp"

namespace Ziben {

    Ref<SubTexture2D> SubTexture2D::CreateFromCoords(
        const Ref<Texture2D>& texture,
        const glm::vec2&      position,
        const glm::vec2&      cellSize,
        const glm::vec2&      spriteSize
    ) {
        auto sheetWidth  = static_cast<float>(texture->GetWidth());
        auto sheetHeight = static_cast<float>(texture->GetHeight());

        glm::vec2 min = { position.x                  * cellSize.x / sheetWidth, position.y                  * cellSize.y / sheetHeight };
        glm::vec2 max = { (position.x + spriteSize.x) * cellSize.x / sheetWidth, (position.y + spriteSize.y) * cellSize.y / sheetHeight };

        return CreateRef<SubTexture2D>(texture, min, max);
    }

    SubTexture2D::SubTexture2D(const Ref<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max)
        : m_Texture(texture)
        , m_TexCoords({
            glm::vec2(min.x, min.y),
            glm::vec2(max.x, min.y),
            glm::vec2(max.x, max.y),
            glm::vec2(min.x, max.y)
        }) {}

} // namespace Ziben