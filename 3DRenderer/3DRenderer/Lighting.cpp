#include <GL/glew.h>

#include <iostream>
#include "Lighting.h"

Light::Light(ln::vec3 color) : color(color), type(0) {}

void Light::SetColor(ln::vec3 col)
{
	color = col;
}

int Light::GetType()
{
	return type;
}

void Light::SetType(int ty)
{
	type = ty;
}

void Light::SetColor(double r, double g, double b, double a)
{
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}

ln::vec4 Light::GetColor() const
{
	return color;
}

void Light::SetName(std::string newName)
{
	name = newName;
}
std::string Light::GetName() const
{
	return name;
}

void Light::UpdateProj(ln::vec3 camera) {}


DirectLight::DirectLight(ln::vec3 color) : Light(color) { 
	SetName("directLight");
	SetType(1);
	UpdateProj();
}
DirectLight::DirectLight(ln::vec3 rotation, ln::vec3 color) : Light(color), rotation(rotation) { SetName("directLight"); SetType(1); }

ln::vec3 DirectLight::GetRotation() { 
	return rotation;
}
ln::vec3 DirectLight::GetForward() { return forward; }
void DirectLight::SetRotation(ln::vec3 rot)
{
	rotation = rot;
	ln::mat3 tmpMat = ln::eulerRotation(rot);
	forward = tmpMat * ln::vec3(0, 1, 0);
	up = tmpMat * ln::vec3(0, 0, 1);
	rotMetricies = tmpMat;
	UpdateProj();
}
void DirectLight::Rotate(ln::vec3 offset)
{
	rotation += offset;
	ln::mat3 tmpMat = ln::eulerRotation(rotation);
	forward = tmpMat * ln::vec3(0, 1, 0);
	up = tmpMat * ln::vec3(0, 0, 1);
	rotMetricies = tmpMat;
	UpdateProj();
}

void DirectLight::UpdateProj(ln::vec3 camera)
{
	ln::mat4 proj = ln::orthographic(-10, 10, -10, 10, -10, 10);
	ln::mat4 view = ln::lookAt(camera + (forward * 20), camera, up);
	projection = proj * view;
}


PointLight::PointLight(ln::vec3 color, float distance) : Light(color), distance(distance) 
{
	SetName("pointLight");
	SetType(2);
}
PointLight::PointLight(ln::vec3 position, ln::vec3 color, float distance) : Light(color), distance(distance), position(position) 
{
	SetName("pointLight");
	SetType(2);
}

float PointLight::GetDistance()
{
	return distance;
}

void PointLight::SetDistance(float dis)
{
	distance = dis;
}

ln::vec3 PointLight::GetPosition()
{
	return position;
}

void PointLight::SetPosition(ln::vec3 pos)
{
	position = pos;
}

void PointLight::Move(ln::vec3 offset)
{
	position += offset;
}


SpotLight::SpotLight(ln::vec3 color, float distance, float angle) 
	: DirectLight(color), angle(angle), distance(distance)
{ 
	SetName("spotLight"); 
	SetType(3);
	UpdateProj({0, 0, 0});
}
SpotLight::SpotLight(ln::vec3 color, ln::vec3 position, float distance, float angle) 
	: DirectLight(color), angle(angle), distance(distance), position(position) 
{
	SetName("spotLight");
	SetType(3);
}
SpotLight::SpotLight(ln::vec3 color, ln::vec3 position, ln::vec3 rotation, float distance, float angle) 
	: DirectLight(rotation, color), angle(angle), distance(distance), position(position) 
{ 
	SetName("spotLight"); 
	SetType(3);
}

void SpotLight::UpdateProj(ln::vec3 camera)
{
	ln::mat4 proj = ln::perspective(ln::radians(90), 0.1, 50);
	ln::mat4 view = ln::lookAt(position + (forward * 20), position, up);
	projection = proj * view;
}

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

ln::vec3 SpotLight::GetPosition()
{
	return position;
}

void SpotLight::SetPosition(ln::vec3 pos)
{
	position = pos;
}
void SpotLight::Move(ln::vec3 offset)
{
	position += offset;
}