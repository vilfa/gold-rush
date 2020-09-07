#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

//uniform float offset;

out vec3 ourColor;
out vec2 TexCoord;
//out vec3 offsetColor;

void main()
{
	gl_Position = vec4(aPos, 1.0);
	ourColor = aColor;
	TexCoord = aTexCoord;
	//vec3 vOffset = vec3(-aPos.x + offset, -aPos.yz);
	//offsetColor = vOffset;
}