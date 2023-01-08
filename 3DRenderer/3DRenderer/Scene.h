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

	void SetProj(float* proj);
	void Render(float* proj);
private:
	std::vector <Mesh*> objects;
	std::vector <Entity*> entities;
};