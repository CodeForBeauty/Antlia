#pragma once

#include "linmath.h"


class Light
{
public:
	Light(lm::vec3 color);

	void SetColor(lm::vec3 color);
	void SetColor(float r, float g, float b, float a);
	lm::vec4 GetColor() const;

	void SetName(std::string newName);
	std::string GetName() const;

	int GetType();

	float intensity = 1;

	virtual void UpdateProj(lm::vec3 camera = { 0, 0, 0 });

	lm::mat4 projection = {};


protected:
	void SetType(int type);

private:
	int type;
	lm::vec4 color;
	std::string name = "light";
};


class DirectLight : public Light
{
public:
	DirectLight(lm::vec3 color);
	DirectLight(lm::vec3 rotation, lm::vec3 color);

	lm::vec3 GetRotation();
	lm::vec3 GetForward();

	void SetRotation(lm::vec3 rotation);
	void Rotate(lm::vec3 offset);

	void UpdateProj(lm::vec3 camera = {0, 0, 0}) override;

protected:
	lm::vec3 rotation;
	lm::vec3 forward = {0, 1, 0};
	lm::vec3 up = {0, 0, 1};
	lm::mat4 rotMetricies = {};
};

class PointLight : public Light
{
public:
	PointLight(lm::vec3 color, float distance = 5);
	PointLight(lm::vec3 position, lm::vec3 color, float distance = 5);

	float GetDistance();

	void SetDistance(float distance);

	lm::vec3 GetPosition();

	void SetPosition(lm::vec3 position);
	void Move(lm::vec3 offset);
protected:
	lm::vec3 position;
	float distance;
};

class SpotLight : public DirectLight
{
public:
	SpotLight(lm::vec3 color, float distance = 5, float angle = 25);
	SpotLight(lm::vec3 color, lm::vec3 position, float distance = 5, float angle = 25);
	SpotLight(lm::vec3 color, lm::vec3 position, lm::vec3 rotation, float distance = 5, float angle = 25);

	float GetAngle();
	void SetAngle(float angle);

	float GetDistance();
	void SetDistance(float distance);

	void UpdateProj(lm::vec3 camera = {0, 0, 0}) override;

	lm::vec3 GetPosition();

	void SetPosition(lm::vec3 position);
	void Move(lm::vec3 offset);
private:
	lm::vec3 position;
	float angle;
	float distance;
};

