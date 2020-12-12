#version 420 core

layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 aColor;

layout (std140, binding = 0) uniform Matrices
{
    mat4 projection;
    mat4 view;
};

out vec3 vsout_fColor;

uniform vec2 offsets[100];

void main()
{
    vsout_fColor = aColor;
    vec2 offset = offsets[gl_InstanceID];
    gl_Position = projection * view * vec4(aPos + offset, 0.0, 1.0);
}