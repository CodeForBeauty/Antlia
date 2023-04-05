#pragma once

#include <vector>
#include "Objects.h"
#include "Lighting.h"
#include "Camera.h"
#include "Shader.h"


class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(Mesh* object);
	void AddLight(Light* light);
	void AddEntity(Entity* entity);
	void AddMaterial(Material* material);
	void SetObjectMaterial(Mesh* object, Material* material);

	void DeleteMaterial(Material* material);
	void DeleteLight(Light* light);
	void DeleteObject(Mesh* object);

	void Render(float* proj, int width, int height);

	Camera preview = Camera(45.0f, 0.5, 100, 1920, 1080);
	Camera renderCamera = Camera(45.0f, 0.1, 100, 1920, 1080);

	std::string savePath = "";

	std::vector <Mesh*> objects;
	std::vector <Light*> lights;
	std::vector <Entity*> entities;
	std::vector <Material*> materials;
private:
	std::vector <std::vector <Mesh*>> materialGroup;

	unsigned int vao, vbo, ibo;

	bool updateLight = true;
	unsigned int program;
	unsigned int directShadowProgram, cubeShadowProgram;

	unsigned int directShadowFBO, directShadowMap, cubeShadowFBO, cubeShadowMap;
	unsigned int shadowWidth = 2048, shadowHeight = 2048;

	unsigned int directShadowRendererFBO, directShadowRendererMap;
	

	Shader DirectShadowVert = Shader("shaders/ShadowMap.vert", GL_VERTEX_SHADER);
	Shader DirectShadowFrag = Shader("shaders/ShadowMap.frag", GL_FRAGMENT_SHADER);
	
	Shader CubeShadowVert = Shader("shaders/ShadowMap1.vert", GL_VERTEX_SHADER);
	Shader CubeShadowFrag = Shader("shaders/ShadowMap1.frag", GL_FRAGMENT_SHADER);
	Shader CubeShadowGeom = Shader("shaders/ShadowMap1.geom", GL_GEOMETRY_SHADER);
};