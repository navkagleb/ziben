#include "Timer.hpp"

#include "Singleton.hpp"
#include "ProfilingEngine.hpp"

namespace Ziben::Profiling {

    Timer::Timer(const char* name)
        : m_Name(name) {

        m_StartTimePoint = std::chrono::steady_clock::now();
    }

    Timer::~Timer() {
        namespace chr = std::chrono;

        auto endTimePoint = chr::steady_clock::now();
        auto threadID     = std::hash<std::thread::id>()(std::this_thread::get_id());

        intmax_t start    = chr::time_point_cast<chr::microseconds>(m_StartTimePoint).time_since_epoch().count();
        intmax_t end      = chr::time_point_cast<chr::microseconds>(m_StartTimePoint).time_since_epoch().count();

        ProfilingEngine::Get().WriteProfile({ m_Name, start, end, threadID });
    }

} // namespace Ziben::Profiling