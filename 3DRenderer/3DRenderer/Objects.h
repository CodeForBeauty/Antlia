#pragma once

#include "Metrics.h"
#include "Material.h"
#include "linmath.h"

class Entity
{
public:
	Entity();
	Entity(ln::vec3 position);
	Entity(ln::vec3 position, ln::vec3 rotation);
	Entity(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale);
	~Entity();

	virtual void SetPosition(ln::vec3 position);
	virtual void Move(ln::vec3 offset);
	virtual void SetRotation(ln::vec3 rotation);
	virtual void Rotate(ln::vec3 offset);
	virtual void SetScale(ln::vec3 scale);
	virtual void AddScale(ln::vec3 offset);

	ln::vec3 GetPosition() const;
	ln::vec3 GetRotation() const;
	ln::vec3 GetScale() const;

	void SetName(std::string newName);
	std::string GetName() const;

protected:
	ln::vec3 position;
	ln::vec3 rotation;
	ln::vec3 scale;
	std::string name = "entity";
};

class Geometry
{
public:
	Geometry(Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	~Geometry();
	const Vertex* verticies;
	Vertex* transformedVerticies;
	const unsigned int* indecies;
	unsigned int* transformedIndecies;
	const unsigned int verticiesCount;
	const unsigned int indeciesCount;
};

class Mesh : public Entity
{
public:
	Mesh();
	Mesh(Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(ln::vec3 position, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(ln::vec3 position, ln::vec3 rotation, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(ln::vec3 position);
	Mesh(ln::vec3 position, ln::vec3 rotation);
	Mesh(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale);
	~Mesh();

	void SetPosition(ln::vec3 position) override;
	void Move(ln::vec3 offset) override;
	void SetRotation(ln::vec3 rotation) override;
	void Rotate(ln::vec3 offset) override;
	void SetScale(ln::vec3 scale) override;
	void AddScale(ln::vec3 offset) override;

	Geometry* geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
	Material* material;
	bool isSmooth = true;
	ln::mat4 transformMatrix = {};
};

class Plane : public Mesh
{
public:
	Plane();
	Plane(ln::vec3 position);
	Plane(ln::vec3 position, ln::vec3 rotation);
	Plane(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale);
private:
	void setGeometry();
};

class Cube : public Mesh
{
public:
	Cube();
	Cube(ln::vec3 position);
	Cube(ln::vec3 position, ln::vec3 rotation);
	Cube(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale);
private:
	void setGeometry();
};

class Sphere : public Mesh
{
public:
	Sphere(int resolution = 10);
	Sphere(ln::vec3 position, int resolution = 10);
	Sphere(ln::vec3 position, ln::vec3 rotation, int resolution = 10);
	Sphere(ln::vec3 position, ln::vec3 rotation, ln::vec3 scale, int resolution = 10);
private:
	void setGeometry(int resolution);
};