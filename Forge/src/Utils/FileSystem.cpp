// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Utils/FileSystem.h"
#include "Forge/Utils/Log.h"
#include <fstream>

#ifdef _WIN32
#    include <windows.h>
#elif defined(__linux__)
#    include <linux/limits.h>
#    include <unistd.h>
#elif defined(__APPLE__)
#    include <mach-o/dyld.h>
#endif

namespace forge {

std::filesystem::path FileSystem::s_ApplicationDataPath;
std::filesystem::path FileSystem::s_ExecutablePath;
std::string FileSystem::s_ApplicationName;
bool FileSystem::s_Initialized = false;

void FileSystem::Init(const std::string& applicationName) {
    if (s_Initialized) {
        Log::Warn("FileSystem already initialized!");
        return;
    }

    s_ApplicationName = applicationName;
    Initialize();
    s_Initialized = true;
}

void FileSystem::Initialize() {
// Get executable path
#ifdef _WIN32
    wchar_t path[MAX_PATH];
    GetModuleFileNameW(nullptr, path, MAX_PATH);
    s_ExecutablePath = std::filesystem::path(path).parent_path();
#elif defined(__linux__)
    char result[PATH_MAX];
    ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
    s_ExecutablePath = std::filesystem::path(std::string(result, (count > 0) ? count : 0)).parent_path();
#elif defined(__APPLE__)
    char path[PATH_MAX];
    uint32_t size = sizeof(path);
    if (_NSGetExecutablePath(path, &size) == 0) {
        s_ExecutablePath = std::filesystem::path(path).parent_path();
    }
#endif

// Set up application data path
#ifdef _WIN32
    s_ApplicationDataPath = std::filesystem::path(getenv("APPDATA")) / s_ApplicationName;
#elif defined(__linux__)
    const char* xdgData = getenv("XDG_DATA_HOME");
    if (xdgData) {
        s_ApplicationDataPath = std::filesystem::path(xdgData) / s_ApplicationName;
    } else {
        s_ApplicationDataPath = std::filesystem::path(getenv("HOME")) / ".local/share" / s_ApplicationName;
    }
#elif defined(__APPLE__)
    s_ApplicationDataPath = std::filesystem::path(getenv("HOME")) / "Library/Application Support" / s_ApplicationName;
#endif

    // Create necessary directories
    CreateDirectoryIfNotExists(GetCachePath());
    CreateDirectoryIfNotExists(GetShaderCachePath());
    CreateDirectoryIfNotExists(GetConfigPath());
    CreateDirectoryIfNotExists(GetLogPath());
}

std::filesystem::path FileSystem::GetExecutablePath() {
    return s_ExecutablePath;
}

std::filesystem::path FileSystem::GetApplicationDataPath() {
    return s_ApplicationDataPath;
}

std::filesystem::path FileSystem::GetResourcesPath() {
    return GetExecutablePath() / "resources";
}

std::filesystem::path FileSystem::GetCachePath() {
    return s_ApplicationDataPath / "cache";
}

std::filesystem::path FileSystem::GetShaderCachePath() {
    return GetCachePath() / "shaders";
}

std::filesystem::path FileSystem::GetConfigPath() {
    return s_ApplicationDataPath / "config";
}

std::filesystem::path FileSystem::GetLogPath() {
    return s_ApplicationDataPath / "logs";
}

std::filesystem::path FileSystem::GetShadersPath() {
    return GetResourcesPath() / "shaders";
}

void FileSystem::CreateDirectoryIfNotExists(const std::filesystem::path& path) {
    if (!std::filesystem::exists(path)) {
        try {
            std::filesystem::create_directories(path);
            Log::Trace("Created directory: {}", path.string());
        } catch (const std::filesystem::filesystem_error& e) {
            Log::Error("Failed to create directory {}: {}", path.string(), e.what());
        }
    }
}

bool FileSystem::Exists(const std::filesystem::path& path) {
    return std::filesystem::exists(path);
}

std::optional<std::string> FileSystem::ReadFile(const std::filesystem::path& path) {
    if (!Exists(path)) {
        Log::Error("File does not exist: {}", path.string());
        return std::nullopt;
    }

    try {
        std::ifstream file(path, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            Log::Error("Failed to open file: {}", path.string());
            return std::nullopt;
        }

        std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return content;
    } catch (const std::exception& e) {
        Log::Error("Failed to read file {}: {}", path.string(), e.what());
        return std::nullopt;
    }
}

bool FileSystem::WriteFile(const std::filesystem::path& path, const std::string& content) {
    try {
        CreateDirectoryIfNotExists(path.parent_path());
        std::ofstream file(path, std::ios::out | std::ios::binary);
        if (!file.is_open()) {
            Log::Error("Failed to open file for writing: {}", path.string());
            return false;
        }

        file.write(content.c_str(), content.size());
        return true;
    } catch (const std::exception& e) {
        Log::Error("Failed to write file {}: {}", path.string(), e.what());
        return false;
    }
}

} // namespace forge
