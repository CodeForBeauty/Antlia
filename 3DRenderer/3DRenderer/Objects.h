#pragma once

#include "Metrics.h"
#include "Material.h"
#include "linmath.h"

class Entity
{
public:
	Entity();
	Entity(Vector3D* position);
	Entity(Vector3D* position, Vector3D* rotation);
	Entity(Vector3D* position, Vector3D* rotation, Vector3D* scale);
	~Entity();

	virtual void SetPosition(const Vector3D& position) const;
	virtual void Move(const Vector3D& offset) const;
	virtual void SetRotation(const Vector3D& rotation) const;
	virtual void Rotate(const Vector3D& offset) const;
	virtual void SetScale(const Vector3D& scale) const;
	virtual void AddScale(const Vector3D& offset) const;

	Vector3D GetPosition() const;
	Vector3D GetRotation() const;
	Vector3D GetScale() const;

	void SetName(std::string newName);
	std::string GetName() const;

protected:
	Vector3D* position;
	Vector3D* rotation;
	Vector3D* scale;
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