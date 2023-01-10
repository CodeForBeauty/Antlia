#include "Camera.h"

Camera::Camera(float view, float near, float far)
	:near(near), far(far)
{
	fov = linmath::deg2radians(view);
	position = new Vector3D();
	rotation = new Vector3D();

	linmath::rotateMetricies(*rotation, rotMetricies);
}

Camera::~Camera()
{
	delete position, rotation, rotMetricies;
}

void Camera::SetPosition(const Vector3D& pos)
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
	update = true;
}
void Camera::Move(const Vector3D& offset)
{
	position->x += offset.x;
	position->y += offset.y;
	position->z += offset.z;
	update = true;
}
void Camera::SetRotation(const Vector3D& rot)
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
	update = true;
	linmath::rotateMetricies(*rotation, rotMetricies);
}
void Camera::Rotate(const Vector3D& offset)
{
	rotation->x += offset.x;
	rotation->y += offset.y;
	rotation->z += offset.z;
	update = true;
	linmath::rotateMetricies(*rotation, rotMetricies);
}

Vector3D Camera::GetPosition() const
{
	return *position;
}
Vector3D Camera::GetRotation() const
{
	return -*rotation;
}