// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "OpenGLShader.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Utils/Log.h"

#include <cstdlib>
#include <fstream>
#include <glad/glad.h>

#include "spirv_cross/spirv.hpp"
#include "spirv_cross/spirv_glsl.hpp"

namespace forge {

[[nodiscard]] constexpr GLenum ShaderTypeToOpenGL(ShaderType type) noexcept;

OpenGLShader::OpenGLShader(std::unordered_map<ShaderType, std::vector<uint32_t>>& shaderSPIRV) noexcept {
    std::vector<unsigned int> shaderIDs;
    shaderIDs.reserve(shaderSPIRV.size());

    // Convert and compile each shader stage
    for (const auto& [type, spirv_binary] : shaderSPIRV) {
        // Convert SPIR-V to GLSL using SPIRV-Cross
        spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));

        // Set GLSL version and options
        spirv_cross::CompilerGLSL::Options options;
        options.version = 450;
        options.es = false;
        glsl.set_common_options(options);

        // Compile to GLSL
        std::string source = glsl.compile();
        
        // Compile the shader
        unsigned int shaderID = CompileShader(source, ShaderTypeToOpenGL(type));
        if (shaderID) {
            shaderIDs.push_back(shaderID);
        } else {
            Log::Critical("Failed to compile {} shader", GetShaderTypeName(type));
            
            // Cleanup already compiled shaders
            for (auto id : shaderIDs) {
                glDeleteShader(id);
            }
            return;
        }
    }

    // Link the shader program
    m_ProgramID = LinkShaders(shaderIDs);
    if (!m_ProgramID) {
        Log::Critical("Failed to link shader program");
    }
}

OpenGLShader::~OpenGLShader() {
    if (m_ProgramID) {
        glDeleteProgram(m_ProgramID);
    }
}

void OpenGLShader::Bind() const {
    FORGE_ASSERT(m_ProgramID, "Attempting to bind invalid shader program");
    glUseProgram(m_ProgramID);
}

void OpenGLShader::UnBind() const {
    glUseProgram(0);
}

unsigned int OpenGLShader::CompileShader(const std::string& source, GLenum shaderType) {
    unsigned int shaderID = glCreateShader(shaderType);
    const char* src = source.c_str();
    glShaderSource(shaderID, 1, &src, nullptr);
    glCompileShader(shaderID);

    int success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetShaderInfoLog(shaderID, 1024, nullptr, infoLog);
        Log::Critical("Shader compilation failed: {}", std::string(infoLog));
        glDeleteShader(shaderID);
        return 0;
    }

    return shaderID;
}

unsigned int OpenGLShader::LinkShaders(const std::vector<unsigned int>& shaderIDs) {
    if (shaderIDs.empty()) {
        Log::Critical("No shaders to link");
        return 0;
    }

    unsigned int programID = glCreateProgram();

    for (unsigned int shaderID : shaderIDs) {
        glAttachShader(programID, shaderID);
    }

    glLinkProgram(programID);

    int success;
    glGetProgramiv(programID, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[1024];
        glGetProgramInfoLog(programID, 1024, nullptr, infoLog);
        Log::Critical("Shader program linking failed: {}", std::string(infoLog));
        glDeleteProgram(programID);

        for (unsigned int shaderID : shaderIDs) {
            glDeleteShader(shaderID);
        }

        return 0;
    }

    // Cleanup individual shaders
    for (unsigned int shaderID : shaderIDs) {
        glDetachShader(programID, shaderID);
        glDeleteShader(shaderID);
    }

    return programID;
}

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
        Log::Critical("Unknown shader type");
        break;
    }
    FORGE_ASSERT(false, "Unknown shader type");
    return GL_VERTEX_SHADER;
}

} // namespace forge
