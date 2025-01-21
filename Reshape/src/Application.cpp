// Copyright (c) 2025-present, Rusu Alexei & Project contributors.
// Distributed under the MIT License (http://opensource.org/licenses/MIT)

#include "Application.h"

namespace reshape {

Application::Application() {
    forge::Log::Info("Application constructor");
    m_Window = forge::Window::Create();
    m_Window->SetEventCallback(std::bind(&Application::HandleEvent, this, std::placeholders::_1));

    m_Context = forge::GraphicsContext::Create(m_Window);
    m_RenderAPI = forge::RenderAPI::Create();

    m_Shader = forge::Shader::Create("shaders/main.glsl", forge::ShaderOrigin::File);
    m_Shader->Bind();
}

Application::~Application() {}

void Application::Run() {
    while (m_IsRunning) {
        PROFILE_SCOPE("Main Loop");

        forge::ClearState clearState;
        clearState.color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearState.clearColor = true;
        clearState.clearDepth = true;
        m_RenderAPI->Clear(clearState);

        /*m_Shader->Bind();*/

        m_Context->SwapBuffers();
        m_Window->Update();
    }
}

void Application::HandleEvent(const forge::Event& event) {
    if (event.GetType() == forge::EventType::Window) {
        forge::WindowEvent windowEvent = static_cast<const forge::WindowEvent&>(event);
        if (windowEvent.GetAction() == forge::Action::Close) {
            forge::Log::Trace("Window Closed Event");
            m_IsRunning = false;
        }
    }
};
} // namespace reshape
