#type vertex
#version 460

layout (location = 0) in vec3  VertexPosition;
layout (location = 1) in vec4  Color;
layout (location = 2) in vec2  TexCoord;
layout (location = 3) in float TexIndex;
layout (location = 4) in float TilingFactor;
layout (location = 5) in int   EntityHandle;

out      vec4  v_Color;
out      vec2  v_TexCoord;
out flat float v_TexIndex;
out      float v_TilingFactor;
out flat int   v_EntityHandle;

uniform mat4 u_ViewProjectionMatrix;

void main() {
    v_Color        = Color;
    v_TexCoord     = TexCoord;
    v_TexIndex     = TexIndex;
    v_TilingFactor = TilingFactor;
    v_EntityHandle = EntityHandle;
    gl_Position   = u_ViewProjectionMatrix * vec4(VertexPosition, 1.0);
}

#type fragment
#version 460

in      vec4  v_Color;
in      vec2  v_TexCoord;
in      float v_TexIndex;
in      float v_TilingFactor;
in flat int   v_EntityHandle;

uniform sampler2D u_Textures[32];

layout (location = 0) out vec4 FragColor1;
layout (location = 1) out int  FragColor2;

void main() {
    FragColor1 = texture(u_Textures[int(v_TexIndex)], v_TexCoord * v_TilingFactor) * v_Color;
    FragColor2 = v_EntityHandle;
}