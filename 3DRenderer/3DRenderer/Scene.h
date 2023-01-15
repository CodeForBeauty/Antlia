#pragma once

#include <vector>
#include "Objects.h"
#include "Lighting.h"
#include "Camera.h"

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

	void Render(float* proj);

	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;

	Camera* preview = new Camera(90, 0.5, 1);
	Camera* renderCamera;

private:
	std::vector <Mesh*> objects;
	std::vector <Light*> lights;
	std::vector <Entity*> entities;
	std::vector <Material*> materials;
	std::vector <std::vector <Mesh*>> materialGroup;

	bool updateLight = false;
};