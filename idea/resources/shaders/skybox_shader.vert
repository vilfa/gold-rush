#version 330 core

layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

// We're displaying the skybox, so no model matrix needed.
uniform mat4 view;
uniform mat4 projection;

void main()
{
	TexCoords = aPos;
	vec4 glPos = projection * view * vec4(aPos, 1.0);
	gl_Position = glPos.xyww; // Make sure that the depth of the skybox is always 1.0 - maximum.
}