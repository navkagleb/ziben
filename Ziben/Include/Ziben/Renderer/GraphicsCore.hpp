#pragma once

#include <GL/glew.h>

namespace Ziben {

    using HandleType = std::uint32_t;
    using IndexType  = std::uint32_t;

    enum class BufferUsage : uint16_t {
        Static  = GL_STATIC_DRAW,
        Dynamic = GL_DYNAMIC_DRAW,
        Stream  = GL_STREAM_DRAW
    };

} // namespace Ziben