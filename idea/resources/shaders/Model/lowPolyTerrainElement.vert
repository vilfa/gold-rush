#version 420 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 3) in mat4 aModel;

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

void main()
{
	vs_out.fragNormal = aNormal;
    vs_out.fragPos = vec3(aModel * vec4(aPosition, 1.0));
	gl_Position = projection * view * aModel * vec4(aPosition, 1.0);
}