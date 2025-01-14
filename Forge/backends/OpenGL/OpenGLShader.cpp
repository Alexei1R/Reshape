// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "OpenGLShader.h"
#include "Forge/Forge.hpp"
#include "Forge/Renderer/Shader.h"
#include "ShaderGenerator.h"
#include "ShaderParser.h"

#include <cstdlib>
#include <fstream>
#include <glad/glad.h>

namespace forge {

[[nodiscard]] constexpr GLenum ShaderTypeToOpenGL(ShaderType type) noexcept;

void OpenGLShader::SaveSPIRVToFile(const std::vector<uint32_t>& spirvBinary, const std::string& filename) const {
    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Failed to open file for writing: " << filename << std::endl;
        return;
    }

    outFile.write(reinterpret_cast<const char*>(spirvBinary.data()), spirvBinary.size() * sizeof(uint32_t));
    if (!outFile) {
        std::cerr << "Failed to write SPIR-V binary to file: " << filename << std::endl;
    }

    outFile.close();
}

OpenGLShader::OpenGLShader(const std::string& data, const ShaderOrigin origin) noexcept {

    ShaderParser parser(data, origin);

    /*    for (auto [type, source] : parser.GetAllShaderSources()) {*/
    /**/
    /*        Log::Trace("Shader with type {} and source {} \n\n\n", GetShaderTypeName(type), source);*/
    /*    }*/

    std::vector<uint32_t> spirvBinary;

    ShaderSPIRVGenerator spirvGenerator;
    spirvGenerator.Generate(parser.GetAllShaderSources(), parser.GetShaderFileName(), spirvBinary);

    // Print the SPIR-V binaries
    std::cout << "SPIR-V Binary: ";
    for (const auto& word : spirvBinary) {
        std::cout << std::hex << word << " ";
    }
    std::cout << std::dec << std::endl; // Reset to decimal format
    //
    //
    //
    // Save to disk in home directory
    const char* homeDir = std::getenv("HOME");
    if (homeDir != nullptr) {
        std::string filePath = std::string(homeDir) + "/" + parser.GetShaderFileName() + ".spv";
        SaveSPIRVToFile(spirvBinary, filePath);
    } else {
        std::cerr << "Home directory not found." << std::endl;
    }
}

OpenGLShader::~OpenGLShader() {}

void OpenGLShader::Bind() const {}

void OpenGLShader::UnBind() const {}

[[nodiscard]] constexpr GLenum ShaderTypeToOpenGL(ShaderType type) noexcept {
    switch (type) {
    case ShaderType::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderType::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderType::Geometry:
        return GL_GEOMETRY_SHADER;
    case ShaderType::Compute:
        return GL_COMPUTE_SHADER;
    case ShaderType::TessControl:
        return GL_TESS_CONTROL_SHADER;
    case ShaderType::TessEvaluation:
        return GL_TESS_EVALUATION_SHADER;
    case ShaderType::Unknown:
        Log::Critical("This shader type is ilegal ");
        break;
    }
    FORGE_ASSERT(false, "Unknown shader type");
    return GL_VERTEX_SHADER;
}

} // namespace forge
