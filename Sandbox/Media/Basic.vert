#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec2 TexCoord;

out vec2 f_TexCoord;

uniform mat4 u_ViewProjectionMatrix;
uniform mat4 u_Transform;

void main() {
    f_TexCoord  = TexCoord;
    gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(VertexPosition, 1.0);
}