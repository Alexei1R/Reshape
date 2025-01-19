// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef SHADERPARSER_H
#define SHADERPARSER_H

#include "Forge/Renderer/Shader.h"
#include "Forge/Utils/ErrorCodes.h"
#include <filesystem>
#include <string>
#include <unordered_map>

namespace forge {

class ShaderParser {
public:
    ShaderParser(const std::string& shaderData, const ShaderOrigin shaderOrigin);

    ErrorResult ParseString(const std::string& shader) noexcept;
    ErrorResult ReadFile(const std::filesystem::path& filePath, std::string& out_shaderData) noexcept;

    [[nodiscard]] inline const std::unordered_map<ShaderType, std::string>& GetAllShaderSources() const noexcept {
        return m_Shaders;
    }

    [[nodiscard]] inline const std::string& GetShaderFileName() const noexcept {
        return m_ShaderName;
    }

private:
    std::unordered_map<ShaderType, std::string> m_Shaders;
    std::string m_ShaderName;

    const std::unordered_map<std::string, ShaderType> m_ShaderTypeMap{
        {"vertex", ShaderType::Vertex},   {"fragment", ShaderType::Fragment},       {"geometry", ShaderType::Geometry},
        {"compute", ShaderType::Compute}, {"tesscontrol", ShaderType::TessControl}, {"tessevaluation", ShaderType::TessEvaluation},
    };
};

} // namespace forge

#endif
