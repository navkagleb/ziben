#include "Timer.hpp"

#include "ProfileEngine.hpp"

#include <iostream>

namespace Ziben::Profile {

    Timer::Timer(const char* name)
        : m_Name(name) {

        m_StartTimePoint = std::chrono::high_resolution_clock::now();
    }

    Timer::~Timer() {
        namespace chr = std::chrono;

        auto endTimePoint = chr::high_resolution_clock::now();
        auto threadID     = std::hash<std::thread::id>()(std::this_thread::get_id());

        intmax_t start    = chr::time_point_cast<chr::microseconds>(m_StartTimePoint).time_since_epoch().count();
        intmax_t end      = chr::time_point_cast<chr::microseconds>(endTimePoint).time_since_epoch().count();

        ProfileEngine::Get().WriteProfile({ m_Name, start, end, threadID });
    }

} // namespace Ziben::Profile