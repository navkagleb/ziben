#include "VertexBuffer.hpp"

#include <GL/glew.h>

namespace Ziben {

    VertexBuffer::VertexBuffer(const void* data, std::size_t size)
        : m_Handle(0) {

        glGenBuffers(1, &m_Handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(size), data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_Handle);
    }

    void VertexBuffer::Bind(VertexBuffer& vertexBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer.m_Handle);
    }

    void VertexBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

} // namespace Ziben