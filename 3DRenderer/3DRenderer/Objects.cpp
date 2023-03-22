#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Objects.h"
#include "linmath.h"

Entity::Entity() {}
Entity::Entity(ln::vec3 position) 
	: position(position) {}
Entity::Entity(ln::vec3 position, ln::vec3 rotation) 
	: position(position), rotation(rotation) {}
Entity::Entity(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale) 
	: position(position), rotation(rotation), scale(scale) {}
Entity::~Entity()
{
	delete position, rotation, scale;
}

void Entity::SetPosition(ln::vec3 pos)
{
	position = pos;
}

void Entity::Move(ln::vec3 offset)
{
	position += offset;
}

void Entity::SetRotation(ln::vec3 rot)
{
	rotation = rot;
}

void Entity::Rotate(ln::vec3 offset)
{
	rotation += offset;
}

void Entity::SetScale(ln::vec3 value)
{
	scale = value;
}

void Entity::AddScale(ln::vec3 offset)
{
	scale += offset;
}

ln::vec3 Entity::GetPosition() const
{
	return position;
}

ln::vec3 Entity::GetRotation() const
{
	return rotation;
}

ln::vec3 Entity::GetScale() const
{
	return scale;
}

void Entity::SetName(std::string newName)
{
	name = newName;
}
std::string Entity::GetName() const
{
	return name;
}

Geometry::Geometry(Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount)
	:verticies(verticies), indecies(indecies)
	,verticiesCount(verticiesCount), indeciesCount(indeciesCount) 
{
	transformedVerticies = (Vertex*)malloc(sizeof(Vertex) * verticiesCount);
	memcpy(transformedVerticies, verticies, sizeof(Vertex) * verticiesCount);
	transformedIndecies = (unsigned int*)malloc(sizeof(unsigned int) * indeciesCount);
	memcpy(transformedIndecies, indecies, sizeof(unsigned int) * indeciesCount);
}

Geometry::~Geometry()
{
	delete indecies, verticies, transformedVerticies, transformedIndecies;
}


