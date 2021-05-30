#pragma once

namespace Ziben::Profile {

    using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;
    using IntegralMicroseconds	    = std::chrono::duration<intmax_t, std::micro>;


    class Timer {
    public:
        explicit Timer(const char* name);
        ~Timer();

    private:
        const char*                                        m_Name;
        std::chrono::time_point<std::chrono::steady_clock> m_StartTimePoint;

    }; // class Timer

} // namespace Ziben::Profile