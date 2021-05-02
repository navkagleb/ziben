#pragma once

#include "Timer.hpp"
#include "Ziben/Utility/Singleton.hpp"

#include <mutex>
#include <iostream>

namespace Ziben::Profile {

    struct ProfileResult {
        std::string               Name;
        FloatingPointMicroseconds Start;
        IntegralMicroseconds      ElapsedTime;
        uintmax_t                 ThreadID = 0;
    };

    class ProfileEngine_Impl {
    public:
        friend class Ziben::Singleton<ProfileEngine_Impl>;

    public:
        ProfileEngine_Impl() = default;
        ~ProfileEngine_Impl() = default;

        void BeginSession(const std::string& name, const std::string& filename = "result.json");
        void EndSession();

        void WriteProfile(const ProfileResult& result);

    private:
        void WriteHeader();
        void WriteFooter();

    private:
        std::string   m_CurrentSession;
        std::ofstream m_OutputStream;
        std::mutex    m_Mutex;

    }; // ProfilingEngine_Impl

    using ProfileEngine = Ziben::Singleton<ProfileEngine_Impl>;

} // namespace Ziben::Profiling

#define ZIBEN_PROFILE 1

#if ZIBEN_PROFILE
    #define ZIBEN_PROFILE_BEGIN_SESSION(name, filename) ::Ziben::Profile::ProfileEngine::GetRef().BeginSession(name, filename)
    #define ZIBEN_PROFILE_END_SESSION()                 ::Ziben::Profile::ProfileEngine::GetRef().EndSession()
    #define ZIBEN_PROFILE_SCOPE(name)                   ::Ziben::Profile::Timer Timer##__FILE__##__LINE__(name)
    #define ZIBEN_PROFILE_FUNCTION()                    ZIBEN_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
    #define ZIBEN_PROFILE_BEGIN_SESSION(name, filename)
    #define ZIBEN_PROFILE_END_SESSION()
    #define ZIBEN_PROFILE_SCOPE(name)
    #define ZIBEN_PROFILE_FUNCTION()
#endif