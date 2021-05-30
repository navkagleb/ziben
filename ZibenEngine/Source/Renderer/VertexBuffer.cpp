#include "VertexBuffer.hpp"

namespace Ziben {

    Ref<VertexBuffer> VertexBuffer::Create(std::size_t size) {
        return CreateRef<VertexBuffer>(size);
    }

    Ref<VertexBuffer> VertexBuffer::Create(const void* data, std::size_t size, BufferUsage usage) {
        return CreateRef<VertexBuffer>(data, size, usage);
    }

    void VertexBuffer::Bind(const Ref<VertexBuffer>& vertexBuffer) {
        ZIBEN_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer->m_Handle);
    }

    void VertexBuffer::Unbind() {
        ZIBEN_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    VertexBuffer::VertexBuffer(std::size_t size)
        : m_Handle(0)
        , m_Size(size)
        , m_Usage(BufferUsage::Dynamic) {

        ZIBEN_PROFILE_FUNCTION();

        glGenBuffers(1, &m_Handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Size), nullptr, static_cast<GLenum>(m_Usage));
    }

    VertexBuffer::VertexBuffer(const void* data, std::size_t size, BufferUsage usage)
        : m_Handle(0)
        , m_Size(size)
        , m_Usage(usage) {

        ZIBEN_PROFILE_FUNCTION();

        glGenBuffers(1, &m_Handle);
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(m_Size), data, static_cast<GLenum>(m_Usage));
    }

    VertexBuffer::~VertexBuffer() {
        ZIBEN_PROFILE_FUNCTION();

        glDeleteBuffers(1, &m_Handle);
    }

    void VertexBuffer::SetLayout(const VertexBufferLayout& layout) {
        m_Layout = layout;
    }

    void VertexBuffer::SetData(const void* data, std::size_t size) const {
        glBindBuffer(GL_ARRAY_BUFFER, m_Handle);
        glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<GLsizeiptr>(size), data);
    }

} // namespace Ziben