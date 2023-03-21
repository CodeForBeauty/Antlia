#pragma once

#include "Metrics.h"
#include "linmath.h"
#include "Shader.h"

class Light
{
public:
	Light(ln::vec3 color);

	void SetColor(ln::vec3 color);
	void SetColor(double r, double g, double b, double a);
	ln::vec4 GetColor() const;

	void SetName(std::string newName);
	std::string GetName() const;

	int GetType();

	float intensity = 1;

	virtual void UpdateProj(ln::vec3 camera = { 0, 0, 0 });

	ln::mat4 projection = {};


protected:
	void SetType(int type);

private:
	int type;
	ln::vec4 color;
	std::string name = "light";
};


class DirectLight : public Light
{
public:
	DirectLight(ln::vec3 color);
	DirectLight(ln::vec3 rotation, ln::vec3 color);

	ln::vec3 GetRotation();
	ln::vec3 GetForward();

	void SetRotation(ln::vec3 rotation);
	void Rotate(ln::vec3 offset);

	void UpdateProj(ln::vec3 camera = {0, 0, 0}) override;

protected:
	ln::vec3 rotation;
	ln::vec3 forward = {0, 1, 0};
	ln::vec3 up = {0, 0, 1};
	ln::mat4 rotMetricies = {};
};

class PointLight : public Light
{
public:
	PointLight(ln::vec3 color, float distance = 5);
	PointLight(ln::vec3 position, ln::vec3 color, float distance = 5);

	float GetDistance();

	void SetDistance(float distance);

	ln::vec3 GetPosition();

	void SetPosition(ln::vec3 position);
	void Move(ln::vec3 offset);
protected:
	ln::vec3 position;
	float distance;
};

class SpotLight : public DirectLight
{
public:
	SpotLight(ln::vec3 color, float distance = 5, float angle = 25);
	SpotLight(ln::vec3 color, ln::vec3 position, float distance = 5, float angle = 25);
	SpotLight(ln::vec3 color, ln::vec3 position, ln::vec3 rotation, float distance = 5, float angle = 25);

	float GetAngle();
	void SetAngle(float angle);

	float GetDistance();
	void SetDistance(float distance);

	void UpdateProj(ln::vec3 camera = {0, 0, 0}) override;

	ln::vec3 GetPosition();

	void SetPosition(ln::vec3 position);
	void Move(ln::vec3 offset);
private:
	ln::vec3 position;
	float angle;
	float distance;
};

