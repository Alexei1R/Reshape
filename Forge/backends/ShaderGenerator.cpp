// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "ShaderGenerator.h"
#include "Forge/Utils/ErrorCodes.h"

namespace forge {

ErrorResult ShaderSPIRVGenerator::Generate(const std::unordered_map<ShaderType, std::string>& shadersSource, std::string name,
                                           std::vector<uint32_t>& shaderSpirvBinary) noexcept {
    shaderc::Compiler compiler;
    shaderc::CompileOptions options;

    // Like -DMY_DEFINE=1
    options.AddMacroDefinition("MY_DEFINE", "1");
    options.SetOptimizationLevel(shaderc_optimization_level_size);

    for (const auto& [type, source] : shadersSource) {
        shaderc::SpvCompilationResult module = compiler.CompileGlslToSpv(source, ToShadercType(type), name.c_str(), options);

        if (module.GetCompilationStatus() != shaderc_compilation_status_success) {
            std::cerr << module.GetErrorMessage();
            return ErrorCode::ResourceCreationFailed;
        }

        shaderSpirvBinary.assign(module.cbegin(), module.cend());
    }

    return ErrorCode::Success;
}

shaderc_shader_kind ShaderSPIRVGenerator::ToShadercType(ShaderType type) {
    switch (type) {
    case ShaderType::Vertex:
        return shaderc_glsl_vertex_shader;
    case ShaderType::Fragment:
        return shaderc_glsl_fragment_shader;
    case ShaderType::Geometry:
        return shaderc_glsl_geometry_shader;
    case ShaderType::Compute:
        return shaderc_glsl_compute_shader;
    case ShaderType::TessControl:
        return shaderc_glsl_tess_control_shader;
    case ShaderType::TessEvaluation:
        return shaderc_glsl_tess_evaluation_shader;
    default:
        return shaderc_glsl_infer_from_source;
    }
}

} // namespace forge
