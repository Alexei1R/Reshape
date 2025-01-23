// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Forge/Renderer/BufferImpl.h"
#include "Forge/Utils/Log.h"
#include "Forge/Utils/Platform.h"
#include "OpenGL/OpenGLBuffer.h"

namespace forge {

Shared<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t count, BufferDrawMode mode) {

    auto api = PlatformAPI::GetDefaultGraphicsAPI();

    try {
        switch (api) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLVertexBuffer>(data, count, mode);
        case GraphicsAPI::Vulkan:
        case GraphicsAPI::DirectX12:
        case GraphicsAPI::Metal:
            Log::Error("VertexBuffer: {} not implemented", PlatformAPI::GetGraphicsAPIName(api));
            FORGE_ASSERT(false, "Graphics API not implemented for VertexBuffer");
            break;
        default:
            Log::Error("Unknown graphics API");
            FORGE_ASSERT(false, "Unknown graphics API");
        }
    } catch (const std::exception& e) {
        Log::Error("Failed to create vertex buffer: {}", e.what());
        FORGE_ASSERT(false, e.what());
    }

    return nullptr;
}

Shared<IndexBuffer> IndexBuffer::Create(uint32_t* data, uint32_t count, BufferDrawMode mode) {

    auto api = PlatformAPI::GetDefaultGraphicsAPI();

    try {
        switch (api) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLIndexBuffer>(data, count, mode);
        case GraphicsAPI::Vulkan:
        case GraphicsAPI::DirectX12:
        case GraphicsAPI::Metal:
            Log::Error("IndexBuffer: {} not implemented", PlatformAPI::GetGraphicsAPIName(api));
            FORGE_ASSERT(false, "Graphics API not implemented for IndexBuffer");
            break;
        default:
            Log::Error("Unknown graphics API");
            FORGE_ASSERT(false, "Unknown graphics API");
        }
    } catch (const std::exception& e) {
        Log::Error("Failed to create index buffer: {}", e.what());
        FORGE_ASSERT(false, e.what());
    }

    return nullptr;
}

Shared<VertexArrayBuffer> VertexArrayBuffer::Create() {
    auto api = PlatformAPI::GetDefaultGraphicsAPI();

    try {
        switch (api) {
        case GraphicsAPI::OpenGL:
            return std::make_shared<OpenGLVertexArrayBuffer>();
        case GraphicsAPI::Vulkan:
        case GraphicsAPI::DirectX12:
        case GraphicsAPI::Metal:
            Log::Error("VertexArrayBuffer: {} not implemented", PlatformAPI::GetGraphicsAPIName(api));
            FORGE_ASSERT(false, "Graphics API not implemented for VertexArrayBuffer");
            break;
        default:
            Log::Error("Unknown graphics API");
            FORGE_ASSERT(false, "Unknown graphics API");
        }
    } catch (const std::exception& e) {
        Log::Error("Failed to create vertex array buffer: {}", e.what());
        FORGE_ASSERT(false, e.what());
    }

    return nullptr;
}

} // namespace forge
