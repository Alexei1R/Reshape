#type vertex

#version 450 core

layout(location = 0) in vec3 aPos;

layout(location = 0) out gl_PerVertex {
    vec4 gl_Position;
};

void main() {
    gl_Position = vec4(aPos, 1.0);
}

#type fragment

#version 450 core
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 1.0, 0.0, 1.0); // Red color
}
