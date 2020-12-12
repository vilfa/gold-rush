#version 420 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aOffset;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 vsout_fColor;

void main()
{
    vsout_fColor = aColor;
    gl_Position = projection * view * vec4(aPos + aOffset, 0.0, 1.0);
}