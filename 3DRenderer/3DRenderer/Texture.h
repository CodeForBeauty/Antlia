#pragma once

class Texture
{
public:
	Texture(const char* path);
	~Texture();

	void Bind(int slot = 0);
	void Unbind();

	int GetWidth();
	int GetHeight();
	int GetChannels();
	unsigned int GetTexture();

private:
	unsigned int slot = 0;
	int width;
	int height;
	int channels;
	unsigned char* bytes;

	unsigned int texture;

};