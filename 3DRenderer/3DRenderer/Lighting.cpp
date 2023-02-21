#include <iostream>
#include "Lighting.h"

Light::Light(linmath::vec3 color) : color({ color.x, color.y, color.z, 0.0f }), type(0) {}

void Light::SetColor(linmath::vec3 col)
{
	color.x = col.x;
	color.y = col.y;
	color.z = col.z;
}

int Light::GetType()
{
	return type;
}

void Light::SetType(int ty)
{
	type = ty;
}

void Light::SetColor(float r, float g, float b)
{
	color.x = r;
	color.y = g;
	color.z = b;
}

linmath::vec3 Light::GetColor() const
{
	return { color.x, color.y, color.z };
}

void Light::SetName(std::string newName)
{
	name = newName;
}
std::string Light::GetName() const
{
	return name;
}


DirectLight::DirectLight(linmath::vec3 color) : Light(color), rotation(new Vector3D()) { SetName("directLight"); SetType(1); }
DirectLight::DirectLight(Vector3D* rotation, linmath::vec3 color) : Light(color), rotation(rotation) { SetName("directLight"); SetType(1); }
DirectLight::~DirectLight(){ delete rotation, rotMetricies; }

Vector3D DirectLight::GetRotation() { return *rotation; }
linmath::vec3 DirectLight::GetVector() { return pointing; }
void DirectLight::SetRotation(const Vector3D rot)
{
	rotation->x = rot.x;
	rotation->y = rot.y;
	rotation->z = rot.z;
	linmath::rotateMetricies(*rotation, rotMetricies);
	pointing = linmath::multiplyByMetricies4x4(rotMetricies, {0, 1, 0});
}
void DirectLight::Rotate(const Vector3D offset)
{
	rotation->x += offset.x;
	rotation->y += offset.y;
	rotation->z += offset.z;
	linmath::rotateMetricies(*rotation, rotMetricies);
	pointing = linmath::multiplyByMetricies4x4(rotMetricies, { 0, 1, 0 });
}


PointLight::PointLight(linmath::vec3 color, float distance) : Light(color), distance(distance), position(new Vector3D()) { SetName("pointLight"); SetType(2); }
PointLight::PointLight(Vector3D* position, linmath::vec3 color, float distance) : Light(color), distance(distance), position(position) { SetName("pointLight"); SetType(2); }
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


SpotLight::SpotLight(linmath::vec3 color, float distance, float angle) : DirectLight(color), angle(angle), distance(distance), position(new Vector3D()) 
	{ SetName("spotLight"); SetType(3); }
SpotLight::SpotLight(linmath::vec3 color, Vector3D* position, float distance, float angle) 
	: DirectLight(color), angle(angle), distance(distance), position(position) { SetName("spotLight"); SetType(3); }
SpotLight::SpotLight(linmath::vec3 color, Vector3D* position, Vector3D* rotation, float distance, float angle) 
	: DirectLight(rotation, color), angle(angle), distance(distance), position(position) { SetName("spotLight"); SetType(3); }

float SpotLight::GetAngle()
{
	return angle;
}

void SpotLight::SetAngle(float anl)
{
	angle = anl;
}

float SpotLight::GetDistance()
{
	return distance;
}
void SpotLight::SetDistance(float dis)
{
	distance = dis;
}

Vector3D SpotLight::GetPosition()
{
	return *position;
}

void SpotLight::SetPosition(const Vector3D pos)
{
	position->x = pos.x;
	position->y = pos.y;
	position->z = pos.z;
}
void SpotLight::Move(const Vector3D offset)
{
	position->x += offset.x;
	position->y += offset.y;
	position->z += offset.z;
}