// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef OPENGLRENDERAPI_H
#define OPENGLRENDERAPI_H

#include "Forge/Renderer/RenderAPI.h"

namespace forge {

class OpenGLRenderAPI final : public RenderAPI {
public:
    OpenGLRenderAPI() = default;
    ~OpenGLRenderAPI() override = default;

    void Clear(const ClearState& state) override;
};

} // namespace forge
#endif
