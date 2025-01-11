// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef PLATFORM_H
#define PLATFORM_H

#include <string>
#include <vector>

namespace forge {

// Forward declarations of enum classes
enum class Platform { Windows, Linux, MacOS, iOS, Android, WebGL, Unknown };

enum class GraphicsAPI { OpenGL, Vulkan, DirectX12, Metal, WebGL, None };

class PlatformAPI {
public:
    // Platform detection methods
    static Platform GetCurrentPlatform() {
#if defined(RESHAPE_PLATFORM_WINDOWS)
        return Platform::Windows;
#elif defined(RESHAPE_PLATFORM_LINUX)
        return Platform::Linux;
#elif defined(RESHAPE_PLATFORM_IOS)
        return Platform::iOS;
#elif defined(RESHAPE_PLATFORM_ANDROID)
        return Platform::Android;
#elif defined(RESHAPE_PLATFORM_WEBGL)
        return Platform::WebGL;
#elif defined(__APPLE__)
        return Platform::MacOS;
#else
        return Platform::Unknown;
#endif
    }

    // Returns all available graphics APIs for the current platform
    static std::vector<GraphicsAPI> GetAvailableGraphicsAPIs() {
        std::vector<GraphicsAPI> availableAPIs;

        // Check each API for support
        if (IsGraphicsAPISupported(GraphicsAPI::OpenGL))
            availableAPIs.push_back(GraphicsAPI::OpenGL);

        if (IsGraphicsAPISupported(GraphicsAPI::Vulkan))
            availableAPIs.push_back(GraphicsAPI::Vulkan);

        if (IsGraphicsAPISupported(GraphicsAPI::DirectX12))
            availableAPIs.push_back(GraphicsAPI::DirectX12);

        if (IsGraphicsAPISupported(GraphicsAPI::Metal))
            availableAPIs.push_back(GraphicsAPI::Metal);

        if (IsGraphicsAPISupported(GraphicsAPI::WebGL))
            availableAPIs.push_back(GraphicsAPI::WebGL);

        return availableAPIs;
    }

    // Select and set a graphics API from the available ones
    static bool SelectGraphicsAPI(GraphicsAPI api) {
        if (IsGraphicsAPISupported(api)) {
            s_SelectedApi = api;
            return true;
        }
        return false;
    }

    // Get the currently selected Graphics API
    static GraphicsAPI GetSelectedGraphicsAPI() {
        return s_SelectedApi;
    }

    // Get the default Graphics API for the current platform
    static GraphicsAPI GetDefaultGraphicsAPI() {
        return GetPlatformDefaultAPI();
    }

    static bool IsGraphicsAPISupported(GraphicsAPI api) {
        const Platform currentPlatform = GetCurrentPlatform();

        switch (api) {
        case GraphicsAPI::OpenGL:
#ifdef RESHAPE_SUPPORT_OPENGL
            return true;
#else
            return false;
#endif

        case GraphicsAPI::Vulkan:
#ifdef RESHAPE_SUPPORT_VULKAN
            return currentPlatform != Platform::WebGL && currentPlatform != Platform::iOS;
#else
            return false;
#endif

        case GraphicsAPI::DirectX12:
#ifdef RESHAPE_SUPPORT_DIRECTX
            return currentPlatform == Platform::Windows;
#else
            return false;
#endif

        case GraphicsAPI::Metal:
#ifdef RESHAPE_SUPPORT_METAL
            return currentPlatform == Platform::MacOS || currentPlatform == Platform::iOS;
#else
            return false;
#endif

        case GraphicsAPI::WebGL:
            return currentPlatform == Platform::WebGL;

        default:
            return false;
        }
    }

    // Utility methods for string conversion
    static std::string GetPlatformName(Platform platform) {
        switch (platform) {
        case Platform::Windows:
            return "Windows";
        case Platform::Linux:
            return "Linux";
        case Platform::MacOS:
            return "macOS";
        case Platform::iOS:
            return "iOS";
        case Platform::Android:
            return "Android";
        case Platform::WebGL:
            return "WebGL";
        case Platform::Unknown:
            return "Unknown";
        default:
            return "Invalid Platform";
        }
    }

    static std::string GetGraphicsAPIName(GraphicsAPI api) {
        switch (api) {
        case GraphicsAPI::OpenGL:
            return "OpenGL";
        case GraphicsAPI::Vulkan:
            return "Vulkan";
        case GraphicsAPI::DirectX12:
            return "DirectX 12";
        case GraphicsAPI::Metal:
            return "Metal";
        case GraphicsAPI::WebGL:
            return "WebGL";
        case GraphicsAPI::None:
            return "None";
        default:
            return "Invalid Graphics API";
        }
    }

private:
    static constexpr GraphicsAPI GetPlatformDefaultAPI() {
#if defined(RESHAPE_PLATFORM_WINDOWS) && defined(RESHAPE_SUPPORT_DIRECTX)
        return GraphicsAPI::DirectX12;
#elif (defined(__APPLE__) || defined(RESHAPE_PLATFORM_IOS)) && defined(RESHAPE_SUPPORT_METAL)
        return GraphicsAPI::Metal;
#elif defined(RESHAPE_PLATFORM_WEBGL)
        return GraphicsAPI::WebGL;
#elif defined(RESHAPE_SUPPORT_VULKAN)
        return GraphicsAPI::Vulkan;
#elif defined(RESHAPE_SUPPORT_OPENGL)
        return GraphicsAPI::OpenGL;
#else
        return GraphicsAPI::None;
#endif
    }

    static inline GraphicsAPI s_SelectedApi = GraphicsAPI::OpenGL;
};

} // namespace forge
#endif // PLATFORM_H
