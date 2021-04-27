#type vertex
#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

uniform vec4 u_LightPosition;
uniform vec3 u_Kd;
uniform vec3 u_Ld;

uniform mat4 u_ModelViewMatrix;
uniform mat3 u_NormalMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_MVP;

void main() {
    vec3 tnorm     = normalize(u_NormalMatrix * VertexNormal);
    vec4 eyeCoords = u_ModelViewMatrix * vec4(VertexPosition, 1.0);
    vec3 s         = normalize(vec3(u_LightPosition - eyeCoords));

    LightIntensity = u_Ld * u_Kd * max(dot(s, tnorm), 0.0);

    gl_Position    = u_MVP * vec4(VertexPosition, 1.0);
}

#type fragment
#version 460

in vec3 LightIntensity;

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(LightIntensity, 1.0);
}