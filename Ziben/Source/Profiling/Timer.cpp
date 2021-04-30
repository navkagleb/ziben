#include "Timer.hpp"

#include "ProfileEngine.hpp"

namespace Ziben::Profile {

    Timer::Timer(const char* name)
        : m_Name(name) {

        m_StartTimePoint = std::chrono::steady_clock::now();
    }

    Timer::~Timer() {
        namespace chr = std::chrono;

        auto endTimePoint = chr::steady_clock::now();
        auto threadID     = std::hash<std::thread::id>()(std::this_thread::get_id());

//        intmax_t start    = chr::time_point_cast<chr::microseconds>(m_StartTimePoint).time_since_epoch().count();
//        intmax_t end      = chr::time_point_cast<chr::microseconds>(endTimePoint).time_since_epoch().count();

        ProfileEngine::Get().WriteProfile({
            m_Name,
            FloatingPointMicroseconds(m_StartTimePoint.time_since_epoch()),
            std::chrono::duration_cast<IntegralMicroseconds>(endTimePoint - m_StartTimePoint),
            threadID
        });
    }

} // namespace Ziben::Profile