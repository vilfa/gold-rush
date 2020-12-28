#version 330 core

in vec2 TexCoords;
in vec3 Normal;
in vec3 Position;

//uniform sampler2D texture_diffuse_1;
//uniform sampler2D texture_specular_1;
//uniform sampler2D texture_normal_1;

uniform vec3 cameraPos;
uniform samplerCube skybox;

void main()
{
	float refractionRatio = 1.00 / 1.52; // The light ray goes from air to glass (assuming the backpack is made of glass).
	vec3 I = normalize(Position - cameraPos);
	vec3 R = refract(I, normalize(Normal), refractionRatio);
//	gl_FragColor = 1.0 * texture(texture_diffuse_1, TexCoords) * texture(texture_specular_1, TexCoords) * texture(skybox, R);
	gl_FragColor = texture(skybox, R);
}