#pragma once

#include "TriangleMesh.hpp"

class Torus : public TriangleMesh {
public:
    Torus(float outerRadius, float innerRadius, std::size_t sideCount, std::size_t ringCount);

private:
    struct Vertex {
        glm::vec3 Position;
        glm::vec3 Normals;
    };

private:
    Ziben::Ref<Ziben::VertexBuffer> m_VertexBuffer;
    Ziben::Ref<Ziben::IndexBuffer>  m_IndexBuffer;

}; // class Torus