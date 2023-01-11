#pragma once

class Vector3D
{
public:
	Vector3D();
	Vector3D(float x, float y);
	Vector3D(float x, float y, float z);

	float x;
	float y;
	float z;

	Vector3D operator+ (Vector3D& value);
	Vector3D operator* (Vector3D& value);
	Vector3D operator- (Vector3D& value);
	Vector3D operator/ (Vector3D& value);

	Vector3D operator+ (const Vector3D& value);
	Vector3D operator* (const Vector3D& value);
	Vector3D operator- (const Vector3D& value);
	Vector3D operator/ (const Vector3D& value);
	
	Vector3D operator- ();
	
	Vector3D operator+ (float& value);
	Vector3D operator- (float& value);
	Vector3D operator* (float& value);
	Vector3D operator/ (float& value);

	void operator+= (Vector3D& value);
	void operator+= (float& value);

	void operator+= (const Vector3D& value);
};