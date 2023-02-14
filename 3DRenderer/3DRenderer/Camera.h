#pragma once

#include "Objects.h"
#include "linmath.h"

class Camera
{
public:
	Camera(float fov, float near, float far);
	~Camera();

	void SetPosition(const Vector3D& position);
	void Move(const Vector3D& offset);
	void SetRotation(const Vector3D& rotation);
	void Rotate(const Vector3D& offset);

	Vector3D GetPosition() const;
	Vector3D GetRotation() const;


	float fov;
	float near;
	float far;

	bool update = true;
	
	float* rotMetricies = new float[16] {
											1, 0, 0, 0,
											0, 1, 0, 0,
											0, 0, 1, 0,
											0, 0, 1, 0
										};

	Vector3D forward = Vector3D(0, 0, 1);
	Vector3D right = Vector3D(1, 0, 0);
private:
	Vector3D* position;
	Vector3D* rotation;
};