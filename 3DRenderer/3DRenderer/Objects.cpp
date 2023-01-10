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
	delete verticies, indecies;
}


Mesh::Mesh(Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity()
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(Vector3D* position, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position)
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position, rotation)
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount)
	: Entity(position, rotation, scale)
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
}

Mesh::Mesh() : Entity() {}
Mesh::Mesh(Vector3D* position) : Entity(position) {}
Mesh::Mesh(Vector3D* position, Vector3D* rotation) : Entity(position, rotation) {}
Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Entity(position, rotation, scale) {}

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
		geometry->transformedVerticies[i].position = linmath::mulVector3dByVec3(*scale / (*scale + offset), linmath::subVector3dByVec3(*position, geometry->transformedVerticies[i].position));
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
	delete geometry;
	geometry = new Geometry(
		new Vertex[4]{
					 {{  0.5,  0.5, 0 }, { 1.0, 1.0 }},
					 {{ -0.5,  0.5, 0 }, { 0.0, 1.0 }},
					 {{ -0.5, -0.5, 0 }, { 0.0, 0.0 }},
					 {{  0.5, -0.5, 0 }, { 1.0, 0.0 }}
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
	delete geometry;
	geometry = new Geometry(
		new Vertex[8]{
				{{  0.5,  0.5, -0.5 }, { 1.0, 1.0 }},
				{{ -0.5,  0.5, -0.5 }, { 0.0, 1.0 }},
				{{ -0.5, -0.5, -0.5 }, { 0.0, 0.0 }},
				{{  0.5, -0.5, -0.5 }, { 1.0, 0.0 }},

				{{  0.5, -0.5,  0.5 }, { 1.0, 0.0 }},
				{{  0.5,  0.5,  0.5 }, { 1.0, 1.0 }},

				{{ -0.5,  0.5,  0.5 }, { 0.0, 1.0 }},
				{{ -0.5, -0.5,  0.5 }, { 0.0, 0.0 }}
		},
		new unsigned int[36] {
					2, 1, 0,
					0, 3, 2,

					0, 3, 4,
					4, 5, 0,

					1, 6, 7,
					7, 2, 1,

					0, 1, 6,
					6, 5, 0,

					3, 2, 7,
					7, 4, 3,

					7, 6, 5,
					5, 4, 7
			}, 8, 36
				);
}