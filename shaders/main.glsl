#name default

#type vertex
#version 450 core
layout (location = 0) in vec3 aPosition;   // matches attribute 0
layout (location = 1) in vec2 aTexCoord;   // matches attribute 1

void main() {
    gl_Position = vec4(aPosition, 1.0);
}

#type fragment
#version 450 core

layout(location = 0) out vec4 FragColor;

void main() {
    // Output a solid blue color
    FragColor = vec4(0.0, 0.0, 1.0, 1.0);
}
