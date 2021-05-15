#pragma once

#include "GraphicsCore.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace Ziben {

    enum class FrameBufferTextureFormat : uint32_t {
        None = 0,

        // Color
        RGBA8,
        RedInteger,

        // Depth / Stencil
        Depth24Stencil8,

        // Defaults
        Depth = Depth24Stencil8
    };

    struct FrameBufferTextureSpecification {
        FrameBufferTextureFormat TextureFormat = FrameBufferTextureFormat::None;

//        FrameBufferTextureSpecification() = default;
//        FrameBufferTextureSpecification(FrameBufferTextureFormat format)
//            : TextureFormat(format) {}
    };

    struct FrameBufferAttachmentSpecification {
        std::vector<FrameBufferTextureSpecification> Attachments;

        FrameBufferAttachmentSpecification() = default;
        FrameBufferAttachmentSpecification(std::initializer_list<FrameBufferTextureSpecification> attachments)
            : Attachments(attachments) {}
    };

    struct FrameBufferSpecification {
        uint32_t                           Width           = 0;
        uint32_t                           Height          = 0;
        uint32_t                           Samples         = 1;
        FrameBufferAttachmentSpecification Attachments;

        bool                               SwapChainTarget = false;
    };

    class FrameBuffer {
    public:
        static Ref<FrameBuffer> Create(FrameBufferSpecification&& specification);

        static void Bind(const Ref<FrameBuffer>& frameBuffer);
        static void Unbind();

    public:
        explicit FrameBuffer(FrameBufferSpecification&& specification);
        ~FrameBuffer();

    public:
        [[nodiscard]] inline const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

        [[nodiscard]] HandleType GetColorAttachmentHandle(std::size_t index = 0) const;

        void Invalidate();
        void Resize(uint32_t width, uint32_t height);
        void ClearColorAttachment(std::size_t index, int value);

        int ReadPixel(uint32_t attachmentIndex, int x, int y);

    private:
        void Clear();

    private:
        static inline constexpr uint32_t s_MaxFrameBufferSize = 8192;

    private:
        HandleType                                   m_Handle;
        FrameBufferSpecification                     m_Specification;

        std::vector<FrameBufferTextureSpecification> m_ColorAttachmentSpecification;
        FrameBufferTextureSpecification              m_DepthAttachmentSpecification;

        std::vector<HandleType>                      m_ColorAttachments;
        HandleType                                   m_DepthAttachment;

    }; // class FrameBuffer

} // namespace Ziben