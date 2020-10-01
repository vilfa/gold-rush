#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec3 FragmentPosition;
out vec3 FragmentNormal;
out vec2 TextureCoordinates;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	FragmentPosition = vec3(model * vec4(aPos, 1.0));
	FragmentNormal = mat3(transpose(inverse(model))) * aNormal;
	TextureCoordinates = aTexCoords;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}