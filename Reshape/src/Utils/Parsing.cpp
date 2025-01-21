// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Parsing.h"
#include <algorithm>
#include <cctype>

namespace reshape {

std::string CommandLineParser::ToLower(std::string str) {
    std::transform(str.begin(), str.end(), str.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return str;
}

forge::GraphicsAPI CommandLineParser::ParseGraphicsAPI(const std::string& apiStr) {
    std::string api = ToLower(apiStr);

    if (api == "opengl")
        return forge::GraphicsAPI::OpenGL;
    if (api == "vulkan")
        return forge::GraphicsAPI::Vulkan;
    if (api == "dx12" || api == "directx12")
        return forge::GraphicsAPI::DirectX12;
    if (api == "metal")
        return forge::GraphicsAPI::Metal;
    if (api == "webgl")
        return forge::GraphicsAPI::WebGL;

    return forge::GraphicsAPI::None;
}

void CommandLineParser::PrintUsage() {
    forge::Log::Info("Usage: Reshape [--api <graphics_api>]");
    forge::Log::Info("Available Graphics APIs:");

    auto availableAPIs = forge::PlatformAPI::GetAvailableGraphicsAPIs();
    for (const auto& api : availableAPIs) {
        forge::Log::Info("  - {}", forge::PlatformAPI::GetGraphicsAPIName(api));
    }
}

forge::GraphicsAPI CommandLineParser::ParseCommandLine(int argc, char* argv[], bool& apiSpecified) {
    forge::GraphicsAPI selectedAPI = forge::PlatformAPI::GetDefaultGraphicsAPI();
    apiSpecified = false;

    for (int i = 1; i < argc; i++) {
        std::string arg = argv[i];
        if (arg == "--api" && i + 1 < argc) {
            apiSpecified = true;
            selectedAPI = ParseGraphicsAPI(argv[++i]);
        } else if (arg == "--help" || arg == "-h") {
            PrintUsage();
            exit(0);
        }
    }

    return selectedAPI;
}

} // namespace reshape
