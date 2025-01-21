// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
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

[[nodiscard]] GLenum OpenGLShader::ShaderTypeToOpenGL(ShaderType type) noexcept {
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
    default:
        Log::Critical("Unknown shader type");
        return 0;
    }
}

OpenGLShader::OpenGLShader(std::unordered_map<ShaderType, std::vector<uint32_t>>& shaderSPIRV) noexcept {
    std::vector<unsigned int> shaderIDs;
    shaderIDs.reserve(shaderSPIRV.size());

    for (auto& [type, spirv_binary] : shaderSPIRV) {
        ReflectShaderResources(spirv_binary, type);

        spirv_cross::CompilerGLSL glsl(std::move(spirv_binary));

        spirv_cross::CompilerGLSL::Options options;
        options.version = 450;
        options.es = false;
        glsl.set_common_options(options);

        std::string source = glsl.compile();

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

void OpenGLShader::ReflectShaderResources(const std::vector<uint32_t>& spirv, ShaderType type) {
    try {
        spirv_cross::CompilerGLSL glsl(spirv);
        spirv_cross::ShaderResources resources = glsl.get_shader_resources();

        // Reflect Uniform Buffers
        for (const auto& resource : resources.uniform_buffers) {
            ShaderResource ubo{};
            ubo.binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            ubo.set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            ubo.name = resource.name;
            ubo.type = ShaderResourceType::UniformBuffer;

            // Get buffer size and member count
            const auto& bufferType = glsl.get_type(resource.base_type_id);
            ubo.size = glsl.get_declared_struct_size(bufferType);
            ubo.memberCount = bufferType.member_types.size();

            m_UniformBuffers.push_back(ubo);
        }

        // Reflect Storage Buffers
        for (const auto& resource : resources.storage_buffers) {
            ShaderResource ssbo{};
            ssbo.binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            ssbo.set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            ssbo.name = resource.name;
            ssbo.type = ShaderResourceType::StorageBuffer;

            const auto& bufferType = glsl.get_type(resource.base_type_id);
            ssbo.size = glsl.get_declared_struct_size(bufferType);
            ssbo.memberCount = bufferType.member_types.size();

            m_StorageBuffers.push_back(ssbo);
        }

        // Reflect Samplers
        for (const auto& resource : resources.sampled_images) {
            ShaderResource sampler{};
            sampler.binding = glsl.get_decoration(resource.id, spv::DecorationBinding);
            sampler.set = glsl.get_decoration(resource.id, spv::DecorationDescriptorSet);
            sampler.name = resource.name;
            sampler.type = ShaderResourceType::Sampler;

            m_Samplers.push_back(sampler);
        }

        // Reflect Stage Inputs (vertex attributes for vertex shader)
        if (type == ShaderType::Vertex) {
            for (const auto& resource : resources.stage_inputs) {
                ShaderResource input{};
                input.location = glsl.get_decoration(resource.id, spv::DecorationLocation);
                input.name = resource.name;
                input.type = ShaderResourceType::Input;

                const auto& inputType = glsl.get_type(resource.type_id);
                input.size = inputType.vecsize * sizeof(float); // Assuming float attributes

                m_StageInputs.push_back(input);
            }
        }

        // Reflect Stage Outputs
        for (const auto& resource : resources.stage_outputs) {
            ShaderResource output{};
            output.location = glsl.get_decoration(resource.id, spv::DecorationLocation);
            output.name = resource.name;
            output.type = ShaderResourceType::Output;

            m_StageOutputs.push_back(output);
        }

    } catch (const spirv_cross::CompilerError& e) {
        Log::Error("SPIRV-Cross reflection error: {}", e.what());
    }
}

bool OpenGLShader::HasUniformBuffer(const std::string& name) const noexcept {
    return std::find_if(m_UniformBuffers.begin(), m_UniformBuffers.end(), [&name](const ShaderResource& resource) {
               return resource.name == name;
           }) != m_UniformBuffers.end();
}

bool OpenGLShader::HasStorageBuffer(const std::string& name) const noexcept {
    return std::find_if(m_StorageBuffers.begin(), m_StorageBuffers.end(), [&name](const ShaderResource& resource) {
               return resource.name == name;
           }) != m_StorageBuffers.end();
}

bool OpenGLShader::HasSampler(const std::string& name) const noexcept {
    return std::find_if(m_Samplers.begin(), m_Samplers.end(), [&name](const ShaderResource& resource) {
               return resource.name == name;
           }) != m_Samplers.end();
}

const ShaderResource* OpenGLShader::FindResource(const std::string& name) const noexcept {
    // Search in all resource collections
    auto findInCollection = [&name](const std::vector<ShaderResource>& collection) {
        auto it = std::find_if(collection.begin(), collection.end(), [&name](const ShaderResource& resource) {
            return resource.name == name;
        });
        return it != collection.end() ? &(*it) : nullptr;
    };

    if (auto* resource = findInCollection(m_UniformBuffers))
        return resource;
    if (auto* resource = findInCollection(m_StorageBuffers))
        return resource;
    if (auto* resource = findInCollection(m_Samplers))
        return resource;
    if (auto* resource = findInCollection(m_StageInputs))
        return resource;
    if (auto* resource = findInCollection(m_StageOutputs))
        return resource;

    return nullptr;
}

const ShaderResource* OpenGLShader::FindResourceByBinding(uint32_t binding, uint32_t set) const noexcept {
    // Search in resources that can have bindings
    auto findInCollection = [binding, set](const std::vector<ShaderResource>& collection) {
        auto it = std::find_if(collection.begin(), collection.end(), [binding, set](const ShaderResource& resource) {
            return resource.binding == binding && resource.set == set;
        });
        return it != collection.end() ? &(*it) : nullptr;
    };

    if (auto* resource = findInCollection(m_UniformBuffers))
        return resource;
    if (auto* resource = findInCollection(m_StorageBuffers))
        return resource;
    if (auto* resource = findInCollection(m_Samplers))
        return resource;

    return nullptr;
}

} // namespace forge
