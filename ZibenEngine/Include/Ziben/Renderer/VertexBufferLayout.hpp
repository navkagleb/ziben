#pragma once

#include "Shader.hpp"

namespace Ziben {

    class VertexBufferLayout {
    public:
        struct Element {
            std::string      Name;
            ShaderData::Type Type         = ShaderData::Type::None;
            std::size_t      Offset       = 0;
            bool             IsNormalized = false;

            Element(ShaderData::Type type, std::string name)
                : Name(std::move(name))
                , Type(type) {}
        };

    public:
        using ElementContainer     = std::vector<Element>;
        using ElementIterator      = ElementContainer::iterator;
        using ElementConstIterator = ElementContainer::const_iterator;

    public:
        VertexBufferLayout();
        VertexBufferLayout(std::initializer_list<Element> elements);
        ~VertexBufferLayout() = default;

        [[nodiscard]] inline std::size_t GetStride() const { return m_Stride; }

        [[nodiscard]] ElementIterator Begin() { return m_Elements.begin(); }
        [[nodiscard]] ElementIterator End() { return m_Elements.end(); }

        [[nodiscard]] ElementConstIterator Begin() const { return m_Elements.begin(); }
        [[nodiscard]] ElementConstIterator End() const { return m_Elements.end(); }

    private:
        ElementContainer m_Elements;
        std::size_t      m_Stride;

    }; // class VertexBufferLayout

    // For range based for!
    inline auto begin(VertexBufferLayout& layout) { return layout.Begin(); }
    inline auto end(VertexBufferLayout& layout) { return layout.End(); }

    inline auto begin(const VertexBufferLayout& layout) { return layout.Begin(); }
    inline auto end(const VertexBufferLayout& layout) { return layout.End(); }

} // namespace Ziben