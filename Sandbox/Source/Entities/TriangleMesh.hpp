#pragma once

#include <Ziben/Graphics/VertexArray.hpp>

class TriangleMesh {
public:
    TriangleMesh();
    virtual ~TriangleMesh() = default;

    void OnRender() const;

protected:
    Ziben::VertexArray* m_VertexArray;

}; // TriangleMesh