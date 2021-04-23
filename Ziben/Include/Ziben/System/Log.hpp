#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

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

    }; // class Log

    using Log = Singleton<Log_Impl>;

} // namespace Ziben

// Core Log
#define ZIBEN_CORE_TRACE(...)   ::Ziben::Log::Get().GetCoreLogger()->trace(__VA_ARGS__)
#define ZIBEN_CORE_INFO(...)    ::Ziben::Log::Get().GetCoreLogger()->info(__VA_ARGS__)
#define ZIBEN_CORE_WARN(...)    ::Ziben::Log::Get().GetCoreLogger()->warn(__VA_ARGS__)
#define ZIBEN_CORE_ERROR(...)   ::Ziben::Log::Get().GetCoreLogger()->error(__VA_ARGS__)
#define ZIBEN_CORE_FATAL(...)   ::Ziben::Log::Get().GetCoreLogger()->fatal(__VA_ARGS__)

// Client Log
#define ZIBEN_TRACE(...)        ::Ziben::Log::Get().GetClientLogger()->trace(__VA_ARGS__)
#define ZIBEN_INFO(...)         ::Ziben::Log::Get().GetClientLogger()->info(__VA_ARGS__)
#define ZIBEN_WARN(...)         ::Ziben::Log::Get().GetClientLogger()->warn(__VA_ARGS__)
#define ZIBEN_ERROR(...)        ::Ziben::Log::Get().GetClientLogger()->error(__VA_ARGS__)
#define ZIBEN_FATAL(...)        ::Ziben::Log::Get().GetClientLogger()->fatal(__VA_ARGS__)