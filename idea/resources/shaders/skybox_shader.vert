#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TextureCoordinates;

// We're displaying the skybox, so no model matrix needed.
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TextureCoordinates = aPos;
	gl_Position = projection * view * vec4(aPos, 1.0);
}