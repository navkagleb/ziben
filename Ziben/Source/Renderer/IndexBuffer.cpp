#include "IndexBuffer.hpp"

namespace Ziben {

    IndexBuffer* IndexBuffer::Create(const IndexType* indices, std::size_t count, BufferUsage usage) {
        return new IndexBuffer(indices, count, usage);
    }

    void IndexBuffer::Bind(IndexBuffer& indexBuffer) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer.m_Handle);
    }

    void IndexBuffer::Unbind() {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    IndexBuffer::IndexBuffer(const IndexType* indices, std::size_t count, BufferUsage usage)
        : m_Handle(0)
        , m_Count(count)
        , m_Usage(usage) {

        glGenBuffers(1, &m_Handle);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            static_cast<GLsizeiptr>(count * sizeof(IndexType)),
            indices,
            static_cast<GLenum>(usage)
        );
    }

    IndexBuffer::~IndexBuffer() {
        glDeleteBuffers(1, &m_Handle);
    }

} // namespace Ziben