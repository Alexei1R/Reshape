// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "Forge/Renderer/Shader.h"
#include <unordered_map>
#include <vector>

#include <glad/glad.h>

namespace forge {

class OpenGLShader final : public Shader {
public:
    explicit OpenGLShader(std::unordered_map<ShaderType, std::vector<uint32_t>>& shaderSPIRV) noexcept;
    ~OpenGLShader() override;

    // Delete copy operations
    OpenGLShader(const OpenGLShader&) = delete;
    OpenGLShader& operator=(const OpenGLShader&) = delete;

    // Allow move operations
    OpenGLShader(OpenGLShader&&) noexcept = default;
    OpenGLShader& operator=(OpenGLShader&&) noexcept = default;

    void Bind() const override;
    void UnBind() const override;

    [[nodiscard]] inline unsigned int GetProgramID() const noexcept { return m_ProgramID; }

private:
    [[nodiscard]] unsigned int CompileShader(const std::string& source, GLenum shaderType);
    [[nodiscard]] unsigned int LinkShaders(const std::vector<unsigned int>& shaderIDs);

private:
    unsigned int m_ProgramID{0};
};

} // namespace forge

#endif
