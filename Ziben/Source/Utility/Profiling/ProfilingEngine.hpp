#pragma once

//#include "Singleton.hpp"

namespace Ziben {

    template <typename T>
    class Singleton;

}

namespace Ziben::Profiling {

    struct ProfileResult {
        std::string Name;
        intmax_t    Start    = 0;
        intmax_t    End      = 0;
        uintmax_t   ThreadID = 0;
    };

    class ProfilingEngine_Impl {
    public:
        friend class Ziben::Singleton<ProfilingEngine_Impl>;

    public:
        ProfilingEngine_Impl();
        ~ProfilingEngine_Impl() = default;

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

    using ProfilingEngine = Ziben::Singleton<ProfilingEngine_Impl>;

} // namespace Ziben::Profiling