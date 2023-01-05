#pragma once

#include "Metrics.h"

#define PI 3.14159

namespace linmath
{
	void perspective(float width, float height, float fov, float far, float near, float* out);

	float deg2radians(float degree);
	void multiplyMetricies4x4(float* metrix1, float* metrix2, float* out);
	void rotateMetricies(Vector3D& degree, float* out);
}