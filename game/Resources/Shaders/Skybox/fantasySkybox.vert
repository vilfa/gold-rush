#version 420 core

layout (location = 0) in vec3 aPos;

out VS_OUT
{
	vec3 texCoords;
} vs_out;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
    mat4 view3;
};

void main()
{
	vs_out.texCoords = aPos;
	gl_Position = (projection * view3 * vec4(aPos, 1.0)).xyww; // Make sure that the depth of the skybox is always 1.0 - maximum.
}