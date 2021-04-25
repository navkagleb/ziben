#pragma once

#include "GraphicsCore.hpp"
#include "Ziben/Utility/Reference.hpp"

namespace Ziben {

    class IndexBuffer {
    public:
        static Ref<IndexBuffer> Create(
            const IndexType* indices,
            std::size_t      count,
            BufferUsage      usage = BufferUsage::Static
        );

        static void Bind(const Ref<IndexBuffer>& indexBuffer);
        static void Unbind();

    public:
        IndexBuffer(const IndexType* indices, std::size_t count, BufferUsage usage);
        ~IndexBuffer();

        [[nodiscard]] inline std::size_t GetCount() const { return m_Count; }
        [[nodiscard]] inline BufferUsage GetUsage() const { return m_Usage; }

    private:
        HandleType  m_Handle;
        std::size_t m_Count;
        BufferUsage m_Usage;

    }; // class IndexBuffer

} // namespace Ziben