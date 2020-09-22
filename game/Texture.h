#ifndef TEXTURE_H
#define TEXTURE_H

#include <iostream>

#include <stb/stb_image.h>
#include <glad/glad.h>

enum class TXenum {
	TEXTURE_JPG,
	TEXTURE_PNG,
};

class Texture
{
public:
	unsigned int ID;
	int Width;
	int Height;
	int N_Channels;

	Texture(const char* texturePath, TXenum type, bool flipVertical, GLenum textureWrapping, 
		GLenum mipmapFiltering);

	void Use(GLenum textureUnit) const;

private:
	static unsigned int _textureCount;
};

#endif // !TEXTURE_H

