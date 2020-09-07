#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <iostream>

class Texture
{
public:
	unsigned int ID;
	int width, height, nChannels;

	Texture(const char* texturePath);

	void use();
};

#endif // !TEXTURE_H

