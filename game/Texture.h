#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <stb/stb_image.h>
#include <glad/glad.h>

enum TXenum {
	TEXTURE_JPG = 0,
	TEXTURE_PNG = 1,
};

class Texture
{
public:
	unsigned int ID;
	int width, height, nChannels;

	Texture(const char* texturePath, TXenum type, bool flipVertical, GLenum textureWrapping, 
		GLenum mipmapFiltering);

	void use(GLenum textureUnit) const;

private:
	static unsigned int _textureCount;
};

#endif // !TEXTURE_H

