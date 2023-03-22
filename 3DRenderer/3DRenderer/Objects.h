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
	Mesh(Vector3D* position, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(Vector3D* position, Vector3D* rotation, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale, Vertex* verticies, unsigned int* indecies, unsigned int verticiesCount, unsigned int indeciesCount);
	Mesh(Vector3D* position);
	Mesh(Vector3D* position, Vector3D* rotation);
	Mesh(Vector3D* position, Vector3D* rotation, Vector3D* scale);
	~Mesh();

	void SetPosition(const Vector3D& position) const override;
	void Move(const Vector3D& offset) const override;
	void SetRotation(const Vector3D& rotation) const override;
	void Rotate(const Vector3D& offset) const override;
	void SetScale(const Vector3D& scale) const override;
	void AddScale(const Vector3D& offset) const override;

	Geometry* geometry = new Geometry(
		new Vertex[3]{ { {  0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {1.0, 1.0}},
					   { { -0.5,  0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 1.0}},
					   { { -0.5, -0.5, 0 }, {0.0, 0.0, 1.0}, {0.0, 0.0}} },
		new unsigned int[3] {0, 1, 2},
		3, 3
	);
	Material* material;
	bool isSmooth = true;
	float* rotMetricies = new float[16] {
											1, 0, 0, 0,
											0, 1, 0, 0,
											0, 0, 1, 0,
											0, 0, 1, 0
										};
};

class Plane : public Mesh
{
public:
	Plane();
	Plane(Vector3D* position);
	Plane(Vector3D* position, Vector3D* rotation);
	Plane(Vector3D* position, Vector3D* rotation, Vector3D* scale);
private:
	void setGeometry();
};

class Cube : public Mesh
{
public:
	Cube();
	Cube(Vector3D* position);
	Cube(Vector3D* position, Vector3D* rotation);
	Cube(Vector3D* position, Vector3D* rotation, Vector3D* scale);
private:
	void setGeometry();
};

class Sphere : public Mesh
{
public:
	Sphere(int resolution = 10);
	Sphere(Vector3D* position, int resolution = 10);
	Sphere(Vector3D* position, Vector3D* rotation, int resolution = 10);
	Sphere(Vector3D* position, Vector3D* rotation, Vector3D* scale, int resolution = 10);
private:
	void setGeometry(int resolution);
};