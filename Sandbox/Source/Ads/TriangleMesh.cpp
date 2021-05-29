#include "TriangleMesh.hpp"

TriangleMesh::TriangleMesh()
    : m_VertexArray(Ziben::VertexArray::Create()) {}

void TriangleMesh::OnRender() const {
    Ziben::VertexArray::Bind(m_VertexArray);

    glDrawElements(
        GL_TRIANGLES,
        static_cast<GLsizei>(m_VertexArray->GetIndexBuffer()->GetCount()),
        GL_UNSIGNED_INT,
        nullptr
    );
}