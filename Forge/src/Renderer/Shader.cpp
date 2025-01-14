// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/Shader.h"

#include "Forge/Utils/Common.h"
#include "Forge/Utils/Log.h"
#include "Forge/Utils/Platform.h"
#include "OpenGL/OpenGLShader.h"

namespace forge {

Shared<Shader> Shader::Create(const std::string& data, const ShaderOrigin origin) noexcept {

    auto api = PlatformAPI::GetSelectedGraphicsAPI();
    Shared<Shader> shader;
    switch (api) {

    case GraphicsAPI::OpenGL:

        shader = CreateShared<OpenGLShader>(data, origin);
        break;
    case GraphicsAPI::Vulkan:
    case GraphicsAPI::DirectX12:
    case GraphicsAPI::Metal:
    case GraphicsAPI::WebGL:
    case GraphicsAPI::None:
        shader = nullptr;
        Log::Critical("This paltform is not supported yet");
        FORGE_ASSERT(false, "This paltform is not supported yet")
        break;
    }

    if (!shader) {
        Log::Critical("Failed to create OpenGL shader");
        return nullptr;
    }

    return shader;
}

} // namespace forge
