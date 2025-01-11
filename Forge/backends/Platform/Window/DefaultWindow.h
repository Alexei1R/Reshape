
// Copyright (c) 2025-present, Rusu Alexei & Forge contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#ifndef DEFAULTWINDOW_H
#define DEFAULTWINDOW_H

#include <GLFW/glfw3.h>

#include "Forge/Renderer/Window.h"

namespace forge {

class DefaultWindow final : public Window {
public:
    explicit DefaultWindow(const WindowDescriptor& descriptor);
    ~DefaultWindow() override;

    void SetEventCallback(const EventCallbackFn& callback) override {}

    void* GetNativeWindow() const override {
        return m_Window;
    }
    void EnableVSync(bool enable) override;
    void Update() override;

    // ===============================

    inline uint32_t GetWidth() const override {
        return m_Data.width;
    }
    inline uint32_t GetHeight() const override {
        return m_Data.height;
    }
    inline bool IsVSyncEnabled() const override {
        return m_Data.vsyncEnabled;
    }
    inline bool IsFullscreen() const override {
        return m_Data.fullscreen;
    }

private:
    struct WindowData {
        std::string name;
        uint32_t width{};
        uint32_t height{};
        bool vsyncEnabled{false};
        bool fullscreen{false};
        EventCallbackFn eventCallback;
    };

    GLFWwindow* m_Window{nullptr};
    WindowData m_Data;
};

} // namespace forge

#endif // DEFAULTWINDOW_H
