#version 330 core

vec3 I = vec3(1, 1, 1);          // point light intensity
vec3 Iamb = vec3(0.8, 0.8, 0.8); // ambient light intensity
vec3 kd = vec3(1, 0, 0);          // diffuse reflectance coefficient
vec3 ka = vec3(0.3, 0.3, 0.3);    // ambient reflectance coefficient
vec3 ks = vec3(0.8, 0.8, 0.8);    // specular reflectance coefficient
vec3 lightPos = vec3(5, 5, 5);    // light position in world coordinates

uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;
uniform vec3 eyePos;

layout(location=0) in vec3 inVertex;
layout(location=1) in vec3 inNormal;

out vec4 color;
out vec4 worldPosition;

void main(void)
{
    // Convert to world coordinates
    vec4 pWorld = modelingMatrix * vec4(inVertex, 1);
    vec3 nWorld = inverse(transpose(mat3x3(modelingMatrix))) * inNormal;

    // Compute lighting
    vec3 L = normalize(lightPos - vec3(pWorld));
    vec3 V = normalize(eyePos - vec3(pWorld));
    vec3 H = normalize(L + V);
    vec3 N = normalize(nWorld);

    float NdotL = dot(N, L); // for diffuse component
    float NdotH = dot(N, H); // for specular component

    vec3 diffuseColor = I * kd * max(0, NdotL);
    vec3 specularColor = I * ks * pow(max(0, NdotH), 100);
    vec3 ambientColor = Iamb * ka;

    color = vec4(diffuseColor + specularColor + ambientColor, 1);

    // Pass the world coordinates to the fragment shader
    worldPosition = modelingMatrix * vec4(inVertex, 1);

    // Transform the vertex with the product of the projection, viewing, and
    // modeling matrices.
    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * vec4(inVertex, 1);
}