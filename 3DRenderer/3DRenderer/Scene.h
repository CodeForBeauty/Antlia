#pragma once

#include <vector>
#include "Objects.h"

class Scene
{
public:
	Scene();
	~Scene();

	void AddObject(Mesh* object);
	void AddEntity(Entity* entity);
	void AddMaterial(Material* material);
	void SetObjectMaterial(Mesh* object, Material* material);

	void Render(float* proj);

	unsigned int vao;
	unsigned int vbo;
	unsigned int ibo;

private:
	std::vector <Mesh*> objects;
	std::vector <Entity*> entities;
	std::vector <Material*> materials;
	std::vector <std::vector <Mesh*>> materialGroup;
};