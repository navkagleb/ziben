#type vertex
#version 460

layout (location = 0) in vec3  VertexPosition;
layout (location = 1) in vec4  Color;
layout (location = 2) in vec2  TexCoord;
layout (location = 3) in float TexIndex;
layout (location = 4) in float TilingFactor;

out vec4  v_Color;
out vec2  v_TexCoord;
out float v_TexIndex;
out float v_TilingFactor;

uniform mat4 u_ViewProjectionMatrix;

void main() {
    v_Color        = Color;
    v_TexCoord     = TexCoord;
    v_TexIndex     = TexIndex;
    v_TilingFactor = TilingFactor;
    gl_Position   = u_ViewProjectionMatrix * vec4(VertexPosition, 1.0);
}

#type fragment
#version 460

in vec4  v_Color;
in vec2  v_TexCoord;
in float v_TexIndex;
in float v_TilingFactor;

uniform sampler2D u_Textures[32];

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
}