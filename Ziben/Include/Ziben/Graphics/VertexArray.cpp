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

        std::cout << "VertexArray begin -- " << std::endl;

        glGenVertexArrays(1, &m_Handle);

        std::cout << "m_Handle = " << m_Handle << std::endl;

        glBindVertexArray(m_Handle);

        std::cout << "VertexArray end -- " << std::endl;
    }

    VertexArray::~VertexArray() {
        glDeleteVertexArrays(1, &m_Handle);
    }

    void VertexArray::PushVertexBuffer(VertexBuffer* vertexBuffer) {
        Bind(*this);
        VertexBuffer::Bind(*vertexBuffer);

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

    void VertexArray::SetIndexBuffer(IndexBuffer* indexBuffer) {
        Bind(*this);
        IndexBuffer::Bind(*indexBuffer);

        m_IndexBuffer = indexBuffer;
    }

} // namespace Ziben