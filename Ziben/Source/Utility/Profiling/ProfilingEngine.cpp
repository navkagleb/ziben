#include "ProfilingEngine.hpp"


namespace Ziben::Profiling {

    ProfilingEngine_Impl::ProfilingEngine_Impl()
        : m_ProfileCount(0) {}

    void ProfilingEngine_Impl::BeginSession(const std::string& name, const std::string& filename) {
        m_CurrentSession = name;
        m_OutputStream.open(filename);

        WriteHeader();
    }

    void ProfilingEngine_Impl::EndSession() {
        WriteFooter();

        m_OutputStream.close();
        m_CurrentSession.clear();
        m_ProfileCount = 0;
    }

    void ProfilingEngine_Impl::WriteProfile(const ProfileResult& result) {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << R"("cat":"function",)";
        m_OutputStream << "\"dur\":" << (result.End - result.Start) << ',';
        m_OutputStream << R"("name":")" << name << "\",";
        m_OutputStream << R"("ph":"X",)";
        m_OutputStream << "\"pid\":0,";
        m_OutputStream << "\"tid\":" << result.ThreadID << ",";
        m_OutputStream << "\"ts\":" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void ProfilingEngine_Impl::WriteHeader() {
        m_OutputStream << R"({"otherData": {}, "traceEvents":[)";
        m_OutputStream.flush();
    }

    void ProfilingEngine_Impl::WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

} // namespace Ziben::Profiling