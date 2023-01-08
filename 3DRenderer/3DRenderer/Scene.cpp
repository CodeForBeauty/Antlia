#include "Scene.h"

Scene::Scene()
{

}

Scene::~Scene()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	for (int i = 0; i < entities.size(); i++)
		delete entities[i];
}

void Scene::AddObject(Mesh* object)
{
	objects.push_back(object);
}

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}

void Scene::SetProj(float* proj)
{
	for (int i = 0; i < objects.size(); i++)
	{
		glUseProgram(objects[i]->material->program);
		objects[i]->material->SetProj(proj);
	}
}

void Scene::Render(float* proj)
{
	for (int i = 0; i < objects.size(); i++)
	{
		glUseProgram(objects[i]->material->program);
		objects[i]->material->SetProj(proj);

		glBindVertexArray(objects[i]->vao);
		glDrawElements(GL_TRIANGLES, objects[i]->geometry->indeciesCount, GL_UNSIGNED_INT, nullptr);
	}
}

