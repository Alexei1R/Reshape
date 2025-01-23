// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef OPENGLBUFFER_H
#define OPENGLBUFFER_H

#include "Forge/Renderer/BufferImpl.h"
#include "glad/glad.h"

namespace forge {

class OpenGLVertexBuffer : public VertexBuffer {
public:
    OpenGLVertexBuffer(const void* data, uint32_t count, BufferDrawMode drawMode);
    virtual ~OpenGLVertexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SubmitData(const void* data, uint32_t count, uint32_t offset = 0) override;
    virtual const BufferLayout& GetLayout() const override {
        return m_Layout;
    }
    virtual void SetLayout(const BufferLayout& layout) override {
        m_Layout = layout;
    }

private:
    uint32_t m_RendererID;
    BufferLayout m_Layout;
    BufferDrawMode m_DrawMode;
};

class OpenGLIndexBuffer : public IndexBuffer {
public:
    OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferDrawMode drawMode);
    virtual ~OpenGLIndexBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void SubmitData(const void* data, uint32_t count, uint32_t offset = 0) override;
    virtual uint32_t GetCount() const override {
        return m_Count;
    }

private:
    uint32_t m_RendererID;
    uint32_t m_Count;
    BufferDrawMode m_DrawMode;
};

class OpenGLVertexArrayBuffer : public VertexArrayBuffer {
public:
    OpenGLVertexArrayBuffer();
    virtual ~OpenGLVertexArrayBuffer();

    virtual void Bind() const override;
    virtual void Unbind() const override;
    virtual void AddVertexBuffer(Shared<VertexBuffer>& vertexBuffer) override;
    virtual void SetIndexBuffer(Shared<IndexBuffer>& indexBuffer) override;
    virtual const Shared<IndexBuffer>& GetIndexBuffer() const override {
        return m_IndexBuffer;
    }

private:
    std::vector<Shared<VertexBuffer>> m_VertexBuffers;
    Shared<IndexBuffer> m_IndexBuffer;
    uint32_t m_RendererID;
};

uint32_t GetComponentCount(BufferDataType type);
GLenum BufferDataTypeToOpenGLBaseType(BufferDataType type);

} // namespace forge

#endif
