#type vertex
#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec4 Color;
layout (location = 2) in vec2 TexCoord;

out vec4 v_Color;
out vec2 v_TexCoord;

uniform mat4 u_ViewProjectionMatrix;

void main() {
    v_Color     = Color;
    v_TexCoord  = TexCoord;
    gl_Position = u_ViewProjectionMatrix * vec4(VertexPosition, 1.0);
}

#type fragment
#version 460

in vec4 v_Color;
in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform float     u_TilingFactor;

layout (location = 0) out vec4 FragColor;

void main() {
    // FragColor = texture(u_Texture, f_TexCoord * u_TilingFactor) * v_Color;
    FragColor = v_Color;
}