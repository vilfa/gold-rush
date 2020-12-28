#version 420 core

in VS_OUT
{
	vec3 texCoords;
} fs_in;

uniform samplerCube skybox;

void main()
{
	gl_FragColor = texture(skybox, fs_in.texCoords);
}