#pragma once

#include <spdlog/spdlog.h>

#include "Ziben/Utility/Singleton.hpp"

namespace Ziben {

    class Log_Impl {
    public:
        friend class Singleton<Log_Impl>;

    public:
        [[nodiscard]] inline std::shared_ptr<spdlog::logger>& GetCoreLogger() { return m_CoreLogger; }
        [[nodiscard]] inline std::shared_ptr<spdlog::logger>& GetClientLogger() { return m_ClientLogger; }

    public:
        Log_Impl();
        ~Log_Impl() = default;

    private:
        std::shared_ptr<spdlog::logger> m_CoreLogger;
        std::shared_ptr<spdlog::logger> m_ClientLogger;

    }; // class Log_Impl

    using Log = Singleton<Log_Impl>;

} // namespace Ziben

// Core Log
#define ZIBEN_CORE_TRACE(...)    ::Ziben::Log::GetRef().GetCoreLogger()->trace(__VA_ARGS__)
#define ZIBEN_CORE_INFO(...)     ::Ziben::Log::GetRef().GetCoreLogger()->info(__VA_ARGS__)
#define ZIBEN_CORE_WARN(...)     ::Ziben::Log::GetRef().GetCoreLogger()->warn(__VA_ARGS__)
#define ZIBEN_CORE_ERROR(...)    ::Ziben::Log::GetRef().GetCoreLogger()->error(__VA_ARGS__)
#define ZIBEN_CORE_CRITICAL(...) ::Ziben::Log::GetRef().GetCoreLogger()->critical(__VA_ARGS__)

// Client Log
#define ZIBEN_TRACE(...)         ::Ziben::Log::GetRef().GetClientLogger()->trace(__VA_ARGS__)
#define ZIBEN_INFO(...)          ::Ziben::Log::GetRef().GetClientLogger()->info(__VA_ARGS__)
#define ZIBEN_WARN(...)          ::Ziben::Log::GetRef().GetClientLogger()->warn(__VA_ARGS__)
#define ZIBEN_ERROR(...)         ::Ziben::Log::GetRef().GetClientLogger()->error(__VA_ARGS__)
#define ZIBEN_CRITICAL(...)      ::Ziben::Log::GetRef().GetClientLogger()->critical(__VA_ARGS__)