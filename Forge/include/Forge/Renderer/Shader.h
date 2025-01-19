// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef SHADER_H
#define SHADER_H

#include "Forge/Utils/Common.h"
#include <cstdint>
#include <string>
#include <vector>

namespace forge {

enum class ShaderType {
    Unknown,
    Vertex,
    Fragment,
    Geometry,
    Compute,
    TessControl,
    TessEvaluation,
};

enum class ShaderOrigin : uint8_t {
    File,
    String,
};

enum class ShaderResourceType {
    UniformBuffer,
    StorageBuffer,
    Sampler,
    Input,
    Output,
    Unknown
};

struct ShaderResource {
    uint32_t binding{0};
    uint32_t location{0};
    uint32_t set{0};
    std::string name;
    ShaderResourceType type{ShaderResourceType::Unknown};
    uint32_t size{0};
    uint32_t memberCount{0};
};

// NOTE: Base shader interface
class Shader {
public:
    virtual ~Shader() = default;

    virtual void Bind() const = 0;
    virtual void UnBind() const = 0;

    // Reflection interface
    [[nodiscard]] virtual const std::vector<ShaderResource>& GetUniformBuffers() const noexcept = 0;
    [[nodiscard]] virtual const std::vector<ShaderResource>& GetStorageBuffers() const noexcept = 0;
    [[nodiscard]] virtual const std::vector<ShaderResource>& GetSamplers() const noexcept = 0;
    [[nodiscard]] virtual const std::vector<ShaderResource>& GetStageInputs() const noexcept = 0;
    [[nodiscard]] virtual const std::vector<ShaderResource>& GetStageOutputs() const noexcept = 0;

    [[nodiscard]] virtual bool HasUniformBuffer(const std::string& name) const noexcept = 0;
    [[nodiscard]] virtual bool HasStorageBuffer(const std::string& name) const noexcept = 0;
    [[nodiscard]] virtual bool HasSampler(const std::string& name) const noexcept = 0;
    
    [[nodiscard]] virtual const ShaderResource* FindResource(const std::string& name) const noexcept = 0;
    [[nodiscard]] virtual const ShaderResource* FindResourceByBinding(uint32_t binding, uint32_t set = 0) const noexcept = 0;

    // NOTE: Factory method to create appropriate shader type
    [[nodiscard]] static Shared<Shader> Create(const std::string& data, const ShaderOrigin origin = ShaderOrigin::File) noexcept;

protected:
    Shader() = default;

    // NOTE: Delete copy operations
    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    // NOTE: Allow move operations
    Shader(Shader&&) noexcept = default;
    Shader& operator=(Shader&&) noexcept = default;
};

constexpr std::string GetShaderTypeName(ShaderType type) {
    switch (type) {
    case ShaderType::Vertex:
        return "Vertex";
    case ShaderType::Fragment:
        return "Fragment";
    case ShaderType::Geometry:
        return "Geometry";
    case ShaderType::Compute:
        return "Compute";
    case ShaderType::TessControl:
        return "TessControl";
    case ShaderType::TessEvaluation:
        return "TessEvaluation";
    default:
        return "Unknown";
    }
}

} // namespace forge

#endif
