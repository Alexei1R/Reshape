// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef LOG_H
#define LOG_H

#include <memory>
#include <string>

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

namespace forge {

class Log {
public:
    static void Init(std::string name);
    static void SetLevel(spdlog::level::level_enum level);

    inline static std::shared_ptr<spdlog::logger>& GetLogger() {
        return s_Logger;
    }

    template <typename... Args>
    static void Info(fmt::format_string<Args...> fmt, Args&&... args) {
        s_Logger->info(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Warn(fmt::format_string<Args...> fmt, Args&&... args) {
        s_Logger->warn(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Error(fmt::format_string<Args...> fmt, Args&&... args) {
        s_Logger->error(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Debug(fmt::format_string<Args...> fmt, Args&&... args) {
        s_Logger->debug(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Critical(fmt::format_string<Args...> fmt, Args&&... args) {
        s_Logger->critical(fmt, std::forward<Args>(args)...);
    }

    template <typename... Args>
    static void Trace(fmt::format_string<Args...> fmt, Args&&... args) {
        s_Logger->trace(fmt, std::forward<Args>(args)...);
    }

private:
    static std::shared_ptr<spdlog::logger> s_Logger;
};

} // namespace forge
#endif
