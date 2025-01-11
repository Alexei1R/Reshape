// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
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

private:
    Shared<forge::Window> m_Window;
    unsigned int m_VAO{0};
    unsigned int m_VBO{0};
};

} // namespace reshape

#endif
