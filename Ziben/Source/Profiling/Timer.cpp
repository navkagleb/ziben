#include "Timer.hpp"

#include "ProfileEngine.hpp"

#include <iostream>

namespace Ziben::Profile {

    Timer::Timer(const char* name)
        : m_Name(name) {

        m_StartTimePoint = std::chrono::steady_clock::now();
    }

    Timer::~Timer() {
        namespace chr = std::chrono;

        auto endTimePoint = chr::steady_clock::now();
        auto threadID     = std::hash<std::thread::id>()(std::this_thread::get_id());

        ProfileEngine::GetRef().WriteProfile({
            m_Name,
            FloatingPointMicroseconds(m_StartTimePoint.time_since_epoch()),
            std::chrono::duration_cast<IntegralMicroseconds>(endTimePoint - m_StartTimePoint),
            threadID
        });
    }

} // namespace Ziben::Profile