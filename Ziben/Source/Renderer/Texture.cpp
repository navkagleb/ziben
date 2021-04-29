#include "Texture.hpp"

#include <stb_image.h>

#include "Ziben/System/Log.hpp"

namespace Ziben {

    Ref<Texture2D> Texture2D::Create(const std::string& filepath) {
        return CreateRef<Texture2D>(filepath);
    }

    Ref<Texture2D> Texture2D::Create(uint32_t width, uint32_t height) {
        return CreateRef<Texture2D>(width, height);
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
        : m_Handle(0)
        , m_InternalFormat(0)
        , m_DataFormat(0) {

        int width;
        int height;
        int channels;

        stbi_set_flip_vertically_on_load(true);
        stbi_uc* data = stbi_load(filepath.c_str(), &width, &height, &channels, 0);

        assert(data);

        m_Width  = width;
        m_Height = height;

        if (channels == 4) {
            m_InternalFormat = GL_RGBA8;
            m_DataFormat     = GL_RGBA;
        } else if (channels == 3) {
            m_InternalFormat = GL_RGB8;
            m_DataFormat     = GL_RGB;
        }

        assert(m_InternalFormat && m_DataFormat);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        glTextureStorage2D(m_Handle, 1, m_InternalFormat, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));

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
            m_DataFormat,                      // GL format
            GL_UNSIGNED_BYTE,                  // GL type
            data                               // pixels
        );

        stbi_image_free(data);
    }

    Texture2D::Texture2D(uint32_t width, uint32_t height)
        : m_Handle(0)
        , m_Width(width)
        , m_Height(height)
        , m_InternalFormat(GL_RGBA8)
        , m_DataFormat(GL_RGBA) {

        glCreateTextures(GL_TEXTURE_2D, 1, &m_Handle);
        glTextureStorage2D(m_Handle, 1, m_InternalFormat, static_cast<GLsizei>(m_Width), static_cast<GLsizei>(m_Height));

        glTextureParameteri(m_Handle, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_Handle, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTextureParameteri(m_Handle, GL_TEXTURE_WRAP_T, GL_REPEAT);

    }

    Texture2D::~Texture2D() {
        if (m_Handle != 0)
            glDeleteTextures(1, &m_Handle);
    }

    void Texture2D::SetData(void* data, uint32_t size) {
        glTextureSubImage2D(
            m_Handle,                          // Target
            0,                                 // Level
            0,                                 // Offset x
            0,                                 // Offset y
            static_cast<GLsizei>(m_Width),     // Width
            static_cast<GLsizei>(m_Height),    // Height
            m_DataFormat,                      // GL format
            GL_UNSIGNED_BYTE,                  // GL type
            data                               // pixels
        );
    }

} // namespace Ziben