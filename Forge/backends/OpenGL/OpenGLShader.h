// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "Forge/Renderer/Shader.h"
#include <glad/glad.h>
#include <unordered_map>
#include <vector>

namespace forge {

class OpenGLShader final : public Shader {
public:
    explicit OpenGLShader(std::unordered_map<ShaderType, std::vector<uint32_t>>& shaderSPIRV) noexcept;
    ~OpenGLShader() override;

    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader& operator=(const OpenGLShader&) = delete;

    OpenGLShader(OpenGLShader&&) noexcept = default;
    OpenGLShader& operator=(OpenGLShader&&) noexcept = default;

    void Bind() const override;
    void UnBind() const override;

    // Reflection interface
    [[nodiscard]] const std::vector<ShaderResource>& GetUniformBuffers() const noexcept override {
        return m_UniformBuffers;
    }
    [[nodiscard]] const std::vector<ShaderResource>& GetStorageBuffers() const noexcept override {
        return m_StorageBuffers;
    }
    [[nodiscard]] const std::vector<ShaderResource>& GetSamplers() const noexcept override {
        return m_Samplers;
    }
    [[nodiscard]] const std::vector<ShaderResource>& GetStageInputs() const noexcept override {
        return m_StageInputs;
    }
    [[nodiscard]] const std::vector<ShaderResource>& GetStageOutputs() const noexcept override {
        return m_StageOutputs;
    }

    [[nodiscard]] bool HasUniformBuffer(const std::string& name) const noexcept override;
    [[nodiscard]] bool HasStorageBuffer(const std::string& name) const noexcept override;
    [[nodiscard]] bool HasSampler(const std::string& name) const noexcept override;

    [[nodiscard]] const ShaderResource* FindResource(const std::string& name) const noexcept override;
    [[nodiscard]] const ShaderResource* FindResourceByBinding(uint32_t binding, uint32_t set = 0) const noexcept override;

    [[nodiscard]] inline unsigned int GetProgramID() const noexcept {
        return m_ProgramID;
    }

private:
    [[nodiscard]] unsigned int CompileShader(const std::string& source, GLenum shaderType);
    [[nodiscard]] unsigned int LinkShaders(const std::vector<unsigned int>& shaderIDs);
    void CleanupShaders(const std::vector<unsigned int>& shaderIDs);
    [[nodiscard]] static GLenum ShaderTypeToOpenGL(ShaderType type) noexcept;
    void ReflectShaderResources(const std::vector<uint32_t>& spirv, ShaderType type);

private:
    unsigned int m_ProgramID{0};

    // Reflected resources
    std::vector<ShaderResource> m_UniformBuffers;
    std::vector<ShaderResource> m_Samplers;
    std::vector<ShaderResource> m_StorageBuffers;
    std::vector<ShaderResource> m_StageInputs;
    std::vector<ShaderResource> m_StageOutputs;
};

} // namespace forge

#endif
