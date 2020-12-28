#version 420 core

in vec3 vsout_fColor;

void main()
{
    gl_FragColor = vec4(vsout_fColor, 1.0);
}