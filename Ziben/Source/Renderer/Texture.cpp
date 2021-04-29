#include "Texture.hpp"

#include <stb_image.h>

#include "Ziben/System/Log.hpp"

namespace Ziben {

    Ref<Texture2D> Texture2D::Create(const std::string& filepath) {
        return CreateRef<Texture2D>(filepath);
    }

    void Texture::Bind(const Ref<Texture>& texture, uint32_t slot) {
        ZIBEN_CORE_ERROR("Ziben::Texture::Bind: Call unimplemented method!");
    }

    void Texture2D::Bind(const Ref<Texture2D>& texture2D, uint32_t slot) {
        glBindTextureUnit(slot, texture2D->m_Handle);
    }

    void Texture2D::Unbind() {
        glBindTextureUnit(0, 0);
    }

    Texture2D::Texture2D(const std::string& filepath)
        : m_Handle(0) {

        int width;
        int height;
        int channels;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        assert(data);

        m_Width  = width;
        m_Height = height;

        GLenum internalFormat = 0;
        GLenum dataFormat     = 0;

        if (channels == 4) {
            internalFormat = GL_RGBA8;
            dataFormat     = GL_RGBA;
        } else if (channels == 3) {
            internalFormat = GL_RGB8;
            dataFormat     = GL_RGB;
        }

        assert(internalFormat && dataFormat);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        glTextureStorage2D(m_Handle, 1, internalFormat, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));

        glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

        glTextureSubImage2D(
            m_Handle,                          // Target
            0,                                 // Level
            0,                                 // Offset x
            0,                                 // Offset y
            static_cast<GLsizei>(m_Width),     // Width
            static_cast<GLsizei>(m_Height),    // Height
            dataFormat,                        // GL format
            GL_UNSIGNED_BYTE,                  // GL type
            data                               // pixels
        );

        stbi_image_free(data);
    }

    Texture2D::~Texture2D() {
        if (m_Handle != 0)
            glDeleteTextures(1, &m_Handle);
    }

} // namespace Ziben