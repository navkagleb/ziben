#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Ziben {

    class VertexArray {
    public:
        static Ref<VertexArray> Create();

        static void Bind(const Ref<VertexArray>& vertexArray);
        static void Unbind();

    public:
        VertexArray();
        ~VertexArray();

        [[nodiscard]] const Ref<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }

        void PushVertexBuffer(const Ref<VertexBuffer>& vertexBuffer);
        void SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer);

    private:
        HandleType                     m_Handle;
        std::size_t                    m_VertexBufferIndex;
        std::vector<Ref<VertexBuffer>> m_VertexBuffers;
        Ref<IndexBuffer>               m_IndexBuffer;

    }; // class VertexArray

} // namespace Ziben