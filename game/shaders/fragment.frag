#version 330 core

in vec3 ourColor;
in vec2 TexCoord;
//in vec3 offsetColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

out vec4 FragColor;

void main()
{
	FragColor = mix(texture(texture1, TexCoord), texture(texture2, vec2(-TexCoord.x, TexCoord.y)), 0.2);
	//FragColor = vec4(ourColor, 1.0);
}