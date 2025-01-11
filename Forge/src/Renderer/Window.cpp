// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/Window.h"
#include "Forge/Utils/Common.h"
#include "Forge/Utils/Log.h"

#include "Platform/Window/DefaultWindow.h"

namespace forge {

Shared<Window> Window::Create(const WindowDescriptor& descriptor) {

    FORGE_ASSERT(descriptor.width > 0, "Window width must be greater than 0");
    FORGE_ASSERT(descriptor.height > 0, "Window height must be greater than 0");
    FORGE_ASSERT(!descriptor.name.empty(), "Window name cannot be empty");

    Shared<Window> window = nullptr;

    try {

        Log::Info("Created window '{}' ({}x{})", descriptor.name, descriptor.width, descriptor.height);
        Log::Info("Window properties:");
        Log::Info("  - Resizable: {}", descriptor.resizable ? "yes" : "no");
        Log::Info("  - Fullscreen: {}", descriptor.fullscreen ? "yes" : "no");

        // TODO: Switch by Platform for now is supported only glfw
        return CreateShared<DefaultWindow>(std::move(descriptor));

    } catch (const std::exception& e) {
        Log::Critical("Failed to create window: {}", e.what());
        FORGE_ASSERT(false, e.what());
    }

    FORGE_ASSERT(window != nullptr, "Failed to create default window - implementation error");
    return window;
}

} // namespace forge
