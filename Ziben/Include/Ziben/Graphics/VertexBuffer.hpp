#pragma once

#include "GraphicsCore.hpp"

namespace Ziben {

    class VertexBuffer {
    public:
        static VertexBuffer* Create(const void* data, std::size_t size, BufferUsage usage = BufferUsage::Static);

        static void Bind(VertexBuffer& vertexBuffer);
        static void Unbind();

    public:
        VertexBuffer(const void* data, std::size_t size, BufferUsage usage);
        ~VertexBuffer();

        [[nodiscard]] inline std::size_t GetSize() const { return m_Size; }
        [[nodiscard]] inline BufferUsage GetUsage() const { return m_Usage; }

    private:
        HandleType  m_Handle;
        std::size_t m_Size;
        BufferUsage m_Usage;

    }; // class VertexBuffer

} // namespace Ziben