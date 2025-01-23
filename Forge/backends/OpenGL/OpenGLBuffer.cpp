// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "OpenGLBuffer.h"
#include "Forge/Utils/Common.h"
#include "Forge/Utils/Log.h"

namespace forge {

uint32_t GetComponentCount(BufferDataType type) {
    switch (type) {
    case BufferDataType::Float:
        return 1;
    case BufferDataType::Float2:
        return 2;
    case BufferDataType::Float3:
        return 3;
    case BufferDataType::Float4:
        return 4;
    case BufferDataType::Mat3:
        return 3 * 3;
    case BufferDataType::Mat4:
        return 4 * 4;
    case BufferDataType::Int:
        return 1;
    case BufferDataType::Int2:
        return 2;
    case BufferDataType::Int3:
        return 3;
    case BufferDataType::Int4:
        return 4;
    case BufferDataType::Bool:
        return 1;
    default:
        return 0;
    }
}

GLenum BufferDataTypeToOpenGLBaseType(BufferDataType type) {
    switch (type) {
    case BufferDataType::Float:
    case BufferDataType::Float2:
    case BufferDataType::Float3:
    case BufferDataType::Float4:
    case BufferDataType::Mat3:
    case BufferDataType::Mat4:
        return GL_FLOAT;
    case BufferDataType::Int:
    case BufferDataType::Int2:
    case BufferDataType::Int3:
    case BufferDataType::Int4:
        return GL_INT;
    case BufferDataType::Bool:
        return GL_BOOL;
    default:
        FORGE_ASSERT(false, "Unknown BufferDataType!");
        return 0;
    }
}

//========================================
//  Vertex Buffer Implementation
//========================================

OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t count, BufferDrawMode drawMode)
    : m_DrawMode(drawMode) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);

    switch (drawMode) {
    case BufferDrawMode::Static:
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_STATIC_DRAW);
        break;
    case BufferDrawMode::Dynamic:
        glBufferData(GL_ARRAY_BUFFER, count * sizeof(float), data, GL_DYNAMIC_DRAW);
        break;
    }
}

void OpenGLVertexBuffer::SubmitData(const void* data, uint32_t count, uint32_t offset) {
    Bind();

    if (m_DrawMode == BufferDrawMode::Dynamic) {
        glBufferSubData(GL_ARRAY_BUFFER, offset, count, data);
    } else {
        FORGE_ASSERT(false, "Can't submit data to a static OpenGLVertexBuffer. Set BufferDrawMode to "
                            "Dynamic.");
    }
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLVertexBuffer::Bind() const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
}

void OpenGLVertexBuffer::Unbind() const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//========================================
//  Index Buffer Implementation
//========================================

OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* data, uint32_t count, BufferDrawMode drawMode)
    : m_Count(count)
    , m_DrawMode(drawMode) {
    glGenBuffers(1, &m_RendererID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);

    switch (drawMode) {
    case BufferDrawMode::Static:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
        break;
    case BufferDrawMode::Dynamic:
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_DYNAMIC_DRAW);
        break;
    }
}

void OpenGLIndexBuffer::SubmitData(const void* data, uint32_t count, uint32_t offset) {
    Bind();

    if (m_DrawMode == BufferDrawMode::Dynamic) {
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, count, data);
    } else {
        FORGE_ASSERT(false, "Can't submit data to a static OpenGLIndexBuffer. Set BufferDrawMode to "
                            "Dynamic.");
    }
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() {
    glDeleteBuffers(1, &m_RendererID);
}

void OpenGLIndexBuffer::Bind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
}

void OpenGLIndexBuffer::Unbind() const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//========================================
//  Vertex Array Buffer Implementation
//========================================

OpenGLVertexArrayBuffer::OpenGLVertexArrayBuffer() {
    glGenVertexArrays(1, &m_RendererID);
}

OpenGLVertexArrayBuffer::~OpenGLVertexArrayBuffer() {
    glDeleteVertexArrays(1, &m_RendererID);
}

void OpenGLVertexArrayBuffer::Bind() const {
    glBindVertexArray(m_RendererID);
}

void OpenGLVertexArrayBuffer::Unbind() const {
    glBindVertexArray(0);
}

void OpenGLVertexArrayBuffer::AddVertexBuffer(Shared<VertexBuffer>& vertexBuffer) {
    if (vertexBuffer->GetLayout().GetElements().empty()) {
        Log::Error("OpenGLVertexBuffer has no layout!");
        return;
    }

    glBindVertexArray(m_RendererID);
    vertexBuffer->Bind();

    const auto& layout = vertexBuffer->GetLayout();
    uint32_t index = 0;
    for (const auto& element : layout) {
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(index, GetComponentCount(element.type), BufferDataTypeToOpenGLBaseType(element.type), GL_FALSE,
                              layout.GetStride(), (const void*)(intptr_t)element.offset);
        index++;
    }

    m_VertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArrayBuffer::SetIndexBuffer(Shared<IndexBuffer>& indexBuffer) {
    glBindVertexArray(m_RendererID);
    indexBuffer->Bind();

    m_IndexBuffer = indexBuffer;
}

} // namespace forge
