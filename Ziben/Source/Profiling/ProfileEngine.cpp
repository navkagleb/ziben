#include "ProfileEngine.hpp"

namespace Ziben::Profile {

    ProfileEngine_Impl::ProfileEngine_Impl()
        : m_ProfileCount(0) {}

    void ProfileEngine_Impl::BeginSession(const std::string& name, const std::string& filename) {
        m_CurrentSession = name;
        m_OutputStream.open(filename);

        WriteHeader();
    }

    void ProfileEngine_Impl::EndSession() {
        WriteFooter();

        m_OutputStream.close();
        m_CurrentSession.clear();
        m_ProfileCount = 0;
    }

    void ProfileEngine_Impl::WriteProfile(const ProfileResult& result) {
        if (m_ProfileCount++ > 0)
            m_OutputStream << ",";

        std::string name = result.Name;
        std::replace(name.begin(), name.end(), '"', '\'');

        m_OutputStream << "{";
        m_OutputStream << R"("cat":"function",)";
        m_OutputStream << R"("dur":)" << (result.End - result.Start) << ',';
        m_OutputStream << R"("name":")" << name << "\",";
        m_OutputStream << R"("ph":"X",)";
        m_OutputStream << R"("pid":0,)";
        m_OutputStream << R"("tid":)" << result.ThreadID << ",";
        m_OutputStream << R"("ts":)" << result.Start;
        m_OutputStream << "}";

        m_OutputStream.flush();
    }

    void ProfileEngine_Impl::WriteHeader() {
        m_OutputStream << R"({"otherData": {}, "traceEvents":[)";
        m_OutputStream.flush();
    }

    void ProfileEngine_Impl::WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

} // namespace Ziben::Profiling