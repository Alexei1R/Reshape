// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Application.h"

#include <glad/glad.h>
// without this comnet lsp may swap them
#include <GLFW/glfw3.h>

namespace reshape {

Application::Application() {
    forge::Log::Info("Application constructor");
    m_Window = forge::Window::Create();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        forge::Log::Critical("GLAD error");
        return;
    }

    {

        // Define the triangle vertices
        const float vertices[] = {0.0f, 0.5f, 0.0f, -0.5f, -0.5f, 0.0f, 0.5f, -0.5f, 0.0f};

        glGenVertexArrays(1, &m_VAO);
        glGenBuffers(1, &m_VBO);

        glBindVertexArray(m_VAO);

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
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

        glClearColor(0.1f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(m_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
        glfwPollEvents();
    }
}

} // namespace reshape
