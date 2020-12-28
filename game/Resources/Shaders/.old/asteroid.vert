#version 420 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in mat4 aInstanceModelMat;

layout (std140, binding = 0) uniform aMatrices
{
    mat4 projection;
    mat4 view;
};

out vec2 vsout_TexCoords;

void main()
{
    vsout_TexCoords = aTexCoords;
    gl_Position = projection * view * aInstanceModelMat * vec4(aPos, 1.0);
}