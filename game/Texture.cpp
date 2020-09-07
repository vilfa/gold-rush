#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include "Texture.h"

Texture::Texture(const char* texturePath)
{
	glGenTextures(1, &ID);
	glBindTexture(GL_TEXTURE_2D, ID);

	/*
	* Set the wrapping and filtering options (on currently bound texture object).
	*/
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	/*
	* Load and generate texture.
	*/
	unsigned char* data = stbi_load(texturePath, &width, &height, &nChannels, 0);
	if (data)
	{
		/*
		* Arguemnts:
		* 1. texture target (only GL_TEXTURE_2D is affected)
		* 2. mipmap level to create texture for (0 = base)
		* 3. the texture format
		* 4. and 5. width, height
		* 6. always 0 (legacy)
		* 7. and 8. format and datatype of original image
		* (in this case: loaded as RGB and stored as char - bytes)
		* 9. actual image data
		*/
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0,
			GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "ERROR::TEXTURE::FILE_READ_ERROR" << std::endl;
	}

	stbi_image_free(data);
}

void Texture::use()
{
	glBindTexture(GL_TEXTURE_2D, ID);
}
