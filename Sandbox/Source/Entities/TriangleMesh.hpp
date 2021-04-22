#pragma once

#include <Ziben/Renderer/VertexArray.hpp>

class TriangleMesh {
public:
    TriangleMesh();
    virtual ~TriangleMesh() = default;

    void OnRender() const;

protected:
    Ziben::VertexArray* m_VertexArray;

}; // TriangleMesh