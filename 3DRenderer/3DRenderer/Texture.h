#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	int GetWidth();
	int GetHeight();
	int GetChannels();
	unsigned int GetTexture();

private:
	int width;
	int height;
	int channels;
	unsigned char* bytes;

	unsigned int texture;

};