#version 420 core

in vec2 TexCoords;

uniform sampler2D texture_diffuse_1;
//uniform sampler2D texture_specular_1;
//uniform sampler2D texture_normal_1;

void main()
{
	gl_FragColor = texture(texture_diffuse_1, TexCoords);
}