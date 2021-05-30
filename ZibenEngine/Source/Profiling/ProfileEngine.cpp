#include "ProfileEngine.hpp"

namespace Ziben::Profile {

    void ProfileEngine_Impl::BeginSession(const std::string& name, const std::string& filename) {
        m_CurrentSession = name;
        m_OutputStream.open(filename);

        WriteHeader();
    }

    void ProfileEngine_Impl::EndSession() {
        std::lock_guard lock(m_Mutex);

        if (!m_CurrentSession.empty()) {
            WriteFooter();

            m_OutputStream.close();
            m_CurrentSession.clear();
        }
    }

    void ProfileEngine_Impl::WriteProfile(const ProfileResult& result) {
        std::stringstream json;

        json << std::setprecision(3) << std::fixed;
        json << ",{";
        json << R"("cat":"function",)";
        json << R"("dur":)" << result.ElapsedTime.count() << ',';
        json << R"("name":")" << result.Name << "\",";
        json << R"("ph":"X",)";
        json << R"("pid":0,)";
        json << R"("tid":)" << result.ThreadID << ",";
        json << R"("ts":)" << result.Start.count();
        json << "}";

        std::lock_guard lock(m_Mutex);

        if (!m_CurrentSession.empty()) {
            m_OutputStream << json.str();
            m_OutputStream.flush();
        }
    }

    void ProfileEngine_Impl::WriteHeader() {
        m_OutputStream << R"({"otherData":{},"traceEvents":[{})";
        m_OutputStream.flush();
    }

    void ProfileEngine_Impl::WriteFooter() {
        m_OutputStream << "]}";
        m_OutputStream.flush();
    }

} // namespace Ziben::Profiling