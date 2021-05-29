#include "Torus.hpp"

#include <glm/gtc/constants.hpp>

#include <Ziben/System/Log.hpp>

Torus::Torus(float outerRadius, float innerRadius, std::size_t sideCount, std::size_t ringCount)
    : m_VertexBuffer(nullptr)
    , m_IndexBuffer(nullptr) {

    uint32_t facesCount  = sideCount * ringCount;
    uint32_t vertexCount = sideCount * (ringCount + 1);

    std::vector<Vertex>           vertices(vertexCount);
    std::vector<Ziben::IndexType> indices(facesCount * 6);

    auto sideFactor = glm::two_pi<float>() / static_cast<float>(sideCount);
    auto ringFactor = glm::two_pi<float>() / static_cast<float>(ringCount);

    int idx  = 0;

    for (uint32_t ring = 0; ring <= ringCount; ++ring) {
        float u  = static_cast<float>(ring) * ringFactor;
        float cu = std::cos(u);
        float su = std::sin(u);

        for (uint32_t side = 0; side < sideCount; ++side) {
            float v  = static_cast<float>(side) * sideFactor;
            float cv = std::cos(v);
            float sv = std::sin(v);
            float r  = (outerRadius + innerRadius * cv);

            vertices[idx].Position.x = r * cu;
            vertices[idx].Position.y = r * su;
            vertices[idx].Position.z = innerRadius * sv;

            vertices[idx].Normals.x = cv * cu * r;
            vertices[idx].Normals.y = cv * su * r;
            vertices[idx].Normals.z = sv * r;

            // Normalize
            auto len = static_cast<float>(std::sqrt(
                std::pow(vertices[idx].Normals.x, 2) +
                std::pow(vertices[idx].Normals.y, 2) +
                std::pow(vertices[idx].Normals.z, 2)
            ));

            vertices[idx].Normals.x /= len;
            vertices[idx].Normals.y /= len;
            vertices[idx].Normals.z /= len;

            ++idx;
        }
    }

    idx = 0;
    for (uint32_t ring = 0; ring < ringCount; ++ring) {
        uint32_t ringStart     = ring * sideCount;
        uint32_t nextRingStart = (ring + 1) * sideCount;

        for (uint32_t side = 0; side < sideCount; ++side) {
            auto nextSide = (side + 1) % sideCount;

            // The quad
            indices[idx    ] = ringStart     + side;
            indices[idx + 1] = nextRingStart + side;
            indices[idx + 2] = nextRingStart + nextSide;
            indices[idx + 3] = ringStart     + side;
            indices[idx + 4] = nextRingStart + nextSide;
            indices[idx + 5] = ringStart     + nextSide;

            idx += 6;
        }
    }

    m_VertexBuffer = Ziben::VertexBuffer::Create(vertices.data(), vertices.size() * sizeof(Vertex));
    m_VertexBuffer->SetLayout({
        { Ziben::ShaderData::Type::Float3, "VertexPosition" },
        { Ziben::ShaderData::Type::Float3, "VertexNormal" }
    });

    m_IndexBuffer = Ziben::IndexBuffer::Create(indices.data(), indices.size());

    m_VertexArray->PushVertexBuffer(m_VertexBuffer);
    m_VertexArray->SetIndexBuffer(m_IndexBuffer);
}