#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexColor;

out vec3 FragVertexColor;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main() {
    FragVertexColor = VertexColor;
    gl_Position     = u_ViewProjectionMatrix * u_Transform * vec4(VertexPosition, 1.0);
}