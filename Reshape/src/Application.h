// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef APPLICATION_H
#define APPLICATION_H

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
};

} // namespace reshape

#endif
