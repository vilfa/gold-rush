#version 420 core

layout (location = 0) in vec3 aPos;

out VS_OUT
{
	vec3 texCoords;
} vs_out;

// We're displaying the skybox, so no model matrix needed.
uniform mat4 projection;
uniform mat4 view;

void main()
{
	vs_out.texCoords = aPos;
	gl_Position = (projection * view * vec4(aPos, 1.0)).xyww; // Make sure that the depth of the skybox is always 1.0 - maximum.
}