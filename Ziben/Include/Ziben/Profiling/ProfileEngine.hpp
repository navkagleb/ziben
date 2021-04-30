#pragma once

#include "Timer.hpp"
#include "Ziben/Utility/Singleton.hpp"

namespace Ziben::Profile {

    struct ProfileResult {
        std::string Name;
        intmax_t    Start    = 0;
        intmax_t    End      = 0;
        uintmax_t   ThreadID = 0;
    };

    class ProfileEngine_Impl {
    public:
        friend class Ziben::Singleton<ProfileEngine_Impl>;

    public:
        ProfileEngine_Impl();
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
        int           m_ProfileCount;

    }; // ProfilingEngine_Impl

    using ProfileEngine = Ziben::Singleton<ProfileEngine_Impl>;

} // namespace Ziben::Profiling

#if ZIBEN_PROFILE
    #define ZIBEN_PROFILE_BEGIN_SESSION(name, filename) ::Ziben::Profile::ProfileEngine::Get().BeginSession(name, filename)
    #define ZIBEN_PROFILE_END_SESSION()                 ::Ziben::Profile::ProfileEngine::Get().EndSession()
    #define ZIBEN_PROFILE_SCOPE(name)                   ::Ziben::Profile::Timer __FILE__##__LINE__##_Timer(name)
    #define ZIBEN_PROFILE_FUNCTION()                    ZIBEN_PROFILE_SCOPE(__PRETTY_FUNCTION__)
#else
    #define ZIBEN_PROFILE_BEGIN_SESSION(name, filename)
    #define ZIBEN_PROFILE_END_SESSION()
    #define ZIBEN_PROFILE_SCOPE(name)
    #define ZIBEN_PROFILE_FUNCTION()
#endif