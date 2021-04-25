#include "VertexBuffer.hpp"

namespace Ziben {

    Ref<VertexBuffer> VertexBuffer::Create(const void* data, std::size_t size, BufferUsage usage) {
        return std::make_shared<VertexBuffer>(data, size, usage);
    }

    void VertexBuffer::Bind(const Ref<VertexBuffer>& vertexBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_Handle);
    }

    void VertexBuffer::Unbind() {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VertexBuffer::VertexBuffer(const void* data, std::size_t size, BufferUsage usage)
        : m_Handle(0)
        , m_Size(size)
        , m_Usage(usage) {

        glGenBuffers(1, &m_Handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Size), data, static_cast<GLenum>(m_Usage));
    }

    VertexBuffer::~VertexBuffer() {
        glDeleteBuffers(1, &m_Handle);
    }

    void VertexBuffer::SetLayout(const VertexBufferLayout& layout) {
        m_Layout = layout;
    }

} // namespace Ziben