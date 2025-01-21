// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef SHADERGENERATOR_H
#define SHADERGENERATOR_H

#include "Forge/Renderer/Shader.h"
#include "Forge/Utils/ErrorCodes.h"
#include <cstdint>
#include <unordered_map>

#include <shaderc/shaderc.hpp>

namespace forge {

class ShaderSPIRVGenerator {
public:
    ShaderSPIRVGenerator() = default;
    ~ShaderSPIRVGenerator() = default;

    ErrorResult Generate(const std::unordered_map<ShaderType, std::string>& shadersSource, std::string name,
                         std::vector<uint32_t>& shaderSpirvBinary) noexcept;

private:
    shaderc_shader_kind ToShadercType(ShaderType type);
};

} // namespace forge

#endif
