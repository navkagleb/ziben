#pragma once

namespace Ziben {

    class VertexBuffer {
    public:
        VertexBuffer(const void* data, std::size_t size);
        ~VertexBuffer();

        static void Bind(VertexBuffer& vertexBuffer);
        static void Unbind();

    private:
        uint32_t m_Handle;

    }; // class VertexBuffer

} // namespace Ziben