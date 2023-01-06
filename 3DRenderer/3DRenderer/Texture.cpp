#include "Texture.h"
#include <stb/stb_image.h>
#include <gl/glew.h>

Texture::Texture(const char* path)
{
	bytes = stbi_load(path, &width, &height, &channels, 0);

	glGenTextures(1, &texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);
}

Texture::~Texture()
{
	stbi_image_free(bytes);
	glBindTexture(GL_TEXTURE_2D, 0);

	delete bytes;
}

int Texture::GetWidth()
{
	return width;
}

int Texture::GetHeight()
{
	return height;
}

int Texture::GetChannels()
{
	return channels;
}

unsigned int Texture::GetTexture()
{
	return texture;
}
