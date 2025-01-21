// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "OpenGLRenderAPI.h"
#include <glad/glad.h>

namespace forge {

void OpenGLRenderAPI::Clear(const ClearState& state) {
    GLbitfield mask = 0;

    if (state.clearColor) {
        mask |= GL_COLOR_BUFFER_BIT;
        glClearColor(state.color.r, state.color.g, state.color.b, state.color.a);
    }

    if (state.clearDepth) {
        mask |= GL_DEPTH_BUFFER_BIT;
        glClearDepth(state.depth);
    }

    if (state.clearStencil) {
        mask |= GL_STENCIL_BUFFER_BIT;
        glClearStencil(state.stencil);
    }

    glClear(mask);
}

} // namespace forge
