// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)
#include "Forge/Renderer/GraphicsContext.h"
#include "Forge/Utils/Log.h"
#include "Forge/Utils/Platform.h"
#include "OpenGL/OpenGLContext.h"

namespace forge {

Unique<GraphicsContext> GraphicsContext::Create(Shared<Window> window) {
    if (!window) {
        Log::Critical("Null window handle passed to GraphicsContext::Create");
        return nullptr;
    }

    Unique<GraphicsContext> context;

    auto api = PlatformAPI::GetSelectedGraphicsAPI();
    switch (api) {
    case GraphicsAPI::OpenGL:
        context = CreateUnique<OpenGLContext>(window);
        break;

    case GraphicsAPI::Vulkan:
#ifdef RESHAPE_VULKAN_SUPPORT
        // context = CreateUnique<VulkanContext>(window);
        Log::Info("Vulkan support will be added in future updates");
#else
        Log::Critical("Vulkan support not enabled in this build");
#endif
        break;

    case GraphicsAPI::DirectX12:
#ifdef RESHAPE_PLATFORM_WINDOWS
        // context = CreateUnique<DX12Context>(window);
        Log::Info("DirectX12 support will be added in future updates");
#else
        Log::Critical("DirectX12 is only supported on Windows");
#endif
        break;

    case GraphicsAPI::Metal:
#ifdef RESHAPE_PLATFORM_MACOS
        // context = CreateUnique<MetalContext>(window);
        Log::Info("Metal support will be added in future updates");
#else
        Log::Critical("Metal is only supported on macOS");
#endif
        break;

    case GraphicsAPI::WebGL:
#ifdef RESHAPE_PLATFORM_WEB
        // context = CreateUnique<WebGLContext>(window);
        Log::Info("WebGL support will be added in future updates");
#else
        Log::Critical("WebGL is only supported on web platforms");
#endif
        break;

    case GraphicsAPI::None:
    default:
        Log::Critical("Unknown or unsupported graphics API selected");
        return nullptr;
    }

    if (!context) {
        Log::Critical("Failed to create graphics context");
        return nullptr;
    }

    if (!context->Init()) {
        Log::Critical("Failed to initialize graphics context");
        return nullptr;
    }

    return context;
}

} // namespace forge
