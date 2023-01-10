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
	delete preview;
	if (renderCamera)
		delete renderCamera;
}

void Scene::AddObject(Mesh* object)
{
	objects.push_back(object);
	if (!object->material)
	{
		materialGroup[0].push_back(object);
		object->material = materials[0];
	}
	int offset = 0;
	for (int i = 0; i < materialGroup[object->material->sceneSlot].size()-1; i++)
	{
		offset += materialGroup[object->material->sceneSlot][i]->geometry->verticiesCount;
	}
	for (int i = 0; i < object->geometry->indeciesCount; i++)
	{
		object->geometry->transformedIndecies[i] = object->geometry->indecies[i] + offset;
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
	glUseProgram(material->program);
	Vector3D camPos = preview->GetPosition();
	material->SetCamPos(camPos.x, camPos.y, camPos.z);
	material->SetView(preview->rotMetricies);
}

void Scene::SetObjectMaterial(Mesh* object, Material* material)
{
	materialGroup[object->material->sceneSlot].erase(std::remove_if(materialGroup[object->material->sceneSlot].begin(),
		materialGroup[object->material->sceneSlot].end(),
		[&](Mesh* i) { return i == object; }), materialGroup[object->material->sceneSlot].end());
	
	materialGroup[material->sceneSlot].push_back(object);
	object->material = material;

	int offset = 0;
	for (int i = 0; i < materialGroup[material->sceneSlot].size() - 1; i++)
	{
		offset += materialGroup[material->sceneSlot][i]->geometry->verticiesCount;
	}
	for (int i = 0; i < object->geometry->indeciesCount; i++)
	{
		object->geometry->transformedIndecies[i] = object->geometry->indecies[i] + offset;
	}

	
}


void Scene::Render(float* proj)
{
	for (int i = 0; i < materialGroup.size(); i++)
	{
		glUseProgram(materials[i]->program);
		materials[i]->SetProj(proj);

		if (preview->update)
		{
			Vector3D camPos = preview->GetPosition();
			materials[i]->SetCamPos(camPos.x, camPos.y, camPos.z);
			materials[i]->SetView(preview->rotMetricies);
		}

		std::vector <Vertex> batchVerticies;
		std::vector <unsigned int> batchIndecies;

		for (int j = 0; j < materialGroup[i].size(); j++)
		{
			batchVerticies.insert(batchVerticies.end(), materialGroup[i][j]->geometry->transformedVerticies, 
				materialGroup[i][j]->geometry->transformedVerticies + materialGroup[i][j]->geometry->verticiesCount);

			batchIndecies.insert(batchIndecies.end(), materialGroup[i][j]->geometry->transformedIndecies,
				materialGroup[i][j]->geometry->transformedIndecies + materialGroup[i][j]->geometry->indeciesCount);
		}

		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferSubData(GL_ARRAY_BUFFER, 0, batchVerticies.size() * sizeof(Vertex), batchVerticies.data());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, batchIndecies.size() * sizeof(unsigned int), batchIndecies.data());

		glDrawElements(GL_TRIANGLES, batchIndecies.size(), GL_UNSIGNED_INT, nullptr);
	}
	preview->update = false;
}

