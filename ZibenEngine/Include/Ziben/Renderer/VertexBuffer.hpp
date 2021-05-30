#pragma once

#include "GraphicsCore.hpp"
#include "VertexBufferLayout.hpp"

namespace Ziben {

    class VertexBuffer {
    public:
        static Ref<VertexBuffer> Create(std::size_t size);
        static Ref<VertexBuffer> Create(const void* data, std::size_t size, BufferUsage usage = BufferUsage::Static);

        static void Bind(const Ref<VertexBuffer>& vertexBuffer);
        static void Unbind();

    public:
        explicit VertexBuffer(std::size_t size);
        VertexBuffer(const void* data, std::size_t size, BufferUsage usage);
        ~VertexBuffer();

        [[nodiscard]] inline std::size_t GetSize() const { return m_Size; }
        [[nodiscard]] inline BufferUsage GetUsage() const { return m_Usage; }
        [[nodiscard]] inline const VertexBufferLayout& GetLayout() const { return m_Layout; }

        void SetLayout(const VertexBufferLayout& layout);
        void SetData(const void* data, std::size_t size) const;

    private:
        HandleType         m_Handle;
        std::size_t        m_Size;
        BufferUsage        m_Usage;
        VertexBufferLayout m_Layout;

    }; // class VertexBuffer

} // namespace Ziben