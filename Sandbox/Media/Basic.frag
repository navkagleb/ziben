#version 460

in vec2 f_TexCoord;

uniform sampler2D u_Texture;

layout (location = 0) out vec4 FragColor;

void main() {
    if (length(f_TexCoord) == 0.0) {
        FragColor = vec4(0.2, 0.3, 0.8, 1.0);
        return;
    }

    FragColor = texture(u_Texture, f_TexCoord);
}