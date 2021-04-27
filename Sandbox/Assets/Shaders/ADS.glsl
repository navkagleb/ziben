#type vertex
#version 460

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;

out vec3 LightIntensity;

struct LightInfo {
    vec4 Position;   // Light position in eye coords
    vec3 La;         // Ambient light intensity
    vec3 Ld;         // Diffuse light intensity
    vec3 Ls;         // Specular light intensity
};

struct MaterialInfo {
    vec3  Ka;        // Ambient reflectivity
    vec3  Kd;        // Diffuse reflectivity
    vec3  Ks;        // Specular reflectivity
    float Shininess; // Specular shininess factor
};

uniform LightInfo    u_Light;
uniform MaterialInfo u_Material;

uniform mat4         u_ModelViewMatrix;
uniform mat3         u_NormalMatrix;
uniform mat4         u_ProjectionMatrix;
uniform mat4         u_MVP;

void main() {
    vec3  tnorm     = normalize(u_NormalMatrix * VertexNormal);
    vec4  eyeCoords = u_ModelViewMatrix * vec4(VertexPosition, 1.0);
    vec3  s         = normalize(vec3(u_Light.Position - eyeCoords));
    vec3  v         = normalize(-eyeCoords.xyz);
    vec3  r         = reflect(-s, tnorm);
    vec3  ambient   = u_Light.La * u_Material.Ka;
    float sDotN     = max(dot(s, tnorm), 0.0);
    vec3  diffuse   = u_Light.Ld * u_Material.Kd * sDotN;
    vec3  specular  = vec3(0.0);

    if (sDotN > 0.0)
        specular = u_Light.Ls * u_Material.Ks * pow(max(dot(r, v), 0.0), u_Material.Shininess);

    LightIntensity = ambient + diffuse + specular;
    gl_Position    = u_MVP * vec4(VertexPosition, 1.0);
}

#type fragment
#version 460

in vec3 LightIntensity;

layout (location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(LightIntensity, 1.0);
}