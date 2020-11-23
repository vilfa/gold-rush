#version 330 core

in vec2 TextureCoordinates;

//out vec4 FragmentColor;

uniform sampler2D texture_diffuse_1;
uniform sampler2D texture_specular_1;
//uniform sampler2D texture_normal_1;

void main()
{
	gl_FragColor = 1.0 * texture(texture_diffuse_1, TextureCoordinates) * texture(texture_specular_1, TextureCoordinates);
}