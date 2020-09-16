#define STB_IMAGE_IMPLEMENTATION

#include <stb/stb_image.h>

#include "Texture.h"

unsigned int Texture::_textureCount = 0;

Texture::Texture(const char* texturePath, TXenum type, bool flipVertical, GLenum textureWrapping,
	GLenum mipmapFiltering)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	/*
	* Set the wrapping and filtering options (on currently bound texture object).
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapping);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mipmapFiltering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mipmapFiltering);

	/*
	* Load and generate texture.
	*/
	if (flipVertical)
	{
		stbi_set_flip_vertically_on_load(true);
	}

	unsigned char* data = stbi_load(texturePath, &width, &height, &nChannels, 0);
	if (data)
	{
		/*
		* Arguments:
		* 1. texture target (only GL_TEXTURE_2D is affected)
		* 2. mipmap level to create texture for (0 = base)
		* 3. the texture format
		* 4. and 5. width, height
		* 6. always 0 (legacy)
		* 7. and 8. format and datatype of original image
		* (in this case: loaded as RGB and stored as char - bytes)
		* 9. actual image data
		*/
		switch (type)
		{
		case TEXTURE_JPG:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGB, GL_UNSIGNED_BYTE, data);
			break;
		case TEXTURE_PNG:
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
				GL_RGBA, GL_UNSIGNED_BYTE, data);
			break;
		default:
			break;
		}
		
		glGenerateMipmap(GL_TEXTURE_2D);

		textureCount++;
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FILE_READ_ERROR" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::use(GLenum textureUnit) const
{
	glActiveTexture(textureUnit);
	glBindTexture(GL_TEXTURE_2D, ID);
}
