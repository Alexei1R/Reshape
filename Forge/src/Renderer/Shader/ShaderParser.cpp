// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/Shader/ShaderParser.h"
#include "Forge/Renderer/Shader.h"
#include "Forge/Utils/Common.h"
#include "Forge/Utils/Log.h"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace forge {

ShaderParser::ShaderParser(const std::string& shaderData, const ShaderOrigin shaderOrigin) {
    std::string shaderSource;

    switch (shaderOrigin) {
    case ShaderOrigin::File: {
        if (auto result = ReadFile(std::filesystem::path(shaderData), shaderSource); !result) {
            // TODO : Check for access or request permission
            // make sure to have the correct path resources folder ...
            FORGE_ASSERT(false, "Critical error in reading the shader file");
        }
        break;
    }
    case ShaderOrigin::String:
        shaderSource = std::move(shaderData);
        break;
    }

    if (!shaderSource.empty()) {
        if (auto result = ParseString(shaderSource); !result) {
            FORGE_ASSERT(false, "Failed to parse shader source");
        }
    }
}

ErrorResult ShaderParser::ReadFile(const std::filesystem::path& filePath, std::string& out_shaderData) noexcept {
    if (!std::filesystem::exists(filePath)) {
        Log::Critical("Shader file doesn't exist: {}", filePath.string());
        return ErrorCode::FileNotFound;
    }

    std::ifstream file(filePath, std::ios::in);
    if (!file.is_open()) {
        Log::Critical("Failed to open shader file: {}", filePath.string());
        return ErrorCode::FileAccessDenied;
    }

    try {
        // Read the entire file into the string
        out_shaderData.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

        if (!file.good()) {
            Log::Critical("Failed to read shader file content: {}", filePath.string());
            out_shaderData.clear();
            return ErrorCode::SystemBase;
        }
    } catch (const std::exception& e) {
        Log::Critical("Exception while reading shader file: {} - {}", filePath.string(), e.what());
        out_shaderData.clear();
        return ErrorCode::SystemBase;
    }

    file.close();
    return ErrorCode::Success;
}

ErrorResult ShaderParser::ParseString(const std::string& shader) noexcept {
    if (shader.empty()) {
        return ErrorCode::InvalidArgument;
    }

    std::istringstream shaderStream(shader);
    std::string line;
    ShaderType currentShaderType = ShaderType::Unknown;

    while (std::getline(shaderStream, line)) {
        // Trim whitespace from the line
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
                       return !std::isspace(ch);
                   }));

        // Check for the #type directive
        if (line.rfind("#type", 0) == 0) {         // If line starts with #type
            std::string typeName = line.substr(6); // Get the type name after #type
            auto it = m_ShaderTypeMap.find(typeName);
            if (it != m_ShaderTypeMap.end()) {
                currentShaderType = it->second;
            } else {
                Log::Critical("Unknown shader type: {}", typeName);
                return ErrorCode::InvalidArgument;
            }
        }
        // Check for the #name directive
        else if (line.rfind("#name", 0) == 0) { // If line starts with #name
            m_ShaderName = line.substr(6);      // Get the name after #name
        } else {
            // MOTE: If the current line is not a type add it to the shader
            if (currentShaderType != ShaderType::Unknown) {
                m_Shaders[currentShaderType] += line + "\n";
            }
        }
    }
    return ErrorCode::Success;
}

} // namespace forge
