#pragma once

#include <Ziben/Renderer/VertexArray.hpp>

namespace Diffuse {

    class TriangleMesh {
    public:
        TriangleMesh();
        virtual ~TriangleMesh() = default;

        void OnRender() const;

    protected:
        Ziben::Ref<Ziben::VertexArray> m_VertexArray;

    }; // TriangleMesh

}