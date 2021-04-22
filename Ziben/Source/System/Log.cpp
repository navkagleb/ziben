#include "Log.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Ziben {

    Log_Impl::Log_Impl() {
        /*
         * T - time stamp
         * n - log name (Core / Client)
         * v - message
         * */
        spdlog::set_pattern("%^[%T] %n: %v%$");

        m_CoreLogger = spdlog::stdout_color_mt("Core");
        m_CoreLogger->set_level(spdlog::level::trace);

        m_ClientLogger = spdlog::stdout_color_mt("Client");
        m_ClientLogger->set_level(spdlog::level::trace);
    }

} // namespace Ziben