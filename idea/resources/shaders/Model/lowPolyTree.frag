#version 420 core

uniform vec4 color_diffuse_1;
uniform vec4 color_specular_1;
uniform vec4 color_ambient_1;
uniform vec4 color_emissive_1;

in VS_OUT
{
    vec3 fragNormal;
} fs_in;

void main()
{
    gl_FragColor = color_diffuse_1 * color_specular_1 * color_ambient_1 * color_emissive_1;
}