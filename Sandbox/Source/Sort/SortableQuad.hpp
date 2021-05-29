#pragma once

namespace Sandbox {

    struct Quad {
        std::size_t Index    = 0;
        glm::vec2   Position = glm::vec2(0.0f);
        glm::vec2   Size     = glm::vec2(1.0f);
        glm::vec4   Color    = glm::vec4(1.0f);

        inline bool operator <(const Quad& other) const { return Index < other.Index; }
    };

    using QuadContainer = std::vector<Quad>;
    using QuadIterator  = QuadContainer::iterator;

} // namespace Sandbox