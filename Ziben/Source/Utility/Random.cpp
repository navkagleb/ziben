#include "Random.hpp"

namespace Ziben {

    Random_Impl::Random_Impl()
        : m_SeedSequence({
            m_RandomDevice(),
            static_cast<uint32_t>(std::chrono::steady_clock::now().time_since_epoch().count())
        })
        , m_MersenneTwister(m_SeedSequence) {}

} // namespace Ziben