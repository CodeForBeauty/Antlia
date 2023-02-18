#include <GL/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include "Objects.h"
#include "linmath.h"

Entity::Entity()
{
	position = new Vector3D();
	rotation = new Vector3D();
	scale = new Vector3D(1, 1, 1);
}
Entity::Entity(Vector3D* position) : position(position)
{
	rotation = new Vector3D();
	scale = new Vector3D(1, 1, 1);
}
Entity::Entity(Vector3D* position, Vector3D* rotation) : position(position), rotation(rotation)
{
	scale = new Vector3D(1, 1, 1);
}
Entity::Entity(Vector3D* position, Vector3D* rotation, Vector3D* scale) : position(position), rotation(rotation), scale(scale) {}
Entity::~Entity()
{
	delete position, rotation, scale;
}

void Entity::SetPosition(const Vector3D& pos) const
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
}

void Entity::Move(const Vector3D& offset) const
{
	*position += offset;
}

void Entity::SetRotation(const Vector3D& rot) const
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
}

void Entity::Rotate(const Vector3D& offset) const
{
	*rotation += offset;
}

void Entity::SetScale(const Vector3D& value) const
{
	scale->x = value.x;
	scale->y = value.y;
	scale->z = value.z;
}

void Entity::AddScale(const Vector3D& offset) const
{
	*scale += offset;
}

Vector3D Entity::GetPosition() const
{
	return *position;
}

Vector3D Entity::GetRotation() const
{
	return *rotation;
}

Vector3D Entity::GetScale() const
{
	return *scale;
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
	//delete transformedVerticies;
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
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(Vector3D* position, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position)
{
	name = "mesh";
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position, rotation)
{
	name = "mesh";
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount)
	: Entity(position, rotation, scale)
{
	name = "mesh";
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh() : Entity() { name = "mesh"; }
Mesh::Mesh(Vector3D* position) : Entity(position) { name = "mesh"; }
Mesh::Mesh(Vector3D* position, Vector3D* rotation) : Entity(position, rotation) { name = "mesh"; }
Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Entity(position, rotation, scale) { name = "mesh"; }

Mesh::~Mesh()
{
	delete geometry, rotMetricies;
}

void Mesh::SetPosition(const Vector3D& pos) const
{
	for (int i = 0; i < geometry->verticiesCount; i++)
	{
		geometry->transformedVerticies[i].position = linmath::addVector3dByVec3(-(*position - pos), geometry->transformedVerticies[i].position);
	}
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
}

void Mesh::Move(const Vector3D& offset) const
{
	for (int i = 0; i < geometry->verticiesCount; i++)
	{
		geometry->transformedVerticies[i].position = linmath::addVector3dByVec3(offset, geometry->transformedVerticies[i].position);
	}
	*position += offset;
	
}

void Mesh::SetRotation(const Vector3D& rot) const
{
	Vector3D rotOffset = -(*rotation - rot);
	linmath::rotateMetricies(rotOffset, rotMetricies);
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
	for (int i = 0; i < geometry->verticiesCount; i++)
	{
		geometry->transformedVerticies[i].position = linmath::subVector3dByVec3(*position, geometry->transformedVerticies[i].position);
		geometry->transformedVerticies[i].position = linmath::multiplyByMetricies4x4(rotMetricies, geometry->transformedVerticies[i].position);
		geometry->transformedVerticies[i].normal = linmath::multiplyByMetricies4x4(rotMetricies, geometry->transformedVerticies[i].normal);
		geometry->transformedVerticies[i].position = linmath::addVector3dByVec3(*position, geometry->transformedVerticies[i].position);
	}
	
}

void Mesh::Rotate(const Vector3D& offset) const
{
	*rotation += offset;
	linmath::rotateMetricies(*rotation, rotMetricies);
	for (int i = 0; i < geometry->verticiesCount; i++)
	{
		geometry->transformedVerticies[i].position = linmath::multiplyByMetricies4x4(rotMetricies, geometry->verticies[i].position);
		geometry->transformedVerticies[i].normal = linmath::multiplyByMetricies4x4(rotMetricies, geometry->verticies[i].normal);
		geometry->transformedVerticies[i].position = linmath::addVector3dByVec3(*position, geometry->transformedVerticies[i].position);
	}
}

void Mesh::SetScale(const Vector3D& value) const
{
	for (int i = 0; i < geometry->verticiesCount; i++)
	{
		geometry->transformedVerticies[i].position = linmath::subVector3dByVec3(*position, geometry->transformedVerticies[i].position);
		geometry->transformedVerticies[i].position = linmath::mulVector3dByVec3(Vector3D(value.x / scale->x, value.y / scale->y, value.z / scale->z),
			geometry->transformedVerticies[i].position);
		geometry->transformedVerticies[i].position = linmath::addVector3dByVec3(*position, geometry->transformedVerticies[i].position);
	}
	scale->x = value.x;
	scale->y = value.y;
	scale->z = value.z;
}

void Mesh::AddScale(const Vector3D& offset) const
{
	for (int i = 0; i < geometry->verticiesCount; i++)
	{
		geometry->transformedVerticies[i].position = linmath::mulVector3dByVec3(*scale / (*scale + offset), 
			linmath::subVector3dByVec3(*position, geometry->transformedVerticies[i].position));
		geometry->transformedVerticies[i].position = linmath::addVector3dByVec3(*position, geometry->transformedVerticies[i].position);
	}
	*scale += offset;
}


Plane::Plane() : Mesh() { setGeometry(); }
Plane::Plane(Vector3D* position) : Mesh(position) { setGeometry(); }
Plane::Plane(Vector3D* position, Vector3D* rotation) : Mesh(position, rotation) { setGeometry(); }
Plane::Plane(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Mesh(position, rotation, scale) { setGeometry(); }

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
Cube::Cube(Vector3D* position) : Mesh(position) { setGeometry(); }
Cube::Cube(Vector3D* position, Vector3D* rotation) : Mesh(position, rotation) { setGeometry(); }
Cube::Cube(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Mesh(position, rotation, scale) { setGeometry(); }

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
Sphere::Sphere(Vector3D* position, int resolution) : Mesh(position) { setGeometry(resolution); }
Sphere::Sphere(Vector3D* position, Vector3D* rotation, int resolution) : Mesh(position, rotation) { setGeometry(resolution); }
Sphere::Sphere(Vector3D* position, Vector3D* rotation, Vector3D* scale, int resolution) : Mesh(position, rotation, scale) { setGeometry(resolution); }

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