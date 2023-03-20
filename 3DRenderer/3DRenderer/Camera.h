#pragma once

#include "Objects.h"
#include "linmath.h"

class Camera
{
public:
	Camera(float fov, float near, float far, int width, int height);
	~Camera();

	void SetPosition(ln::vec3 position);
	void Move(ln::vec3 offset);
	void SetRotation(ln::vec3 rotation);
	void Rotate(ln::vec3 offset);

	ln::vec3 GetPosition() const;
	ln::vec3 GetRotation() const;

	void UpdateProjection(double windowRatio);


	float fov;
	float near;
	float far;
	int width, height;

	bool update = true;
	
	ln::mat4 camMetricies = {};
	ln::mat4 camProjection = {};

	ln::vec3 forward = ln::vec3(0, 0, 1);
	ln::vec3 right = ln::vec3(1, 0, 0);

private:
	ln::vec3 position = {};
	ln::vec3 rotation = {};
};