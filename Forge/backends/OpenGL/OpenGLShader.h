// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef OPENGLSHADER_H
#define OPENGLSHADER_H

#include "Forge/Renderer/Shader.h"
#include <vector>

namespace forge {

class OpenGLShader final : public Shader {
public:
    OpenGLShader(const std::string& data, const ShaderOrigin origin) noexcept;
    ~OpenGLShader();

    void Bind() const override;
    void UnBind() const override;

    void SaveSPIRVToFile(const std::vector<uint32_t>& spirvBinary, const std::string& filename) const;

private:
    unsigned int m_ProgramID{0};
};

} // namespace forge

#endif
