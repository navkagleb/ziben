#include "VertexArray.hpp"

namespace Ziben {

    Ref<VertexArray> VertexArray::Create() {
        return CreateRef<VertexArray>();
    }

    void VertexArray::Bind(const Ref<VertexArray>& vertexArray) {
        glBindVertexArray(vertexArray->m_Handle);
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

    void VertexArray::PushVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        // Bind Current VertexArray
        glBindVertexArray(m_Handle);

        // Bind VertexBuffer
        VertexBuffer::Bind(vertexBuffer);

        m_VertexBuffers.push_back(vertexBuffer);

        for (uint32_t index = 0; const auto& element : vertexBuffer->GetLayout()) {
            glEnableVertexAttribArray(index);
            glVertexAttribPointer(
                index++,
                ShaderData::GetCount(element.Type),
                ShaderData::ToNativeType(element.Type),
                element.IsNormalized,
                static_cast<GLsizei>(vertexBuffer->GetLayout().GetStride()),
                (const void*)element.Offset
            );
        }
    }

    void VertexArray::SetIndexBuffer(const Ref<IndexBuffer>& indexBuffer) {
        // Bind Current VertexArray
        glBindVertexArray(m_Handle);

        // Bind IndexBuffer
        IndexBuffer::Bind(indexBuffer);

        m_IndexBuffer = indexBuffer;
    }

} // namespace Ziben