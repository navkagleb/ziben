#include "VertexArray.hpp"

namespace Ziben {

    VertexArray* VertexArray::Create() {
        return new VertexArray;
    }

    void VertexArray::Bind(VertexArray& vertexArray) {
        glBindVertexArray(vertexArray.m_Handle);
    }

    void VertexArray::Unbind() {
        glBindVertexArray(0);
    }

    VertexArray::VertexArray()
        : m_Handle(0)
        , m_IndexBuffer(nullptr) {

        glGenVertexArrays(1, &m_Handle);
        glBindVertexArray(m_Handle);
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_Handle);
    }

    void VertexArray::PushVertexBuffer(VertexBuffer* vertexBuffer) {
        Bind(*this);
        VertexBuffer::Bind(*vertexBuffer);

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer) {
        Bind(*this);
        IndexBuffer::Bind(*indexBuffer);

        m_IndexBuffer = indexBuffer;
    }

} // namespace Ziben