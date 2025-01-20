// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef OPENGLCONTEXT_H
#define OPENGLCONTEXT_H

#include "Forge/Renderer/GraphicsContext.h"
#include <glad/glad.h>

#include "GLFW/glfw3.h"
namespace forge {

class OpenGLContext final : public GraphicsContext {
public:
    explicit OpenGLContext(Shared<Window> window);
    ~OpenGLContext() override;

    bool Init() override;
    void SwapBuffers() override;
    void MakeCurrent() override;
    void* GetNativeContext() const override;

private:
    void SetupDebugCallbacks();
    static void APIENTRY GLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message,
                                         const void* userParam);

    Shared<Window> m_Window;
    bool m_DebugContextEnabled = false;
};

} // namespace forge

#endif
