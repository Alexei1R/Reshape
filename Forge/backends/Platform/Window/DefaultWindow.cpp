// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "DefaultWindow.h"
#include "Forge/Events/Event.h"
#include "Forge/Events/ImplEvent.h"

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

void DefaultWindow::SetCallBackEvents() {
    // NOTE: Set GLFW callbacks
    glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.width = width;
        data.height = height;

        WindowEvent event(width, height, Action::Resize);
        data.eventCallback(event);
    });
    glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int xPos, int yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowEvent event(xPos, yPos, Action::Move);
        data.eventCallback(event);
    });

    glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowEvent event(0, 0, Action::Close);
        data.eventCallback(event);
    });

    glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Action action = focused ? Action::Focus : Action::LoseFocus;
        WindowEvent event(0, 0, action);
        data.eventCallback(event);
    });

    glfwSetWindowIconifyCallback(m_Window, [](GLFWwindow* window, int iconified) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Action action = iconified ? Action::Iconify : Action::Restore;
        WindowEvent event(0, 0, action);
        data.eventCallback(event);
    });

    glfwSetWindowMaximizeCallback(m_Window, [](GLFWwindow* window, int maximized) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        Action action = maximized ? Action::Maximize : Action::Restore;
        WindowEvent event(0, 0, action);
        data.eventCallback(event);
    });

    glfwSetFramebufferSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        WindowEvent event(width, height, Action::FramebufferResize);
        data.eventCallback(event);
    });

    //
    // Key Events
    //

    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            KeyEvent event(key, Action::KeyPress);
            data.eventCallback(event);

            break;
        }
        case GLFW_RELEASE: {
            KeyEvent event(key, Action::KeyRelease);
            data.eventCallback(event);
            break;
        }
        case GLFW_REPEAT: {
            KeyEvent event(key, Action::KeyRepeat);
            data.eventCallback(event);
            break;
        }
        }
    });

    glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        KeyEvent event(keycode, Action::RegisterKeyChar);
        data.eventCallback(event);
    });

    glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        switch (action) {
        case GLFW_PRESS: {
            KeyEvent event(button, Action::KeyPress);
            data.eventCallback(event);
            break;
        }
        case GLFW_RELEASE: {
            KeyEvent event(button, Action::KeyRelease);
            data.eventCallback(event);
            break;
        }
        }
    });

    glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseEvent event(xOffset, yOffset, Action::MouseScroll);
        data.eventCallback(event);
    });

    glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        MouseEvent event(xPos, yPos, Action::MouseMove);
        data.eventCallback(event);
    });

    glfwSetDropCallback(m_Window, [](GLFWwindow* window, int count, const char* paths[]) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

        std::vector<std::string> droppedFiles;
        droppedFiles.reserve(count);

        for (int i = 0; i < count; ++i) {
            droppedFiles.emplace_back(paths[i]);
        }
        DropEvent event(std::move(droppedFiles), Action::Drop);
        data.eventCallback(event);
    });
}
} // namespace forge
