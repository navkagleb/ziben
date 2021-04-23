#pragma once

#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"

namespace Ziben {

    class VertexArray {
    public:
        static VertexArray* Create();

        static void Bind(const VertexArray& vertexArray);
        static void Unbind();

    public:
        VertexArray();
        ~VertexArray();

        [[nodiscard]] const IndexBuffer& GetIndexBuffer() const { return *m_IndexBuffer; }

        void PushVertexBuffer(VertexBuffer* vertexBuffer);
        void SetIndexBuffer(IndexBuffer* indexBuffer);

    private:
        HandleType                 m_Handle;
        std::vector<VertexBuffer*> m_VertexBuffers;
        IndexBuffer*               m_IndexBuffer;

    }; // class VertexArray

} // namespace Ziben