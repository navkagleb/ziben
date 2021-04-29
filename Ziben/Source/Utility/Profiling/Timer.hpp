#pragma once

namespace Ziben::Profiling {

    class Timer {
    public:
        explicit Timer(const char* name);
        ~Timer();

    private:
        const char*                                        m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;

    }; // class Timer

} // namespace Ziben::Profiling