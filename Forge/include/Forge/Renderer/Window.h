// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef WINDOW_H
#define WINDOW_H

#include <cstdint>
#include <functional>
#include <string>

#include "Forge/Utils/Common.h"

namespace forge {

// Forward declaration for Event class
class Event;

struct WindowDescriptor {
    uint32_t width;
    uint32_t height;
    std::string name;
    bool resizable;
    bool fullscreen;

    WindowDescriptor(uint32_t width = 1080, uint32_t height = 720, std::string name = "Reshape", bool resizable = true,
                     bool fullscreen = false)
        : width(width)
        , height(height)
        , name(std::move(name))
        , resizable(resizable)
        , fullscreen(fullscreen) {}
};

class Window {
public:
    using EventCallbackFn = std::function<void(Event&)>;
    virtual ~Window() = default;

    // Core window operations
    virtual void* GetNativeWindow() const = 0;
    virtual void EnableVSync(bool enable) = 0;
    virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
    virtual void Update() = 0;

    virtual uint32_t GetWidth() const = 0;
    virtual uint32_t GetHeight() const = 0;
    virtual bool IsVSyncEnabled() const = 0;
    virtual bool IsFullscreen() const = 0;

    // Factory method
    static Shared<Window> Create(const WindowDescriptor& descriptor = WindowDescriptor());

protected:
    Window() = default;

    // NOTE: Delete copy operations
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;

    // NOTE: Allow move operations
    Window(Window&&) noexcept = default;
    Window& operator=(Window&&) noexcept = default;
};
} // namespace forge

#endif
