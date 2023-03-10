#pragma once

#include "Metrics.h"

#define PI 3.14159

namespace linmath
{
	struct vec2 { float x, y; };
	struct vec3 { float x, y, z; };
	struct vec4 { float x, y, z, w; };

	void perspective(float width, float height, float fov, float far, float near, float* out);
	void orthographic(float right, float left, float top, float bottom, float far, float near, float* out);

	float deg2radians(float degree);
	void multiplyMetricies4x4(float* metrix1, float* metrix2, float* out);
	void rotateMetricies(Vector3D& degree, float* out);
	vec3 multiplyByMetricies4x4(float* metricies, vec3 position);

	vec3 addVector3dByVec3(const Vector3D vec1, vec3 vec2);
	vec3 subVector3dByVec3(Vector3D vec1, vec3 vec2);
	vec3 mulVector3dByVec3(Vector3D vec1, vec3 vec2);
}

struct Vertex
{
	linmath::vec3 position;
	linmath::vec3 normal;
	linmath::vec2 uv;
	linmath::vec4 color;
};