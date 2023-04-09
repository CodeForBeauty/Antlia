#include <GL/glew.h>

#include <iostream>
#include "Lighting.h"

Light::Light(lm::vec3 color) : color(color), type(0) {}

void Light::SetColor(lm::vec3 col)
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

void Light::SetColor(float r, float g, float b, float a)
{
	color.x = r;
	color.y = g;
	color.z = b;
	color.w = a;
}

lm::vec4 Light::GetColor() const
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

void Light::UpdateProj(lm::vec3 camera) {}


DirectLight::DirectLight(lm::vec3 color) : Light(color) { 
	SetName("directLight");
	SetType(1);
	UpdateProj();
}
DirectLight::DirectLight(lm::vec3 rotation, lm::vec3 color) : Light(color), rotation(rotation) { SetName("directLight"); SetType(1); }

lm::vec3 DirectLight::GetRotation() { 
	return rotation;
}
lm::vec3 DirectLight::GetForward() { return forward; }
void DirectLight::SetRotation(lm::vec3 rot)
{
	rotation = rot;
	lm::mat3 tmpMat = lm::eulerRotation(rot);
	forward = tmpMat * lm::vec3(0, 1, 0);
	up = tmpMat * lm::vec3(0, 0, 1);
	rotMetricies = tmpMat;
	UpdateProj();
}
void DirectLight::Rotate(lm::vec3 offset)
{
	rotation += offset;
	lm::mat3 tmpMat = lm::eulerRotation(rotation);
	forward = tmpMat * lm::vec3(0, 1, 0);
	up = tmpMat * lm::vec3(0, 0, 1);
	rotMetricies = tmpMat;
	UpdateProj();
}

void DirectLight::UpdateProj(lm::vec3 camera)
{
	lm::mat4 proj = lm::orthographic(-10, 10, -10, 10, -10, 10);
	lm::mat4 view = lm::lookAt(camera + (forward * 20), camera, up);
	projection = proj * view;
}


PointLight::PointLight(lm::vec3 color, float distance) : Light(color), distance(distance) 
{
	SetName("pointLight");
	SetType(2);
}
PointLight::PointLight(lm::vec3 position, lm::vec3 color, float distance) : Light(color), distance(distance), position(position) 
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

lm::vec3 PointLight::GetPosition()
{
	return position;
}

void PointLight::SetPosition(lm::vec3 pos)
{
	position = pos;
}

void PointLight::Move(lm::vec3 offset)
{
	position += offset;
}


SpotLight::SpotLight(lm::vec3 color, float distance, float angle) 
	: DirectLight(color), angle(angle), distance(distance)
{ 
	SetName("spotLight"); 
	SetType(3);
	UpdateProj({0, 0, 0});
}
SpotLight::SpotLight(lm::vec3 color, lm::vec3 position, float distance, float angle) 
	: DirectLight(color), angle(angle), distance(distance), position(position) 
{
	SetName("spotLight");
	SetType(3);
}
SpotLight::SpotLight(lm::vec3 color, lm::vec3 position, lm::vec3 rotation, float distance, float angle) 
	: DirectLight(rotation, color), angle(angle), distance(distance), position(position) 
{ 
	SetName("spotLight"); 
	SetType(3);
}

void SpotLight::UpdateProj(lm::vec3 camera)
{
	lm::mat4 proj = lm::perspective(lm::radians(90), 0.f, 10.0f);
	lm::mat4 view = lm::lookAt((forward * 20), {0, 0, 0}, -up);
	projection = view * proj;
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

lm::vec3 SpotLight::GetPosition()
{
	return position;
}

void SpotLight::SetPosition(lm::vec3 pos)
{
	position = pos;
}
void SpotLight::Move(lm::vec3 offset)
{
	position += offset;
}