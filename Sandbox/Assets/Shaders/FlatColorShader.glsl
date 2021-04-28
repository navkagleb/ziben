// Flat color shader

#type vertex
#version 460

layout (location = 0) in vec3 VertexPosition;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main() {
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(VertexPosition, 1.0);
}

#type fragment
#version 460

uniform vec4 u_Color;

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = u_Color;
}