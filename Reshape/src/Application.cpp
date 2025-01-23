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

    // Create shader
    m_Shader = forge::Shader::Create("shaders/main.glsl", forge::ShaderOrigin::File);
    m_Shader->Bind();

    // Create uniform buffers using GLAD directly
    glGenBuffers(1, &m_CameraUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(forge::math::mat4f), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_CameraUBO); // binding = 0

    glGenBuffers(1, &m_TransformUBO);
    glBindBuffer(GL_UNIFORM_BUFFER, m_TransformUBO);
    glBufferData(GL_UNIFORM_BUFFER, sizeof(forge::math::mat4f), nullptr, GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, m_TransformUBO); // binding = 1

    // Cube vertices with positions and colors
    struct Vertex {
        forge::math::vec3f position;
        forge::math::vec3f color;
    };

    Vertex vertices[] = {// Front face (red)
                         {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                         {{0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                         {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
                         {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},

                         // Right face (green)
                         {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
                         {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
                         {{0.5f, 0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},

                         // Back face (blue)
                         {{0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                         {{-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
                         {{0.5f, 0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},

                         // Left face (yellow)
                         {{-0.5f, -0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
                         {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 0.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {1.0f, 1.0f, 0.0f}},

                         // Top face (magenta)
                         {{-0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
                         {{0.5f, 0.5f, -0.5f}, {1.0f, 0.0f, 1.0f}},
                         {{0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},
                         {{-0.5f, 0.5f, 0.5f}, {1.0f, 0.0f, 1.0f}},

                         // Bottom face (cyan)
                         {{-0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
                         {{0.5f, -0.5f, 0.5f}, {0.0f, 1.0f, 1.0f}},
                         {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}},
                         {{-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f}}};

    uint32_t indices[] = {
        // Front face (facing towards positive Z)
        0, 1, 2,  2, 3, 0,
        // Right face
        4, 5, 6,  6, 7, 4,
        // Back face
        8, 9, 10, 10, 11, 8,
        // Left face
        12, 13, 14, 14, 15, 12,
        // Top face
        16, 17, 18, 18, 19, 16,
        // Bottom face
        20, 21, 22, 22, 23, 20
    };

    // Create and setup buffers
    m_VBO = forge::VertexBuffer::Create(vertices, sizeof(vertices));

    forge::BufferLayout layout = {{forge::BufferDataType::Float3, "a_Position"}, {forge::BufferDataType::Float3, "a_Color"}};
    m_VBO->SetLayout(layout);

    m_EBO = forge::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    m_VAO = forge::VertexArrayBuffer::Create();
    m_VAO->AddVertexBuffer(m_VBO);
    m_VAO->SetIndexBuffer(m_EBO);

    // Setup camera view and projection
    forge::math::mat4f view =
        forge::math::lookAt(forge::math::vec3f(0.0f, 0.0f, 3.0f),  // Camera position changed to positive Z
                           forge::math::vec3f(0.0f),                // Looking at origin
                           forge::math::vec3f(0.0f, 1.0f, 0.0f));  // Up vector

    forge::math::mat4f projection = forge::math::perspective(forge::math::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    m_ViewProjection = projection * view;

    // Update camera UBO
    glBindBuffer(GL_UNIFORM_BUFFER, m_CameraUBO);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(forge::math::mat4f), &m_ViewProjection);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
}

Application::~Application() {
    // Cleanup OpenGL UBOs
    glDeleteBuffers(1, &m_CameraUBO);
    glDeleteBuffers(1, &m_TransformUBO);
}

void Application::Run() {
    while (m_IsRunning) {
        PROFILE_SCOPE("Main Loop");

        forge::ClearState clearState;
        clearState.color = {0.1f, 0.1f, 0.1f, 1.0f};
        clearState.clearColor = true;
        clearState.clearDepth = true;
        m_RenderAPI->Clear(clearState);
        // Clear depth buffer at the beginning of each frame
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update transform matrix for rotation
        static float rotation = 0.0f;
        rotation += 0.001f;
        m_Transform = forge::math::rotate(forge::math::mat4f(1.0f), rotation, forge::math::vec3f(1.0f, 1.0f, 0.0f));

        // Update transform UBO
        glBindBuffer(GL_UNIFORM_BUFFER, m_TransformUBO);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(forge::math::mat4f), &m_Transform);

        // Bind shader
        m_Shader->Bind();

        // Draw cube
        m_VAO->Bind();
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        m_VAO->Unbind();

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
