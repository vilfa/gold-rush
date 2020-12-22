#version 420 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out VS_OUT
{
    vec3 fragPos;
    vec3 fragNormal;
} vs_out;

uniform mat4 model;

void main()
{
    vs_out.fragNormal = aNormal;
    vs_out.fragPos = vec3(model * vec4(aPosition, 1.0));
	gl_Position = projection * view * model * vec4(aPosition, 1.0);
}