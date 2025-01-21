// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/RenderAPI.h"
#include "Forge/Utils/Log.h"
#include "OpenGL/OpenGLRenderAPI.h"

namespace forge {

Shared<RenderAPI> RenderAPI::s_Instance = nullptr;

Shared<RenderAPI> RenderAPI::Create() {
    if (s_Instance) {
        Log::Warn("RenderAPI already exists!");
        return s_Instance;
    }

    auto api = PlatformAPI::GetSelectedGraphicsAPI();
    switch (api) {
    case GraphicsAPI::OpenGL:
        s_Instance = CreateShared<OpenGLRenderAPI>();
        break;
    case GraphicsAPI::Vulkan:
    case GraphicsAPI::DirectX12:
    case GraphicsAPI::Metal:
    case GraphicsAPI::WebGL:
    case GraphicsAPI::None:
        Log::Critical("Selected graphics API is not supported yet");
        FORGE_ASSERT(false, "Selected graphics API is not supported yet");
        return nullptr;
    }

    return s_Instance;
}

} // namespace forge
