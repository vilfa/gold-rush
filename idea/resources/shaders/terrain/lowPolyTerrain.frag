#version 420 core

in VS_OUT
{
    vec3 fragNormal;
    vec3 fragColor;
} fs_in;

void main()
{
    gl_FragColor = vec4(fs_in.fragColor, 1.0);
}