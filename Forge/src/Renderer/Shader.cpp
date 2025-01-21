// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/Shader.h"

#include "Forge/Renderer/Shader/ShaderGenerator.h"
#include "Forge/Renderer/Shader/ShaderParser.h"
#include "Forge/Utils/Common.h"
#include "Forge/Utils/FileSystem.h"
#include "Forge/Utils/Log.h"
#include "Forge/Utils/Platform.h"

#include "OpenGL/OpenGLShader.h"

#include <filesystem>
#include <fstream>
#include <unordered_map>

namespace forge {

std::unordered_map<ShaderType, std::vector<uint32_t>> GenerateAndCacheSPIRV(const std::string& data, const ShaderOrigin origin);

Shared<Shader> Shader::Create(const std::string& data, const ShaderOrigin origin) noexcept {
    auto spirvBinaries = GenerateAndCacheSPIRV(data, origin);

    if (spirvBinaries.empty()) {
        Log::Critical("Failed to generate any valid SPIR-V binaries");
        return nullptr;
    }

    auto api = PlatformAPI::GetSelectedGraphicsAPI();
    Shared<Shader> shader;

    switch (api) {
    case GraphicsAPI::OpenGL:
        shader = CreateShared<OpenGLShader>(spirvBinaries);
        break;
    case GraphicsAPI::Vulkan:
    case GraphicsAPI::DirectX12:
    case GraphicsAPI::Metal:
    case GraphicsAPI::WebGL:
    case GraphicsAPI::None:
        Log::Critical("This platform is not supported yet");
        FORGE_ASSERT(false, "This platform is not supported yet");
        return nullptr;
    }

    if (!shader) {
        Log::Critical("Failed to create shader");
        return nullptr;
    }

    return shader;
}

std::unordered_map<ShaderType, std::vector<uint32_t>> GenerateAndCacheSPIRV(const std::string& data, const ShaderOrigin origin) {
    ShaderParser parser(data, origin);
    const auto& shaderSources = parser.GetAllShaderSources();
    const auto& shaderName = parser.GetShaderFileName();
    std::unordered_map<ShaderType, std::vector<uint32_t>> spirvBinaries;

    // Early return if shader name is empty
    if (shaderName.empty()) {
        Log::Critical("Shader name is empty, skipping SPIR-V generation");
        FORGE_ASSERT(false, "Shader name is empty, skipping SPIR-V generation");
        return spirvBinaries;
    }

    auto cachePath = FileSystem::GetShaderCachePath();
    ShaderSPIRVGenerator spirvGenerator;

    // Process each shader type separately
    for (const auto& [type, source] : shaderSources) {
        std::string typeStr = GetShaderTypeName(type);
        auto shaderCachePath = cachePath / (shaderName + "." + typeStr + ".spv");

        // Try to load from cache first
        if (std::filesystem::exists(shaderCachePath)) {
            try {
                std::ifstream inFile(shaderCachePath, std::ios::binary);
                if (inFile) {
                    inFile.seekg(0, std::ios::end);
                    size_t size = inFile.tellg();
                    inFile.seekg(0, std::ios::beg);

                    std::vector<uint32_t> spirv;
                    spirv.resize(size / sizeof(uint32_t));
                    inFile.read(reinterpret_cast<char*>(spirv.data()), size);
                    spirvBinaries[type] = std::move(spirv);
                    continue;
                }
            } catch (const std::exception& e) {
                Log::Error("Failed to read cached shader: {}", e.what());
            }
        }

        // Generate new SPIR-V if cache doesn't exist or couldn't be read
        std::vector<uint32_t> spirv;
        std::unordered_map<ShaderType, std::string> singleShaderSource = {{type, source}};

        auto result = spirvGenerator.Generate(singleShaderSource, shaderName, spirv);
        if (!result) {
            Log::Critical("Failed to generate SPIR-V for shader type: {}", typeStr);
            continue;
        }

        // Cache the newly generated SPIR-V
        try {
            FileSystem::CreateDirectoryIfNotExists(cachePath);
            std::ofstream outFile(shaderCachePath, std::ios::binary | std::ios::trunc);
            if (outFile) {
                outFile.write(reinterpret_cast<const char*>(spirv.data()), spirv.size() * sizeof(uint32_t));
                Log::Trace("Cached SPIR-V shader: {}", shaderCachePath.string());
            }
        } catch (const std::exception& e) {
            Log::Error("Failed to cache shader: {}", e.what());
        }

        spirvBinaries[type] = std::move(spirv);
    }

    return spirvBinaries;
}

} // namespace forge
