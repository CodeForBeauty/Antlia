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

void Entity::SetPosition(Vector3D& pos) const
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
}

void Entity::Move(Vector3D& offset) const
{
	*position += offset;
}

void Entity::SetRotation(Vector3D& rot) const
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
}

void Entity::Rotate(Vector3D& offset) const
{
	*rotation += offset;
}

void Entity::SetScale(Vector3D& value) const
{
	scale->x = value.x;
	scale->y = value.y;
	scale->z = value.z;
}

void Entity::AddScale(Vector3D& offset) const
{
	*scale += offset;
}

Vector3D* Entity::GetPosition() const
{
	return position;
}

Vector3D* Entity::GetRotation() const
{
	return rotation;
}

Vector3D* Entity::GetScale() const
{
	return scale;
}


Geometry::Geometry(Vertex* vertices, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	:verticies(vertices), indecies(indecies)
	,verticiesCount(verticiesCount), indeciesCount(indeciesCount) {}

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

void Mesh::SetPosition(Vector3D& pos) const
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
	
}

void Mesh::Move(Vector3D& offset) const
{
	*position += offset;
	
}

void Mesh::SetRotation(Vector3D& rot) const
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
	linmath::rotateMetricies(*rotation, rotMetricies);
	
}

void Mesh::Rotate(Vector3D& offset) const
{
	*rotation += offset;
	linmath::rotateMetricies(*rotation, rotMetricies);
	
}

void Mesh::SetScale(Vector3D& value) const
{
	scale->x = value.x;
	scale->y = value.y;
	scale->z = value.z;
	
}

void Mesh::AddScale(Vector3D& offset) const
{
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