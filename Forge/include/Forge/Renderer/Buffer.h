// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef BUFFER_H
#define BUFFER_H

#include "Forge/Utils/Common.h"
#include <cstdint>
#include <string>
#include <vector>

namespace forge {

// The main interface for all buffers
class Buffer {
public:
    virtual ~Buffer() {}

    virtual void Bind() const = 0;
    virtual void Unbind() const = 0;

private:
};

enum class BufferDataType {
    None = 0,
    Float,
    Float2,
    Float3,
    Float4,
    Mat3,
    Mat4,
    Int,
    Int2,
    Int3,
    Int4,
    Bool

};

constexpr unsigned int GetDataTypeSize(BufferDataType type) {
    switch (type) {
    case BufferDataType::Float:
        return 4;
    case BufferDataType::Float2:
        return 4 * 2;
    case BufferDataType::Float3:
        return 4 * 3;
    case BufferDataType::Float4:
        return 4 * 4;
    case BufferDataType::Mat3:
        return 4 * 3 * 3;
    case BufferDataType::Mat4:
        return 4 * 4 * 4;
    case BufferDataType::Int:
        return 4;
    case BufferDataType::Int2:
        return 4 * 2;
    case BufferDataType::Int3:
        return 4 * 3;
    case BufferDataType::Int4:
        return 4 * 4;
    case BufferDataType::Bool:
        return 1;
    case BufferDataType::None:
        return 0;
    }
    FORGE_ASSERT(false, "Unknown BufferDataType!");
    return 0;
}

struct BufferElement {
    std::string name;
    BufferDataType type;
    unsigned int size;
    unsigned int offset;

    BufferElement(BufferDataType type, std::string name)
        : name(name)
        , type(type)
        , size(GetDataTypeSize(type))
        , offset(0) {}
};

class BufferLayout {
public:
    BufferLayout() = default;
    BufferLayout(std::initializer_list<BufferElement> elements);

    uint32_t GetStride() const;
    const std::vector<BufferElement>& GetElements() const;

    std::vector<BufferElement>::iterator begin();
    std::vector<BufferElement>::iterator end();
    std::vector<BufferElement>::const_iterator begin() const;
    std::vector<BufferElement>::const_iterator end() const;

private:
    void CalculateOffsetsAndStride();

    std::vector<BufferElement> m_Elements;
    uint32_t m_Stride{0};
};

} // namespace forge

#endif
