// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "DefaultWindow.h"

namespace forge {

DefaultWindow::DefaultWindow(const WindowDescriptor& descriptor) {

    // Store window data
    m_Data.width = descriptor.width;
    m_Data.height = descriptor.height;
    m_Data.name = descriptor.name;
    m_Data.fullscreen = descriptor.fullscreen;

    FORGE_ASSERT(glfwInit(), "ERROR Init GLFW")

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    m_Window = glfwCreateWindow(m_Data.width, m_Data.height, m_Data.name.c_str(), nullptr, nullptr);
    FORGE_ASSERT(m_Window, "ERROR to create window")
    glfwMakeContextCurrent(m_Window);

    glfwSetWindowUserPointer(m_Window, &m_Data);
}

DefaultWindow::~DefaultWindow() {
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}

void DefaultWindow::Update() {
    glfwPollEvents();
}
void DefaultWindow::EnableVSync(bool enable) {
    if (enable) {
        glfwSwapInterval(1);
    } else {
        glfwSwapInterval(0);
    }
}

} // namespace forge
