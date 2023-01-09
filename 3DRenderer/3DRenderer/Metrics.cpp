#include "Metrics.h"


Vector3D::Vector3D() : x(0), y(0), z(0) {}
Vector3D::Vector3D(float x, float y) : x(x), y(y), z(0) {}
Vector3D::Vector3D(float x, float y, float z) : x(x), y(y), z(z) {}

Vector3D Vector3D::operator+ (Vector3D& value)
{
	return Vector3D(x + value.x, y + value.y, z + value.z);
}

Vector3D Vector3D::operator- (Vector3D& value)
{
	return Vector3D(x - value.x, y - value.y, z - value.z);
}

Vector3D Vector3D::operator* (Vector3D& value)
{
	return Vector3D(x * value.x, y * value.y, z * value.z);
}

Vector3D Vector3D::operator/ (Vector3D& value)
{
	return Vector3D(x / value.x, y / value.y, z / value.z);
}


Vector3D Vector3D::operator+ (const Vector3D& value)
{
	return Vector3D(x + value.x, y + value.y, z + value.z);
}

Vector3D Vector3D::operator- (const Vector3D& value)
{
	return Vector3D(x - value.x, y - value.y, z - value.z);
}

Vector3D Vector3D::operator* (const Vector3D& value)
{
	return Vector3D(x * value.x, y * value.y, z * value.z);
}

Vector3D Vector3D::operator/ (const Vector3D& value)
{
	return Vector3D(x / value.x, y / value.y, z / value.z);
}

Vector3D Vector3D::operator- ()
{
	return Vector3D(-x, -y, -z);
}


Vector3D Vector3D::operator+ (float& value)
{
	return Vector3D(x + value, y + value, z + value);
}

Vector3D Vector3D::operator- (float& value)
{
	return Vector3D(x - value, y - value, z - value);
}

Vector3D Vector3D::operator* (float& value)
{
	return Vector3D(x * value, y * value, z * value);
}

Vector3D Vector3D::operator/ (float& value)
{
	return Vector3D(x / value, y / value, z / value);
}

void Vector3D::operator+=(Vector3D& value)
{
	x += value.x;
	y += value.y;
	z += value.z;
}

void Vector3D::operator+=(float& value)
{
	x += value;
	y += value;
	z += value;
}