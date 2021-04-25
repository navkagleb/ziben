#version 460

in vec3 FragVertexColor;

uniform vec3 u_Color;

layout (location = 0) out vec4 FragColor;

void main() {
    if (length(FragVertexColor) != 0) {
        FragColor = vec4(FragVertexColor, 1.0);
        return;
    }

    FragColor = vec4(u_Color, 1.0);
}