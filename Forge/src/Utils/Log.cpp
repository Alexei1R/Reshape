// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Utils/Log.h"
#include <spdlog/spdlog-inl.h>

namespace forge {

std::shared_ptr<spdlog::logger> Log::s_Logger = nullptr;

void Log::Init(std::string name) {
    spdlog::set_pattern("%T [%n] %v%$");
    s_Logger = spdlog::stdout_color_mt(name);
    s_Logger->set_level(spdlog::level::trace);
}

void Log::SetLevel(spdlog::level::level_enum level) {
    s_Logger->set_level(level);
    spdlog::set_level(level);
}

} // namespace forge
