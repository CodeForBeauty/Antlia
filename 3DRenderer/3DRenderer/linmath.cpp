#include "linmath.h"

using namespace lm;


float lm::radians(float angle)
{
	return angle * PI / 180;
}

float lm::dot(vec2 v1, vec2 v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

float lm::dot(vec3 v1, vec3 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float lm::dot(vec4 v1, vec4 v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

float lm::cross(vec2 v1, vec2 v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

vec3 lm::cross(vec3 v1, vec3 v2)
{
	vec3 out = {};
	out.x = v1.y * v2.z - v1.z * v2.y;
	out.y = v1.z * v2.x - v1.x * v2.z;
	out.z = v1.x * v2.y - v1.y * v2.x;
	return out;
}


float lm::magnitude(vec2 v)
{
	return std::sqrt(dot(v, v));
}

float lm::magnitude(vec3 v)
{
	return std::sqrt(dot(v, v));
}

float lm::magnitude(vec4 v)
{
	return std::sqrt(dot(v, v));
}

vec2 lm::normalize(vec2 v)
{
	return v / magnitude(v);
}

vec3 lm::normalize(vec3 v)
{
	return v / magnitude(v);
}

vec4 lm::normalize(vec4 v)
{
	return v / magnitude(v);
}


vec2 lm::mat2::getX()
{
	return { x.x, y.x };
}

vec2 lm::mat2::getY()
{
	return { x.y, y.y };
}

vec3 lm::mat3::getX()
{
	return { x.x, y.x, z.x };
}

vec3 lm::mat3::getY()
{
	return { x.y, y.y, z.y };
}

vec3 lm::mat3::getZ()
{
	return { x.z, y.z, z.z };
}

vec4 lm::mat4::getX()
{
	return { x.x, y.x, z.x, w.x };
}

vec4 lm::mat4::getY()
{
	return { x.y, y.y, z.y, w.y };
}

vec4 lm::mat4::getZ()
{
	return { x.z, y.z, z.z, w.z };
}

vec4 lm::mat4::getW()
{
	return { x.w, y.w, z.w, w.w };
}


mat4 lm::orthographic(float left, float right, float bottom, float top, float back, float front)
{
	mat4 out = {};

	out.x.x = 2 / (right - left);
	out.y.y = 2 / (top - bottom);
	out.z.z = 2 / (front - back);

	out.w.x = -((right + left) / (right - left));
	out.w.y = -((top + bottom) / (top - bottom));
	out.w.z = -((front + back) / (front - back));
	out.w.w = 1;

	return out;
}

mat4 lm::perspective(float fov, float near, float far, float ratio)
{
	mat4 out = {};
	out.y.y = 1 / std::tan(fov / 2);
	out.x.x = out.y.y * ratio;
	out.z.z = far / (far - near);
	//out.z.w = -(far / (far - near)) * near;
	out.w.z = 1.0;

	return out;
}

mat4 lm::lookAt(vec3 at, vec3 eye, vec3 up)
{
	mat4 out = {};
	vec3 zaxis = normalize(at - eye);
	vec3 xaxis = normalize(cross(up, zaxis));
	vec3 yaxis = cross(zaxis, xaxis);

	out.x.x = xaxis.x;
	out.y.x = xaxis.y;
	out.z.x = xaxis.z;

	out.x.y = yaxis.x;
	out.y.y = yaxis.y;
	out.z.y = yaxis.z;

	out.x.z = zaxis.x;
	out.y.z = zaxis.y;
	out.z.z = zaxis.z;

	out.w = vec4(-dot(xaxis, eye), -dot(yaxis, eye), -dot(zaxis, eye), 1.0f);

	return out;
}

mat3 lm::eulerRotation(vec3 degree)
{
	vec3 theta = { radians(degree.x), radians(degree.y), radians(degree.z) };

	mat3 rotZ = {};
	rotZ.x = { std::cos(theta.z), -std::sin(theta.z), 0.0f };
	rotZ.y = { -rotZ.x.y, rotZ.x.x, 0.0f };
	rotZ.z.z = 1.0f;

	mat3 rotY = {};
	rotY.x = { std::cos(theta.y), 0.0f, -std::sin(theta.y) };
	rotY.y.y = 1.0f;
	rotY.z = { -rotY.x.z, 0.0f, rotY.x.x };

	mat3 rotX = {};
	rotX.x.x = 1.0f;
	rotX.y = { 0.0f, std::cos(theta.x), -std::sin(theta.x) };
	rotX.z = { 0.0f, -rotX.y.z, rotX.y.y };

	mat3 out = rotZ * rotY * rotX;
	return out;
}

mat4 lm::transform(vec3 position, vec3 rotation)
{
	mat4 out = eulerRotation(rotation);
	out.x.w = position.x;
	out.y.w = position.y;
	out.z.w = position.z;
	return out;
}

vec3 lm::cutVec3(vec4 vec) { return { vec.x, vec.y, vec.z }; }
vec2 lm::cutVec2(vec4 vec) { return { vec.x, vec.y }; }
vec2 lm::cutVec2(vec3 vec) { return { vec.x, vec.y }; }

mat2 lm::cutMat2(mat4 matrix) { return { cutVec2(matrix.x), cutVec2(matrix.y) }; }
mat2 lm::cutMat2(mat3 matrix) { return { cutVec2(matrix.x), cutVec2(matrix.y) }; }
mat3 lm::cutMat3(mat4 matrix) { return { cutVec3(matrix.x), cutVec3(matrix.y), cutVec3(matrix.z) }; }


std::ostream& operator <<(std::ostream& os, vec2 vec)
{
	os << "X: " << vec.x << " Y: " << vec.y << " ";
	return os;
}

std::ostream& operator <<(std::ostream& os, vec3 vec)
{
	os << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z << " ";
	return os;
}

std::ostream& operator <<(std::ostream& os, vec4 vec)
{
	os << "X: " << vec.x << " Y: " << vec.y << " Z: " << vec.z << " W: " << vec.w << " ";
	return os;
}
