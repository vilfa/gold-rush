#version 330 core

in vec3 TextureCoordinates;

uniform samplerCube skybox;

void main()
{
	gl_FragColor = texture(skybox, TextureCoordinates);
}