#pragma once

#include "Metrics.h"
#include "linmath.h"

class Light
{
public:
	Light(linmath::vec3 color);

	void SetColor(linmath::vec3 color);
	void SetColor(float r, float g, float b);

private:
	linmath::vec4 color;
};


class DirectLight : public Light
{
public:
	DirectLight(linmath::vec3 color);
	DirectLight(Vector3D* rotation, linmath::vec3 color);
	~DirectLight();

	Vector3D GetRotation();

	void SetRotation(const Vector3D rotation);
	void Rotate(const Vector3D offset);
protected:
	Vector3D* rotation;
	linmath::vec3 pointing = {0, 1, 0};
	float* rotMetricies = new float[16] {
											1, 0, 0, 0,
											0, 1, 0, 0,
											0, 0, 1, 0,
											0, 0, 1, 0
										};
};

class PointLight : public Light
{
public:
	PointLight(linmath::vec3 color, float distance = 5);
	PointLight(Vector3D* position, linmath::vec3 color, float distance = 10);
	~PointLight();

	float GetDistance();

	void SetDistance(float distance);

	Vector3D GetPosition();

	void SetPosition(const Vector3D position);
	void Move(const Vector3D offset);
protected:
	Vector3D* position;
	float distance;
};

class SpotLight : public DirectLight, public PointLight
{
public:
	SpotLight(linmath::vec3 color, float distance = 5, float angle = 25);
	SpotLight(linmath::vec3 color, Vector3D* position, float distance = 5, float angle = 25);
	SpotLight(linmath::vec3 color, Vector3D* position, Vector3D* rotation, float distance = 5, float angle = 25);

	float GetAngle();

	void SetAngle(float angle);
private:
	float angle;
};

