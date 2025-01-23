// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/Buffer.h"

namespace forge {

// NOTE: BufferLayout
BufferLayout::BufferLayout(std::initializer_list<BufferElement> elements)
    : m_Elements(elements) {
    CalculateOffsetsAndStride();
}

uint32_t BufferLayout::GetStride() const {
    return m_Stride;
}

const std::vector<BufferElement>& BufferLayout::GetElements() const {
    return m_Elements;
}

std::vector<BufferElement>::iterator BufferLayout::begin() {
    return m_Elements.begin();
}

std::vector<BufferElement>::iterator BufferLayout::end() {
    return m_Elements.end();
}

std::vector<BufferElement>::const_iterator BufferLayout::begin() const {
    return m_Elements.begin();
}

std::vector<BufferElement>::const_iterator BufferLayout::end() const {
    return m_Elements.end();
}

void BufferLayout::CalculateOffsetsAndStride() {
    size_t offset = 0;
    m_Stride = 0;
    for (auto& element : m_Elements) {
        element.offset = offset;
        offset += element.size;
        m_Stride += element.size;
    }
}

} // namespace forge
