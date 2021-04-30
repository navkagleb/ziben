#pragma once

namespace Ziben::Profile {

    class Timer {
    public:
        explicit Timer(const char* name);
        ~Timer();

    private:
        const char*                                                 m_Name;
        std::chrono::time_point<std::chrono::high_resolution_clock> m_StartTimePoint;

    }; // class Timer

} // namespace Ziben::Profile