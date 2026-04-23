#pragma once

#include <iostream>
#include <string>
#include <chrono>
#include <iomanip>
#include <utility>
#include <ctime>

namespace Forge {

/**
 * @brief Simple logging system with different severity levels.
 */
class Log {
public:
    enum class Level {
        Info,
        Warn,
        Error
    };

    static void Init() {
        // Initialization if needed (e.g. file logging setup)
    }

    template<typename... Args>
    static void Info(Args&&... args) {
        LogMessage(Level::Info, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Warn(Args&&... args) {
        LogMessage(Level::Warn, std::forward<Args>(args)...);
    }

    template<typename... Args>
    static void Error(Args&&... args) {
        LogMessage(Level::Error, std::forward<Args>(args)...);
    }

private:
    static std::string GetTimestamp() {
        auto now = std::chrono::system_clock::now();
        std::time_t time = std::chrono::system_clock::to_time_t(now);
        std::tm tm_info;
#if defined(_WIN32)
        localtime_s(&tm_info, &time);
#else
        localtime_r(&time, &tm_info);
#endif

        char buffer[32];
        std::strftime(buffer, sizeof(buffer), "%H:%M:%S", &tm_info);
        return std::string(buffer);
    }

    template<typename... Args>
    static void LogMessage(Level level, Args&&... args) {
        std::string prefix;
        switch (level) {
            case Level::Info:  prefix = "[INFO] "; break;
            case Level::Warn:  prefix = "[WARN] "; break;
            case Level::Error: prefix = "[ERROR]"; break;
        }

        std::cout << "[" << GetTimestamp() << "] " << prefix << " ";
        (std::cout << ... << std::forward<Args>(args)) << std::endl;
    }
};

} // namespace Forge

// Convenience macros
#define FORGE_LOG_INFO(...)  ::Forge::Log::Info(__VA_ARGS__)
#define FORGE_LOG_WARN(...)  ::Forge::Log::Warn(__VA_ARGS__)
#define FORGE_LOG_ERROR(...) ::Forge::Log::Error(__VA_ARGS__)