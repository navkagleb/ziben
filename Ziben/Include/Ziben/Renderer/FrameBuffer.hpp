#pragma once

#include "GraphicsCore.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace Ziben {

    struct FrameBufferSpecification {
        uint32_t Width  = 0;
        uint32_t Height = 0;

        bool SwapChainTarget = false;
    };

    class FrameBuffer {
    public:
        static Ref<FrameBuffer> Create(const FrameBufferSpecification& specification);

        static void Bind(const Ref<FrameBuffer>& frameBuffer);
        static void Unbind();

    public:
        explicit FrameBuffer(const FrameBufferSpecification& specification);
        ~FrameBuffer();

    public:
        [[nodiscard]] inline HandleType GetColorAttachmentHandle() const { return m_ColorAttachment; }
        [[nodiscard]] inline const FrameBufferSpecification& GetSpecification() const { return m_Specification; }

        void Invalidate();
        void Resize(uint32_t width, uint32_t height);

    private:
        static constexpr uint32_t s_MaxFrameBufferSize = 8192;

    private:
        HandleType               m_Handle;
        HandleType               m_ColorAttachment;
        HandleType               m_DepthAttachment;
        FrameBufferSpecification m_Specification;

    }; // class FrameBuffer

} // namespace Ziben