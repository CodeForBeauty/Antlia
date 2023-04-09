#pragma once

#include "Metrics.h"
#include "Material.h"
#include "linmath.h"

class Entity
{
public:
	Entity();
	Entity(lm::vec3 position);
	Entity(lm::vec3 position, lm::vec3 rotation);
	Entity(lm::vec3 position, lm::vec3 rotation, lm::vec3 scale);
	~Entity();

	virtual void SetPosition(lm::vec3 position);
	virtual void Move(lm::vec3 offset);
	virtual void SetRotation(lm::vec3 rotation);
	virtual void Rotate(lm::vec3 offset);
	virtual void SetScale(lm::vec3 scale);
	virtual void AddScale(lm::vec3 offset);

	lm::vec3 GetPosition() const;
	lm::vec3 GetRotation() const;
	lm::vec3 GetScale() const;

	void SetName(std::string newName);
	std::string GetName() const;

protected:
	lm::vec3 position;
	lm::vec3 rotation;
	lm::vec3 scale;
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
	Mesh(lm::vec3 position, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(lm::vec3 position, lm::vec3 rotation, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(lm::vec3 position, lm::vec3 rotation, lm::vec3 scale, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(lm::vec3 position);
	Mesh(lm::vec3 position, lm::vec3 rotation);
	Mesh(lm::vec3 position, lm::vec3 rotation, lm::vec3 scale);
	~Mesh();

	void SetPosition(lm::vec3 position) override;
	void Move(lm::vec3 offset) override;
	void SetRotation(lm::vec3 rotation) override;
	void Rotate(lm::vec3 offset) override;
	void SetScale(lm::vec3 scale) override;
	void AddScale(lm::vec3 offset) override;

	Geometry* geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
	Material* material;
	bool isSmooth = true;
	lm::mat4 transformMatrix = {};
};

class Plane : public Mesh
{
public:
	Plane();
	Plane(lm::vec3 position);
	Plane(lm::vec3 position, lm::vec3 rotation);
	Plane(lm::vec3 position, lm::vec3 rotation, lm::vec3 scale);
private:
	void setGeometry();
};

class Cube : public Mesh
{
public:
	Cube();
	Cube(lm::vec3 position);
	Cube(lm::vec3 position, lm::vec3 rotation);
	Cube(lm::vec3 position, lm::vec3 rotation, lm::vec3 scale);
private:
	void setGeometry();
};

class Sphere : public Mesh
{
public:
	Sphere(int resolution = 10);
	Sphere(lm::vec3 position, int resolution = 10);
	Sphere(lm::vec3 position, lm::vec3 rotation, int resolution = 10);
	Sphere(lm::vec3 position, lm::vec3 rotation, lm::vec3 scale, int resolution = 10);
private:
	void setGeometry(int resolution);
};