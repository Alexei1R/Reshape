// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef APPLICATION_H
#define APPLICATION_H

#include "Forge/Forge.hpp"
#include "Forge/Renderer/GraphicsContext.h"
#include "Forge/Utils/Common.h"

namespace reshape {

class Application {
public:
    Application();
    ~Application();

    void Run();

private:
    Shared<forge::Window> m_Window;
    Shared<forge::Shader> m_Shader;
    Shared<forge::RenderAPI> m_RenderAPI;
    Unique<forge::GraphicsContext> m_Context;

    unsigned int m_VAO{0};
    unsigned int m_VBO{0};
};

} // namespace reshape

#endif
