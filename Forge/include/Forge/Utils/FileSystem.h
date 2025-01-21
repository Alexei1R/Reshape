// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <filesystem>
#include <optional>
#include <string>

namespace forge {

class FileSystem {
public:
    static void Init(const std::string& applicationName);

    // Base paths
    static std::filesystem::path GetExecutablePath();
    static std::filesystem::path GetApplicationDataPath();
    static std::filesystem::path GetResourcesPath();

    // Project specific paths
    static std::filesystem::path GetCachePath();
    static std::filesystem::path GetShaderCachePath();
    static std::filesystem::path GetConfigPath();
    static std::filesystem::path GetLogPath();
    static std::filesystem::path GetShadersPath();

    // Utility functions
    static void CreateDirectoryIfNotExists(const std::filesystem::path& path);
    static bool Exists(const std::filesystem::path& path);
    static std::optional<std::string> ReadFile(const std::filesystem::path& path);
    static bool WriteFile(const std::filesystem::path& path, const std::string& content);

private:
    static void Initialize();
    static std::filesystem::path s_ApplicationDataPath;
    static std::filesystem::path s_ExecutablePath;
    static std::string s_ApplicationName;
    static bool s_Initialized;
};

} // namespace forge

#endif
