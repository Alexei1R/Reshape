// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "OpenGLContext.h"
#include "Forge/Renderer/Window.h"
#include "Forge/Utils/Log.h"

namespace forge {

OpenGLContext::OpenGLContext(Shared<Window> window)
    : m_Window(window) {
    FORGE_ASSERT(window, "Window handle is null!");
    m_Window = window;
}

OpenGLContext::~OpenGLContext() {
    // OpenGL context is automatically destroyed with the window
}

bool OpenGLContext::Init() {
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));

    // Initialize GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        Log::Critical("Failed to initialize GLAD");
        return false;
    }

    // Log OpenGL info
    Log::Info("OpenGL Info:");
    Log::Info("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    Log::Info("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    Log::Info("  Version: {0}", (const char*)glGetString(GL_VERSION));

// Setup debug output if supported
#ifdef RESHAPE_BUILD_DEBUG
    m_DebugContextEnabled = true;
    SetupDebugCallbacks();
#endif

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS); // Make sure depth function is explicitly set

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW); // Counter-clockwise winding for front faces
    glCullFace(GL_BACK); // Cull back faces

    return true;
}

void OpenGLContext::SwapBuffers() {
    glfwSwapBuffers(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
}

void OpenGLContext::MakeCurrent() {
    glfwMakeContextCurrent(static_cast<GLFWwindow*>(m_Window->GetNativeWindow()));
}

void* OpenGLContext::GetNativeContext() const {
    return glfwGetCurrentContext();
}

void OpenGLContext::SetupDebugCallbacks() {
    if (!m_DebugContextEnabled)
        return;

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLDebugCallback, nullptr);

    // Filter messages
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
}

void APIENTRY OpenGLContext::GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length,
                                             const GLchar* message, const void* userParam) {
    // Ignore non-significant error/warning codes
    if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
        return;

    const char* sourceStr;
    switch (source) {
    case GL_DEBUG_SOURCE_API:
        sourceStr = "API";
        break;
    case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
        sourceStr = "Window System";
        break;
    case GL_DEBUG_SOURCE_SHADER_COMPILER:
        sourceStr = "Shader Compiler";
        break;
    case GL_DEBUG_SOURCE_THIRD_PARTY:
        sourceStr = "Third Party";
        break;
    case GL_DEBUG_SOURCE_APPLICATION:
        sourceStr = "Application";
        break;
    case GL_DEBUG_SOURCE_OTHER:
        sourceStr = "Other";
        break;
    default:
        sourceStr = "Unknown";
        break;
    }

    switch (severity) {
    case GL_DEBUG_SEVERITY_HIGH:
        Log::Critical("[OpenGL] {0}: {1}", sourceStr, message);
        FORGE_ASSERT(false, "GL_DEBUG_SEVERITY_HIGH triggered");
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        Log::Error("[OpenGL] {0}: {1}", sourceStr, message);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        Log::Warn("[OpenGL] {0}: {1}", sourceStr, message);
        break;
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        Log::Trace("[OpenGL] {0}: {1}", sourceStr, message);
        break;
    }
}
} // namespace forge
