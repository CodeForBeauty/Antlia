#include "Texture.h"
#include <stb/stb_image.h>
#include <gl/glew.h>

#include <iostream>

Texture::Texture(const char* path)
{
	stbi_set_flip_vertically_on_load(false);
	bytes = stbi_load(path, &width, &height, &channels, 4);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, bytes);


	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture::~Texture()
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glDeleteTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(bytes);
}

void Texture::Bind(int slt)
{
	slot = slt;
	glActiveTexture(GL_TEXTURE0 + slt);
	glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
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
