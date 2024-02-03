#version 330 core

layout(location = 0) in vec3 inVertex;
layout(location = 1) in vec3 inNormal;

out vec3 fragNormal;

uniform mat4 modelingMatrix;
uniform mat4 viewingMatrix;
uniform mat4 projectionMatrix;

void main()
{
    gl_Position = projectionMatrix * viewingMatrix * modelingMatrix * vec4(inVertex, 1.0);
    fragNormal = mat3(transpose(inverse(modelingMatrix))) * inNormal;
}
