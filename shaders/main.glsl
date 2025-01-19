#name default

#type vertex
#version 450 core

layout(location = 0) in vec3 aPos;

void main() {
    gl_Position = vec4(aPos, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;

void main() {
    // Output a solid blue color
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
