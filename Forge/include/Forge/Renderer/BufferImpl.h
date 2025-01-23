// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef BUFFERIMPL_H
#define BUFFERIMPL_H

#include "Forge/Renderer/Buffer.h"
#include <cstdint>
namespace forge {

enum class BufferDrawMode : uint8_t { Static, Dynamic };

//========================================
//  Vertex Buffer
//========================================

class VertexBuffer : public Buffer {
public:
    virtual ~VertexBuffer() = default;
    virtual void SubmitData(const void* data, uint32_t count, uint32_t offset = 0) = 0;
    virtual const BufferLayout& GetLayout() const = 0;
    virtual void SetLayout(const BufferLayout& layout) = 0;

    static Shared<VertexBuffer> Create(const void* data, uint32_t count, BufferDrawMode mode = BufferDrawMode::Static);
};

//========================================
//  Index Buffer
//========================================

class IndexBuffer : public Buffer {
public:
    virtual ~IndexBuffer() = default;
    virtual void SubmitData(const void* data, uint32_t count, uint32_t offset = 0) = 0;
    virtual uint32_t GetCount() const = 0;

    static Shared<IndexBuffer> Create(uint32_t* data, uint32_t count, BufferDrawMode mode = BufferDrawMode::Static);
};

//========================================
//  VertexArray Buffer
//========================================

class VertexArrayBuffer : public Buffer {
public:
    virtual ~VertexArrayBuffer() = default;
    virtual void AddVertexBuffer(Shared<VertexBuffer>& vertexBuffer) = 0;
    virtual void SetIndexBuffer(Shared<IndexBuffer>& indexBuffer) = 0;
    virtual const Shared<IndexBuffer>& GetIndexBuffer() const = 0;

    static Shared<VertexArrayBuffer> Create();
};

} // namespace forge

#endif
