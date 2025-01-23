// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef APPLICATION_H
#define APPLICATION_H

#include "glad/glad.h"

#include "Forge/Forge.hpp"

namespace reshape {

class Application {
public:
    Application();
    ~Application();

    void Run();

protected:
    void HandleEvent(const forge::Event& event);

private:
    Shared<forge::Window> m_Window;
    Shared<forge::Shader> m_Shader;
    Shared<forge::RenderAPI> m_RenderAPI;
    Unique<forge::GraphicsContext> m_Context;

    bool m_IsRunning{true};

    Shared<forge::VertexArrayBuffer> m_VAO;
    Shared<forge::VertexBuffer> m_VBO;
    Shared<forge::IndexBuffer> m_EBO;

    // Temporary OpenGL UBO handles until Forge UBO is implemented
    uint32_t m_CameraUBO{0};
    uint32_t m_TransformUBO{0};

    forge::math::mat4f m_ViewProjection{1.0f};
    forge::math::mat4f m_Transform{1.0f};
};

} // namespace reshape

#endif
