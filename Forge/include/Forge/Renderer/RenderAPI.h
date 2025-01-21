// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef RENDERAPI_H
#define RENDERAPI_H

#include "Forge/Utils/Common.h"
#include "Forge/Utils/Math.h"
#include "Forge/Utils/Platform.h"

namespace forge {

struct ClearState {
    vec4f color{0.0f, 0.0f, 0.0f, 1.0f};
    float depth{1.0f};
    uint32_t stencil{0};
    bool clearColor{true};
    bool clearDepth{true};
    bool clearStencil{false};
};

struct Viewport {
    float x{0.0f};
    float y{0.0f};
    float width{0.0f};
    float height{0.0f};
    float minDepth{0.0f};
    float maxDepth{1.0f};
};

// NOTE: This is a singleton class because
// we want to have only one instance of the RenderAPI
class RenderAPI {
public:
    virtual ~RenderAPI() = default;

    virtual void Clear(const ClearState& state) = 0;
    //
    // NOTE: Other virtual functions define here
    //

    // Factory method
    static Shared<RenderAPI> Create();

    static GraphicsAPI GetAPI() {
        return PlatformAPI::GetSelectedGraphicsAPI();
    }

private:
    static Shared<RenderAPI> s_Instance;

protected:
    RenderAPI() = default;

    // NOTE: Delete copy operations
    RenderAPI(const RenderAPI&) = delete;
    RenderAPI& operator=(const RenderAPI&) = delete;

    // NOTE: Delete move operations
    RenderAPI(RenderAPI&&) noexcept = delete;
    RenderAPI& operator=(RenderAPI&&) noexcept = delete;
};
} // namespace forge

#endif
