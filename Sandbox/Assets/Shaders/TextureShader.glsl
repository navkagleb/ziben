#type vertex
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

#type fragment
#version 460

in vec2 f_TexCoord;

uniform sampler2D u_Texture;
uniform vec4      u_Color;

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = texture(u_Texture, f_TexCoord * 10.0) * u_Color;
}