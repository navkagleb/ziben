#include "FrameBuffer.hpp"

namespace Ziben {

    namespace Internal {

        static constexpr bool IsDepthFormat(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::Depth24Stencil8: return true;
                default:                                        break;
            }

            return false;
        }

        static inline constexpr uint32_t TextureTarget(bool isMultiSampled) {
            return isMultiSampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;
        }

        static inline void CreateTextures(bool isMultiSampled, HandleType* handles, std::size_t count) {
            glCreateTextures(TextureTarget(isMultiSampled), static_cast<GLsizei>(count), handles);
        }

        static inline void BindTexture(bool isMultiSampled, HandleType handle) {
            glBindTexture(TextureTarget(isMultiSampled), handle);
        }

        static void AttachColorTexture(
            HandleType  handle,
            uint32_t    samples,
            GLenum      internalFormat,
            GLenum      format,
            uint32_t    width,
            uint32_t    height,
            std::size_t index
        ) {
            bool isMultiSampled = samples > 1;

            if (isMultiSampled) {
                glTexImage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    static_cast<GLint>(samples),
                    internalFormat,
                    static_cast<GLsizei>(width),
                    static_cast<GLsizei>(height),
                    GL_FALSE
                );
            } else {
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    static_cast<GLint>(internalFormat),
                    static_cast<GLsizei>(width),
                    static_cast<GLsizei>(height),
                    0,
                    format,
                    GL_UNSIGNED_BYTE,
                    nullptr
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0 + index,
                TextureTarget(isMultiSampled),
                handle,
                0
            );
        }

        static void AttachDepthTexture(
            HandleType  handle,
            uint32_t    samples,
            GLenum      format,
            GLenum      attachmentType,
            uint32_t    width,
            uint32_t    height
        ) {
            bool isMultiSampled = samples > 1;

            if (isMultiSampled) {
                glTexImage2DMultisample(
                    GL_TEXTURE_2D_MULTISAMPLE,
                    static_cast<GLint>(samples),
                    format,
                    static_cast<GLsizei>(width),
                    static_cast<GLsizei>(height),
                    GL_FALSE
                );
            } else {
                glTexStorage2D(
                    GL_TEXTURE_2D,
                    1,
                    format,
                    static_cast<GLsizei>(width),
                    static_cast<GLsizei>(height)
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            }

            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                attachmentType,
                TextureTarget(isMultiSampled),
                handle,
                0
            );
        }

        static constexpr GLenum ZibenFrameBufferTextureFormatToGL(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::RGBA8:      return GL_RGB8;
                case FrameBufferTextureFormat::RedInteger: return GL_RED_INTEGER;
                default:                                   break;
            }

            assert(false);
            return 0;
        }

        static constexpr GLenum GetGLDataTypeFromZibenFrameBufferTextureFormat(FrameBufferTextureFormat format) {
            switch (format) {
                case FrameBufferTextureFormat::RGBA8:      return GL_UNSIGNED_BYTE;
                case FrameBufferTextureFormat::RedInteger: return GL_INT;
                default:                                   break;
            }

            assert(false);
            return 0;
        }

    } // namespace Internal

    Ref<FrameBuffer> FrameBuffer::Create(FrameBufferSpecification&& specification) {
        return CreateRef<FrameBuffer>(std::move(specification));
    }

    void FrameBuffer::Bind(const Ref<FrameBuffer>& frameBuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->m_Handle);
        glViewport(
            0,
            0,
            static_cast<GLsizei>(frameBuffer->m_Specification.Width),
            static_cast<GLsizei>(frameBuffer->m_Specification.Height)
        );
    }

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FrameBuffer::FrameBuffer(FrameBufferSpecification&& specification)
        : m_Handle(0)
        , m_Specification(specification)
        , m_DepthAttachment(0) {

        for (auto textureSpecification : m_Specification.Attachments.Attachments) {
            if (!Internal::IsDepthFormat(textureSpecification.TextureFormat))
                m_ColorAttachmentSpecification.emplace_back(textureSpecification);
            else
                m_DepthAttachmentSpecification = textureSpecification;
        }

        Invalidate();
    }

    FrameBuffer::~FrameBuffer() {
        Clear();
    }

    HandleType FrameBuffer::GetColorAttachmentHandle(std::size_t index) const {
        assert(index < m_ColorAttachments.size());
        return m_ColorAttachments[index];
    }

    void FrameBuffer::Invalidate() {
        if (m_Handle)
            Clear();

        glCreateFramebuffers(1, &m_Handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

        bool isMultiSampled = m_Specification.Samples > 1;

        // Attachments
        if (!m_ColorAttachmentSpecification.empty()) {
            m_ColorAttachments.resize(m_ColorAttachmentSpecification.size());

            Internal::CreateTextures(isMultiSampled, m_ColorAttachments.data(), m_ColorAttachments.size());

            for (std::size_t i = 0; i < m_ColorAttachments.size(); ++i) {
                Internal::BindTexture(isMultiSampled, m_ColorAttachments[i]);

                switch (m_ColorAttachmentSpecification[i].TextureFormat) {
                    case FrameBufferTextureFormat::RGBA8: {
                        Internal::AttachColorTexture(
                            m_ColorAttachments[i],
                            m_Specification.Samples,
                            GL_RGBA8,
                            GL_RGBA,
                            m_Specification.Width,
                            m_Specification.Height,
                            i
                        );

                        break;
                    }

                    case FrameBufferTextureFormat::RedInteger: {
                        Internal::AttachColorTexture(
                            m_ColorAttachments[i],
                            m_Specification.Samples,
                            GL_R32I,
                            GL_RED_INTEGER,
                            m_Specification.Width,
                            m_Specification.Height,
                            i
                        );

                        break;
                    }

                    default: break;
                }
            }
        }

        if (m_DepthAttachmentSpecification.TextureFormat != FrameBufferTextureFormat::None) {
            Internal::CreateTextures(isMultiSampled, &m_DepthAttachment, 1);
            Internal::BindTexture(isMultiSampled, m_DepthAttachment);

            switch (m_DepthAttachmentSpecification.TextureFormat) {
                case FrameBufferTextureFormat::Depth24Stencil8: {
                    Internal::AttachDepthTexture(
                        m_DepthAttachment,
                        m_Specification.Samples,
                        GL_DEPTH24_STENCIL8,
                        GL_DEPTH_STENCIL_ATTACHMENT,
                        m_Specification.Width,
                        m_Specification.Height
                    );

                    break;
                }

                default: break;
            }
        }

        if (m_ColorAttachments.empty()) {
            // Only depth-pass
            glDrawBuffer(GL_NONE);
        } else {
            assert(m_ColorAttachments.size() <= 4);

            GLenum buffers[4] = {
                GL_COLOR_ATTACHMENT0,
                GL_COLOR_ATTACHMENT1,
                GL_COLOR_ATTACHMENT2,
                GL_COLOR_ATTACHMENT3
            };

            glDrawBuffers(static_cast<GLsizei>(m_ColorAttachments.size()), buffers);
        }

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void FrameBuffer::Resize(uint32_t width, uint32_t height) {
        if (width == 0 || height == 0 || width > s_MaxFrameBufferSize || height > s_MaxFrameBufferSize) {
            ZIBEN_WARN("Can't create FrameBuffer with such parameters: {0} {1}", width, height);
            return;
        }

        m_Specification.Width = width;
        m_Specification.Height = height;

        Invalidate();
    }

    void FrameBuffer::ClearColorAttachment(std::size_t index, int value) {
        auto& specification = m_ColorAttachmentSpecification[index];

        specification.TextureFormat;

        glClearTexImage(
            m_ColorAttachments[index],
            0,
            Internal::ZibenFrameBufferTextureFormatToGL(specification.TextureFormat),
            GL_INT,
            &value
        );
    }

    int FrameBuffer::ReadPixel(uint32_t attachmentIndex, int x, int y) {
        assert(attachmentIndex < m_ColorAttachments.size());

        int pixelData;

        glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);
        glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);

        return pixelData;
    }

    void FrameBuffer::Clear() {
        if (m_Handle) {
            glDeleteFramebuffers(1, &m_Handle);
            m_Handle = 0;
        }

        if (!m_ColorAttachments.empty()) {
            glDeleteTextures(static_cast<GLsizei>(m_ColorAttachments.size()), m_ColorAttachments.data());
            m_ColorAttachments.clear();
        }

        if (m_DepthAttachment) {
            glDeleteTextures(1, &m_DepthAttachment);
            m_DepthAttachment = 0;
        }
    }

} // namespace Ziben