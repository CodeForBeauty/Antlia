#include "Scene.h"
#include <array>

Scene::Scene()
{
	materials.push_back(new Material());
	materialGroup.push_back({});

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 1000, nullptr, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, color));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 4000, nullptr, GL_DYNAMIC_DRAW);
}

Scene::~Scene()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];
	for (int i = 0; i < entities.size(); i++)
		delete entities[i];
	for (int i = 0; i < materials.size(); i++)
		delete materials[i];
}

void Scene::AddObject(Mesh* object)
{
	objects.push_back(object);
	if (!object->material)
	{
		materialGroup[0].push_back(object);
		object->material = materials[0];
	}
}

void Scene::AddEntity(Entity* entity)
{
	entities.push_back(entity);
}

void Scene::AddMaterial(Material* material)
{
	materials.push_back(material);
	materialGroup.push_back({});
	material->sceneSlot = materialGroup.size()-1;
}

void Scene::SetObjectMaterial(Mesh* object, Material* material)
{
	materialGroup[material->sceneSlot].push_back(object);
	object->material = material;
}


void Scene::Render(float* proj)
{
	glBindVertexArray(vao);

	for (int i = 0; i < materialGroup.size(); i++)
	{
		glUseProgram(materials[i]->program);
		materials[i]->SetProj(proj);

		std::vector <Vertex> batchVerticies;
		std::vector <unsigned int> batchIndecies;

		for (int j = 0; j < materialGroup[i].size(); j++)
		{
			batchVerticies.insert(batchVerticies.end(), materialGroup[i][j]->geometry->verticies, materialGroup[i][j]->geometry->verticies + materialGroup[i][j]->geometry->verticiesCount);
			batchIndecies.insert(batchIndecies.end(), materialGroup[i][j]->geometry->indecies, materialGroup[i][j]->geometry->indecies + materialGroup[i][j]->geometry->indeciesCount);
		}

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batchVerticies.size() * sizeof(Vertex), batchVerticies.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batchIndecies.size() * sizeof(unsigned int), batchIndecies.data());
		glBindVertexArray(vao);

		glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);
	}
}

