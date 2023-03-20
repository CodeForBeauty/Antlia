#include "Camera.h"


Camera::Camera(float fov, float near, float far, int width, int height)
	: fov(ln::radians(fov)), near(near), far(far), width(width), height(height)
{
	camProjection = ln::perspective(fov, near, far, (float)height/width);
	camMetricies = ln::eulerRotation(rotation);
}

void Camera::SetPosition(ln::vec3 pos)
{
	position = pos;
	update = true;
	camMetricies.x.w = -pos.x;
	camMetricies.y.w = -pos.y;
	camMetricies.z.w = -pos.z;
}
void Camera::Move(ln::vec3 offset)
{
	position += offset;
	update = true;
	camMetricies.x.w = -position.x;
	camMetricies.y.w = -position.y;
	camMetricies.z.w = -position.z;
}
void Camera::SetRotation(ln::vec3 rot)
{
	rotation = rot;
	update = true;
	ln::mat3 tmpMat = ln::eulerRotation(rotation);
	forward = tmpMat * ln::vec3(0, 0, 1);
	right = tmpMat * ln::vec3(1, 0, 0);
	camMetricies = tmpMat;
}
void Camera::Rotate(ln::vec3 offset)
{
	rotation += offset;
	update = true;
	ln::mat3 tmpMat = ln::eulerRotation(rotation);
	forward = tmpMat * ln::vec3(0, 0, 1);
	right = tmpMat * ln::vec3(1, 0, 0);
	camMetricies = tmpMat;
}

ln::vec3 Camera::GetPosition() const
{
	return position;
}
ln::vec3 Camera::GetRotation() const
{
	return rotation;
}