Mesh::Mesh(Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity()
{
	name = "mesh";
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(ln::vec3 position, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position)
{
	name = "mesh";
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(ln::vec3 position, ln::vec3 rotation, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount)
	: Entity(position, rotation)
{
	name = "mesh";
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount)
	: Entity(position, rotation, scale)
{
	name = "mesh";
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh() : Entity() 
{
	name = "mesh";
	geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
}
Mesh::Mesh(ln::vec3 position) : Entity(position) 
{
	name = "mesh";
	geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
}
Mesh::Mesh(ln::vec3 position, ln::vec3 rotation) : Entity(position, rotation) 
{
	name = "mesh";
	geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
}
Mesh::Mesh(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale) : Entity(position, rotation, scale) 
{
	name = "mesh";
	geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
}

Mesh::~Mesh()
{
	delete geometry;
}

void Mesh::SetPosition(ln::vec3 pos)
{
	for (int i = 0; i < geometry->verticiesCount; i++)
		geometry->transformedVerticies[i].position = pos - position + geometry->transformedVerticies[i].position;
	position = pos;
}

void Mesh::Move(ln::vec3 offset)
{
	for (int i = 0; i < geometry->verticiesCount; i++)
		geometry->transformedVerticies[i].position = offset + geometry->transformedVerticies[i].position;
	position += offset;
	
}

void Mesh::SetRotation(ln::vec3 rot)
{
	transformMatrix = ln::transform(position, rot);
	rotation = rot;
	for (int i = 0; i < geometry->verticiesCount; i++)
		geometry->transformedVerticies[i].position = ln::cutVec3(transformMatrix * geometry->verticies[i].position);
}

void Mesh::Rotate(ln::vec3 offset)
{
	rotation += offset;
	transformMatrix = ln::transform(position, rotation);
	for (int i = 0; i < geometry->verticiesCount; i++)
		geometry->transformedVerticies[i].position = ln::cutVec3(transformMatrix * geometry->verticies[i].position);
}

void Mesh::SetScale(ln::vec3 value)
{
	for (int i = 0; i < geometry->verticiesCount; i++)
		geometry->transformedVerticies[i].position = (geometry->transformedVerticies[i].position - position) * (scale / value) + position;
	scale = value;
}

void Mesh::AddScale(ln::vec3 offset)
{
	for (int i = 0; i < geometry->verticiesCount; i++)
		geometry->transformedVerticies[i].position = (geometry->transformedVerticies[i].position - position) * (scale / (scale + offset)) + position;
	scale += offset;
}


Plane::Plane() : Mesh() { setGeometry(); }
Plane::Plane(ln::vec3 position) : Mesh(position) { setGeometry(); }
Plane::Plane(ln::vec3 position, ln::vec3 rotation) : Mesh(position, rotation) { setGeometry(); }
Plane::Plane(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale) : Mesh(position, rotation, scale) { setGeometry(); }

void Plane::setGeometry()
{
	name = "plane";
	delete geometry;
	geometry = new Geometry(
		new Vertex[4]{
					 {{  0.5,  0.5, 0 }, {0.0, 0.0, -1.0}, {1.0, 1.0}},
					 {{ -0.5,  0.5, 0 }, {0.0, 0.0, -1.0}, {0.0, 1.0}},
					 {{ -0.5, -0.5, 0 }, {0.0, 0.0, -1.0}, {0.0, 0.0}},
					 {{  0.5, -0.5, 0 }, {0.0, 0.0, -1.0}, {1.0, 0.0}}
		},
		new unsigned int[6] {0, 1, 2,
							 2, 3, 0}, 4, 6);
}


Cube::Cube() : Mesh() { setGeometry(); }
Cube::Cube(ln::vec3 position) : Mesh(position) { setGeometry(); }
Cube::Cube(ln::vec3 position, ln::vec3 rotation) : Mesh(position, rotation) { setGeometry(); }
Cube::Cube(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale) : Mesh(position, rotation, scale) { setGeometry(); }

void Cube::setGeometry()
{
	name = "cube";
	delete geometry;
	geometry = new Geometry(
		new Vertex[24]{
				{{  0.5,  0.5, -0.5 }, { 0.0,  0.0, -1.0 }, { 1.0, 1.0 }},
				{{ -0.5,  0.5, -0.5 }, { 0.0,  0.0, -1.0 }, { 0.0, 1.0 }},
				{{ -0.5, -0.5, -0.5 }, { 0.0,  0.0, -1.0 }, { 0.0, 0.0 }},
				{{  0.5, -0.5, -0.5 }, { 0.0,  0.0, -1.0 }, { 1.0, 0.0 }},

				{{  0.5, -0.5,  0.5 }, { 0.0,  0.0,  1.0 }, { 1.0, 0.0 }},
				{{  0.5,  0.5,  0.5 }, { 0.0,  0.0,  1.0 }, { 1.0, 1.0 }},
				{{ -0.5,  0.5,  0.5 }, { 0.0,  0.0,  1.0 }, { 0.0, 1.0 }},
				{{ -0.5, -0.5,  0.5 }, { 0.0,  0.0,  1.0 }, { 0.0, 0.0 }},

				{{  0.5,  0.5,  0.5 }, { 1.0,  0.0,  0.0 }, { 1.0, 1.0 }},
				{{  0.5, -0.5,  0.5 }, { 1.0,  0.0,  0.0 }, { 0.0, 1.0 }},
				{{  0.5, -0.5, -0.5 }, { 1.0,  0.0,  0.0 }, { 0.0, 0.0 }},
				{{  0.5,  0.5, -0.5 }, { 1.0,  0.0,  0.0 }, { 1.0, 0.0 }},

				{{ -0.5, -0.5,  0.5 }, {-1.0,  0.0,  0.0 }, { 0.0, 1.0 }},
				{{ -0.5,  0.5,  0.5 }, {-1.0,  0.0,  0.0 }, { 1.0, 1.0 }},
				{{ -0.5,  0.5, -0.5 }, {-1.0,  0.0,  0.0 }, { 1.0, 0.0 }},
				{{ -0.5, -0.5, -0.5 }, {-1.0,  0.0,  0.0 }, { 0.0, 0.0 }},

				{{ -0.5,  0.5,  0.5 }, { 0.0,  1.0,  0.0 }, { 0.0, 1.0 }},
				{{  0.5,  0.5,  0.5 }, { 0.0,  1.0,  0.0 }, { 1.0, 1.0 }},
				{{  0.5,  0.5, -0.5 }, { 0.0,  1.0,  0.0 }, { 1.0, 0.0 }},
				{{ -0.5,  0.5, -0.5 }, { 0.0,  1.0,  0.0 }, { 0.0, 0.0 }},

				{{ -0.5, -0.5,  0.5 }, { 0.0, -1.0,  0.0 }, { 0.0, 1.0 }},
				{{  0.5, -0.5,  0.5 }, { 0.0, -1.0,  0.0 }, { 1.0, 1.0 }},
				{{  0.5, -0.5, -0.5 }, { 0.0, -1.0,  0.0 }, { 1.0, 0.0 }},
				{{ -0.5, -0.5, -0.5 }, { 0.0, -1.0,  0.0 }, { 0.0, 0.0 }},
		},
		new unsigned int[36] {
								0, 1, 2,
								2, 3, 0,

								6, 5, 4,
								4, 7, 6,

								10, 9, 8,
								8, 11, 10,

								14, 13, 12,
								12, 15, 14,

								18, 17, 16,
								16, 19, 18,

								20, 21, 22,
								22, 23, 20
		}, 24, 36
	);
}


Sphere::Sphere(int resolution) : Mesh() { setGeometry(resolution); }
Sphere::Sphere(ln::vec3 position, int resolution) : Mesh(position) { setGeometry(resolution); }
Sphere::Sphere(ln::vec3 position, ln::vec3 rotation, int resolution) : Mesh(position, rotation) { setGeometry(resolution); }
Sphere::Sphere(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale, int resolution) : Mesh(position, rotation, scale) { setGeometry(resolution); }

void Sphere::setGeometry(int resolution)
{
	name = "sphere";
	delete geometry;
	Vertex* verticies = new Vertex[(resolution) * (resolution) * 2];
	unsigned int* indecies = new unsigned int[(resolution) * (resolution) * 2 * 6];

	float half = ((float)resolution-1) / 2.0;

	for (int z = 0; z < resolution; z++)
	{
		for (int x = 0; x < resolution; x++)
		{
			float i = ((float)x / half) - 1;
			
			float k = ((float)z / half) - 1;
			verticies[(z * resolution + x)].uv = { i, k };
			float j = std::sqrt(1 - (i * i + k * k));
			if (isnan(j)) j = 0;
			
			if (i >= 0)
				i = ( std::sqrt(1.01 - (j * j + k * k)));
			else
				i = (-std::sqrt(1.01 - (j * j + k * k)));
			
			j = std::sqrt(1 - (i * i + k * k));
			if (isnan(j)) j = 0;
			
			verticies[(z * resolution + x)].position = { i, j, k };
			verticies[(z * resolution + x)].normal = { i, j, k };
		}
	}
	for (int z = 0; z < resolution; z++)
	{
		for (int x = 0; x < resolution; x++)
		{
			float i = ((float)x / half) - 1;

			float k = ((float)z / half) - 1;
			verticies[(z * resolution + x) + (resolution * resolution)].uv = {i, k};
			float j = std::sqrt(1 - (i * i + k * k));
			if (isnan(j)) j = 0;

			if (i >= 0)
				i = (std::sqrt(1.01 - (j * j + k * k)));
			else
				i = (-std::sqrt(1.01 - (j * j + k * k)));

			j = -std::sqrt(1 - (i * i + k * k));
			if (isnan(j)) j = 0;

			verticies[(z * resolution + x) + (resolution * resolution)].position = {i, j, k};
			verticies[(z * resolution + x) + (resolution * resolution)].normal = { i, j, k };
		}
	}

	for (int i = 0; i < (resolution-1); i++)
	{
		for (int j = 0; j < (resolution-1); j++)
		{
			int index = (i * resolution + j);
			int vertIndex = (i * (resolution) + j);
			indecies[index * 6] = vertIndex;
			indecies[index * 6 + 1] = vertIndex + 1;
			indecies[index * 6 + 2] = vertIndex + resolution;

			indecies[index * 6 + 5] = vertIndex + resolution;
			indecies[index * 6 + 4] = vertIndex + resolution + 1;
			indecies[index * 6 + 3] = vertIndex + 1;
		}
	}
	for (int i = 0; i < (resolution - 1); i++)
	{
		for (int j = 0; j < (resolution - 1); j++)
		{
			int index = (i * resolution + j)+(resolution)*(resolution);
			int vertIndex = (i * (resolution)+j) + (resolution) * (resolution);
			indecies[index * 6 + 2] = vertIndex;
			indecies[index * 6 + 1] = vertIndex + 1;
			indecies[index * 6] = vertIndex + resolution;

			indecies[index * 6 + 3] = vertIndex + resolution;
			indecies[index * 6 + 4] = vertIndex + resolution + 1;
			indecies[index * 6 + 5] = vertIndex + 1;
		}
	}

	geometry = new Geometry(
		verticies,
		indecies, resolution * resolution * 2, (resolution) * (resolution) * 2 * 6
	);
}