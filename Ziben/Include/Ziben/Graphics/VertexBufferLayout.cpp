#include "VertexBufferLayout.hpp"

namespace Ziben {

    VertexBufferLayout::VertexBufferLayout()
        : m_Stride(0) {}

    VertexBufferLayout::VertexBufferLayout(std::initializer_list<Element> elements)
        : m_Elements(elements)
        , m_Stride(0) {

        for (auto& element : m_Elements) {
            element.Offset  = m_Stride;
            m_Stride       += ShaderData::GetSize(element.Type);
        }
    }

} // namespace Ziben