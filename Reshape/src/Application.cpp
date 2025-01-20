// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Application.h"
#include "Forge/Renderer/GraphicsContext.h"
#include "Forge/Renderer/Shader.h"

#include <glad/glad.h>
// without this comnet lsp may swap them
#include <GLFW/glfw3.h>

namespace reshape {

Application::Application() {
    forge::Log::Info("Application constructor");
    m_Window = forge::Window::Create();

    m_Context = forge::GraphicsContext::Create(m_Window);
    m_RenderAPI = forge::RenderAPI::Create();

    {
        // Define the triangle vertices with positions and texture coordinates
        const float vertices[] = {
            // positions      // texture coords
            0.0f,  0.5f,  0.0f, 0.5f, 1.0f, // top
            -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, // bottom left
            0.5f,  -0.5f, 0.0f, 1.0f, 0.0f  // bottom right
        };

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    m_Shader = forge::Shader::Create("shaders/main.glsl", forge::ShaderOrigin::File);
    m_Shader->Bind();

    for (const auto& input : m_Shader->GetStageInputs()) {
        forge::Log::Info("Vertex attribute: {} at location {}, size {}", input.name, input.location, input.size);
    }

    if (m_Shader->HasUniformBuffer("SceneData")) {
        if (auto* ubo = m_Shader->FindResource("SceneData")) {
            forge::Log::Info("SceneData UBO size: {} bytes", ubo->size);
        }
    }
}

Application::~Application() {
    if (m_VAO != 0) {
        glDeleteVertexArrays(1, &m_VAO);
    }
    if (m_VBO != 0) {
        glDeleteBuffers(1, &m_VBO);
    }
}

void Application::Run() {
    while (!glfwWindowShouldClose(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()))) {
        m_Window->Update();
        PROFILE_SCOPE("Main Loop");

        forge::ClearState clearState;
        clearState.color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearState.clearColor = true;
        clearState.clearDepth = true;
        m_RenderAPI->Clear(clearState);

        m_Shader->Bind();

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
        glfwPollEvents();
    }
}

} // namespace reshape
