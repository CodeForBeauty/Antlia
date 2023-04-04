#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

unsigned int CreateNewTexture(int width, int height,
	unsigned int format, unsigned int type,
	unsigned int filter = GL_NEAREST, unsigned int wrap = GL_CLAMP_TO_EDGE)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	return texture;
}

unsigned int CreateNewCubeMap(int width, int height,
	unsigned int format, unsigned int type,
	unsigned int filter = GL_NEAREST, unsigned int wrap = GL_CLAMP_TO_EDGE)
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
	for (int i = 0; i < 6; i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, type, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, wrap);
	return texture;
}