// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef GRAPHICSCONTEXT_H
#define GRAPHICSCONTEXT_H

#include "Forge/Renderer/Window.h"
#include "Forge/Utils/Common.h"
namespace forge {

class Window;

class GraphicsContext {
public:
    virtual ~GraphicsContext() = default;

    virtual bool Init() = 0;
    virtual void SwapBuffers() = 0;
    virtual void MakeCurrent() = 0;
    virtual void* GetNativeContext() const = 0;

    static Unique<GraphicsContext> Create(Shared<Window> window);

protected:
};

} // namespace forge

#endif

