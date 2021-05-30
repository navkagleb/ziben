#include "VertexArray.hpp"

namespace Ziben {

    Ref<VertexArray> VertexArray::Create() {
        return CreateRef<VertexArray>();
    }

    void VertexArray::Bind(const Ref<VertexArray>& vertexArray) {
        ZIBEN_PROFILE_FUNCTION();

        glBindVertexArray(vertexArray->m_Handle);
    }

    void VertexArray::Unbind() {
        ZIBEN_PROFILE_FUNCTION();

        glBindVertexArray(0);
    }

    VertexArray::VertexArray()
        : m_Handle(0)
        , m_VertexBufferIndex(0)
        , m_IndexBuffer(nullptr) {

        ZIBEN_PROFILE_FUNCTION();

        glGenVertexArrays(1, &m_Handle);
    }

    VertexArray::~VertexArray() {
        ZIBEN_PROFILE_FUNCTION();

        glDeleteVertexArrays(1, &m_Handle);
    }

    void VertexArray::PushVertexBuffer(const Ref<VertexBuffer>& vertexBuffer) {
        // Bind Current VertexArray
        glBindVertexArray(m_Handle);

        // Bind VertexBuffer
        VertexBuffer::Bind(vertexBuffer);

        m_VertexBuffers.push_back(vertexBuffer);

        for (const auto& element : vertexBuffer->GetLayout()) {
//            switch
//            glEnableVertexAttribArray(m_VertexBufferIndex);
//            glVertexAttribPointer(
//                m_VertexBufferIndex++,
//                ShaderData::GetCount(element.Type),
//                ShaderData::ToNativeType(element.Type),
//                element.IsNormalized,
//                static_cast<GLsizei>(vertexBuffer->GetLayout().GetStride()),
//                (const void*)element.Offset
//            );

            switch (element.Type) {
                case ShaderData::Type::Float:
                case ShaderData::Type::Float2:
                case ShaderData::Type::Float3:
                case ShaderData::Type::Float4: {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribPointer(
                        m_VertexBufferIndex++,
                        ShaderData::GetCount(element.Type),
                        ShaderData::ToNativeType(element.Type),
                        element.IsNormalized,
                        static_cast<GLsizei>(vertexBuffer->GetLayout().GetStride()),
                        reinterpret_cast<const void *>(element.Offset)
                    );

                    break;
                }

                case ShaderData::Type::Int:
                case ShaderData::Type::Int2:
                case ShaderData::Type::Int3:
                case ShaderData::Type::Int4:
                case ShaderData::Type::Bool: {
                    glEnableVertexAttribArray(m_VertexBufferIndex);
                    glVertexAttribIPointer(
                        m_VertexBufferIndex++,
                        ShaderData::GetCount(element.Type),
                        ShaderData::ToNativeType(element.Type),
                        static_cast<GLsizei>(vertexBuffer->GetLayout().GetStride()),
                        reinterpret_cast<const void *>(element.Offset)
                    );

                    break;
                }

                case ShaderData::Type::Mat3:
                case ShaderData::Type::Mat4: {
                    int count = ShaderData::GetCount(element.Type);

                    for (int i = 0; i < count; ++i) {
                        glVertexAttribPointer(
                            m_VertexBufferIndex++,
                            count,
                            ShaderData::ToNativeType(element.Type),
                            element.IsNormalized,
                            static_cast<GLsizei>(vertexBuffer->GetLayout().GetStride()),
                            reinterpret_cast<const void *>(element.Offset + sizeof(float) * count * i)
                        );

                        glVertexAttribDivisor(m_VertexBufferIndex, 1);
                    }

                    break;
                }

                default: break;
            }
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