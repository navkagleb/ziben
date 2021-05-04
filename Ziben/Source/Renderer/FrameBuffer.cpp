#include "FrameBuffer.hpp"

namespace Ziben {

    Ref<FrameBuffer> FrameBuffer::Create(const FrameBufferSpecification& specification) {
        return CreateRef<FrameBuffer>(specification);
    }

    void FrameBuffer::Bind(const Ref<FrameBuffer>& frameBuffer) {
        glBindFramebuffer(GL_FRAMEBUFFER, frameBuffer->m_Handle);
    }

    void FrameBuffer::Unbind() {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    FrameBuffer::FrameBuffer(const FrameBufferSpecification& specification)
        : m_Handle(0)
        , m_ColorAttachment(0)
        , m_DepthAttachment(0)
        , m_Specification(specification) {

        Invalidate();
    }

    FrameBuffer::~FrameBuffer() {
        if (m_Handle)
            glDeleteFramebuffers(1, &m_Handle);
    }

    void FrameBuffer::Invalidate() {
        glCreateFramebuffers(1, &m_Handle);
        glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);
        glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);
        glTexImage2D(
            GL_TEXTURE_2D,
            0,
            GL_RGBA8,
            static_cast<GLsizei>(m_Specification.Width),
            static_cast<GLsizei>(m_Specification.Height),
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            nullptr
        );

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);

        glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);
        glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
//        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, nullptr)
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);

        assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

} // namespace Ziben