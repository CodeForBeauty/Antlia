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


Geometry::Geometry(float* vertices, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	:verticies(vertices), indecies(indecies)
	,verticiesCount(verticiesCount), indeciesCount(indeciesCount) {}

Geometry::~Geometry()
{
	delete verticies, indecies;
}


Mesh::Mesh(float* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity()
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
	createBuffers();
}

Mesh::Mesh(Vector3D* position, float* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position)
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
	createBuffers();
}

Mesh::Mesh(Vector3D* position, Vector3D* rotation, float* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount) 
	: Entity(position, rotation)
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
	createBuffers();
}

Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale, float* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount)
	: Entity(position, rotation, scale)
{
	delete geometry;
	geometry = new Geometry(verticies, indecies, verticiesCount, indeciesCount);
	createBuffers();
}

Mesh::Mesh() : Entity() {}
Mesh::Mesh(Vector3D* position) : Entity(position) {}
Mesh::Mesh(Vector3D* position, Vector3D* rotation) : Entity(position, rotation) {}
Mesh::Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Entity(position, rotation, scale) {}

Mesh::~Mesh()
{
	delete geometry, rotMetricies, material;
}

void Mesh::SetPosition(Vector3D& pos) const
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
	material->SetPos(pos.x, pos.y, pos.z);
}

void Mesh::Move(Vector3D& offset) const
{
	*position += offset;
	material->SetPos(position->x, position->y, position->z);
}

void Mesh::SetRotation(Vector3D& rot) const
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
	linmath::rotateMetricies(*rotation, rotMetricies);
	material->SetRot(rotMetricies);
}

void Mesh::Rotate(Vector3D& offset) const
{
	*rotation += offset;
	linmath::rotateMetricies(*rotation, rotMetricies);
	material->SetRot(rotMetricies);
}

void Mesh::SetScale(Vector3D& value) const
{
	scale->x = value.x;
	scale->y = value.y;
	scale->z = value.z;
	material->SetScale(scale->x, scale->y, scale->z);
}

void Mesh::AddScale(Vector3D& offset) const
{
	*scale += offset;
	material->SetScale(scale->x, scale->y, scale->z);
}

void Mesh::SetMaterial(Material* newMat)
{
	delete material;
	material = newMat;
}

void Mesh::Update(float* proj)
{
	material->SetPos(position->x, position->y, position->z);
	material->SetRot(rotMetricies);
	material->SetScale(scale->x, scale->y, scale->z);
	material->SetProj(proj);
}

void Mesh::createBuffers()
{
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * geometry->verticiesCount, geometry->verticies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(3 * sizeof(float)));

	glGenBuffers(1, &ibo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * geometry->indeciesCount, geometry->indecies, GL_STATIC_DRAW);
}


Plane::Plane() : Mesh() { setGeometry(); createBuffers(); }
Plane::Plane(Vector3D* position) : Mesh(position) { setGeometry(); createBuffers(); }
Plane::Plane(Vector3D* position, Vector3D* rotation) : Mesh(position, rotation) { setGeometry(); createBuffers(); }
Plane::Plane(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Mesh(position, rotation, scale) { setGeometry(); createBuffers(); }

void Plane::setGeometry()
{
	delete geometry;
	geometry = new Geometry(
		new float[20] {0.5,  0.5, 0, 1.0, 1.0,
					  -0.5,  0.5, 0, 0.0, 1.0,
					  -0.5, -0.5, 0, 0.0, 0.0,
					   0.5, -0.5, 0, 1.0, 0.0},
		new unsigned int[6] {0, 1, 2,
							 2, 3, 0}, 20, 6);
}


Cube::Cube() : Mesh() { setGeometry(); createBuffers(); }
Cube::Cube(Vector3D* position) : Mesh(position) { setGeometry(); createBuffers(); }
Cube::Cube(Vector3D* position, Vector3D* rotation) : Mesh(position, rotation) { setGeometry(); createBuffers(); }
Cube::Cube(Vector3D* position, Vector3D* rotation, Vector3D* scale) : Mesh(position, rotation, scale) { setGeometry(); createBuffers(); }

void Cube::setGeometry()
{
	delete geometry;
	geometry = new Geometry(
		new float[40] {
				 0.5,  0.5, -0.5, 1.0, 1.0,
				-0.5,  0.5, -0.5, 0.0, 1.0,
				-0.5, -0.5, -0.5, 0.0, 0.0,
				 0.5, -0.5, -0.5, 1.0, 0.0,

				 0.5, -0.5, 0.5, 1.0, 0.0,
				 0.5,  0.5, 0.5, 1.0, 1.0,

				-0.5,  0.5, 0.5, 0.0, 1.0,
				-0.5, -0.5, 0.5, 0.0, 0.0
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
			}, 40, 36
				);
}