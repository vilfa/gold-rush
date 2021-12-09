#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

layout (std140, binding = 0) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

out VS_OUT
{
	vec3 fragPos;
	vec3 fragNormal;
	vec2 texCoords;
} vs_out;

uniform mat4 model;

void main()
{
	vs_out.fragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.fragNormal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.texCoords = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}