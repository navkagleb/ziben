#version 460

in vec3 FragVertexColor;

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(FragVertexColor, 1.0);
}