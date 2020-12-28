#version 420 core

in vec2 vsout_TexCoords;

uniform sampler2D texture_diffuse_1;

void main()
{
    gl_FragColor = texture(texture_diffuse_1, vsout_TexCoords);
}