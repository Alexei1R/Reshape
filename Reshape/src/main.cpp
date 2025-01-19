// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Forge.hpp"

#include "Application.h"
#include "Forge/Utils/FileSystem.h"
#include "Utils/Parsing.h"

int main(int argc, char* argv[]) {
    forge::Log::Init("Reshape");
    forge::FileSystem::Init("Reshape");

    {
        // TODO: Cleanup this code later
        bool apiSpecified;
        forge::Log::Critical("Configuring for Platform: {}",
                             forge::PlatformAPI::GetPlatformName(forge::PlatformAPI::GetCurrentPlatform()));
        forge::GraphicsAPI selectedAPI = reshape::CommandLineParser::ParseCommandLine(argc, argv, apiSpecified);
        if (apiSpecified) {
            if (selectedAPI == forge::GraphicsAPI::None) {
                forge::Log::Error("Invalid Graphics API specified!");
                reshape::CommandLineParser::PrintUsage();
                return 1;
            }
            if (!forge::PlatformAPI::SelectGraphicsAPI(selectedAPI)) {
                forge::Log::Error("Requested Graphics API {} is not supported on this platform!",
                                  forge::PlatformAPI::GetGraphicsAPIName(selectedAPI));
                forge::Log::Error("Falling back to default Graphics API...");
                selectedAPI = forge::PlatformAPI::GetDefaultGraphicsAPI();
            }
        }
        forge::Log::Info("Using Graphics API: {}", forge::PlatformAPI::GetGraphicsAPIName(selectedAPI));
    }

    // NOTE: Initialize and run the application
    reshape::Application application;
    application.Run();

    return 0;
}
