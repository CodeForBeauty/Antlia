#pragma once

#include "Shader.h"
#include "Texture.h"

#define TEXTURE_ALBEDO 1
#define TEXTURE_METALIC 2
#define TEXTURE_SPECULAR 3
#define TEXTURE_ROUGHNESS 4
#define TEXTURE_NORMAL 5

class Material
{
public:
	Material();
	Material(Shader* vs, Shader* fs);
	Material(Shader* vs, Shader* fs, float* albedo);
	Material(Shader* vs, Shader* fs, float* albedo, float metalic);
	Material(float* albedo);
	~Material();

	void SetAlbedo(float r, float g, float b, float a);
	float* GetAlbedo();
	void SetMetalic(float value);
	float GetMetalic();
	void SetRoughness(float value);
	float GetRoughness();
	void SetSpecular(float value);
	float GetSpecular();

	void LoadTexture(const char* path, int slot = 0);
	void DeleteTexture(int slot);

	void Use();
	void Bind();

	void SetCamPos(float x, float y, float z);
	void SetView(float* value);
	void SetProj(float* value);
	unsigned int program;
	unsigned int sceneSlot;
private:
	void CompileShaders();
	Shader* vs;
	Shader* fs;
	Texture* texture;
	Texture* specTex;
	Texture* metalTex;
	Texture* roughTex;
	Texture* normal;
	float* albedo = new float[4] {1, 1, 1, 1};
	float metalic = 0.0;
	float roughness = 0.5;
	float specular = 0.5;

	int camPos;
	int view;
	int proj;
	int color;
};