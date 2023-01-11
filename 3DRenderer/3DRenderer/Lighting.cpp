#include <iostream>
#include "Lighting.h"

Light::Light(linmath::vec3 color) : color({ color.x, color.y, color.z, 0.0f }) {}

void Light::SetColor(linmath::vec3 col)
{
	color.x = col.x;
	color.y = col.y;
	color.z = col.z;
}

void Light::SetColor(float r, float g, float b)
{
	color.x = r;
	color.y = g;
	color.z = b;
}


DirectLight::DirectLight(linmath::vec3 color) : Light(color), rotation(new Vector3D()) {}
DirectLight::DirectLight(Vector3D* rotation, linmath::vec3 color) : Light(color), rotation(rotation) {}
DirectLight::~DirectLight(){ delete rotation, rotMetricies; }

Vector3D DirectLight::GetRotation() { return *rotation; }
void DirectLight::SetRotation(const Vector3D rot)
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
	linmath::rotateMetricies(*rotation, rotMetricies);
	pointing = linmath::multiplyByMetricies4x4(rotMetricies, {0, 1, 0});
	std::cout << pointing.x << " " << pointing.y << " " << pointing.z << std::endl;
}
void DirectLight::Rotate(const Vector3D offset)
{
	rotation->x += offset.x;
	rotation->y += offset.y;
	rotation->z += offset.z;
	linmath::rotateMetricies(*rotation, rotMetricies);
	pointing = linmath::multiplyByMetricies4x4(rotMetricies, { 0, 1, 0 });
}


PointLight::PointLight(linmath::vec3 color, float distance) : Light(color), distance(distance), position(new Vector3D()) {}
PointLight::PointLight(Vector3D* position, linmath::vec3 color, float distance) : Light(color), distance(distance), position(position) {}
PointLight::~PointLight() { delete position; }

float PointLight::GetDistance()
{
	return distance;
}

void PointLight::SetDistance(float dis)
{
	distance = dis;
}

Vector3D PointLight::GetPosition()
{
	return *position;
}

void PointLight::SetPosition(const Vector3D pos)
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
}

void PointLight::Move(const Vector3D offset)
{
	position->x += offset.x;
	position->y += offset.y;
	position->z += offset.z;
}


SpotLight::SpotLight(linmath::vec3 color, float distance, float angle) : DirectLight(color), PointLight(color, distance), angle(angle) {}
SpotLight::SpotLight(linmath::vec3 color, Vector3D* position, float distance, float angle) 
	: DirectLight(color), PointLight(position, color, distance), angle(angle) {}
SpotLight::SpotLight(linmath::vec3 color, Vector3D* position, Vector3D* rotation, float distance, float angle) 
	: DirectLight(rotation, color), PointLight(position, color, distance), angle(angle) {}

float SpotLight::GetAngle()
{
	return angle;
}

void SpotLight::SetAngle(float anl)
{
	angle = anl;
}