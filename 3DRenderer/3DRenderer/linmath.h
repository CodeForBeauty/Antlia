#pragma once
#include <iostream>


const double PI = 3.14159265359;

namespace ln
{
	float radians(float angle);

	struct vec2
	{
		float x, y;

		vec2() { x = 0; y = 0; }
		vec2(float x1, float y1) { x = x1; y = y1; }

		vec2 operator +(const vec2& a) { return { x + a.x, y + a.y }; }
		vec2 operator -(const vec2& a) { return { x - a.x, y - a.y }; }
		vec2 operator /(const vec2& a) { return { x / a.x, y / a.y }; }
		vec2 operator *(const vec2& a) { return { x * a.x, y * a.y }; }

		void operator +=(const vec2& a) { this->x += a.x; this->y += a.y; }
		void operator -=(const vec2& a) { this->x -= a.x; this->y -= a.y; }

		vec2 operator -() { return { -x, -y }; }
		void operator ++(int) { this->x++; this->y++; }

		vec2 operator +(const float& val) { return { x + val, y + val }; }
		vec2 operator -(const float& val) { return { x - val, y - val }; }
		vec2 operator /(const float& val) { return { x / val, y / val }; }
		vec2 operator *(const float& val) { return { x * val, y * val }; }

		void operator +=(const float& val) { this->x += val; this->y += val; }
		void operator -=(const float& val) { this->x -= val; this->y -= val; }

		void operator =(const vec2& a) { this->x = a.x; this->y = a.y; }

		operator float* ()
		{
			return &x;
		}
	};
	struct vec3
	{
		float x, y, z;

		vec3() { x = 0; y = 0; z = 0; }
		vec3(vec2 v) { x = v.x; y = v.y; z = 0; }
		vec3(float x1, float y1, float z1) { x = x1; y = y1; z = z1; }

		vec3 operator +(const vec3& a) { return { x + a.x, y + a.y, z + a.z }; }
		vec3 operator -(const vec3& a) { return { x - a.x, y - a.y, z - a.z }; }
		vec3 operator /(const vec3& a) { return { x / a.x, y / a.y, z / a.z }; }
		vec3 operator *(const vec3& a) { return { x * a.x, y * a.y, z * a.z }; }

		void operator +=(const vec3& a) { this->x += a.x; this->y += a.y; this->z += a.z; }
		void operator -=(const vec3& a) { this->x -= a.x; this->y -= a.y; this->z -= a.z; }

		vec3 operator -() { return { -x, -y, -z }; }
		void operator ++(int) { this->x++; this->y++; this->z++; }

		vec3 operator +(const float& val) { return { x + val, y + val, z + val }; }
		vec3 operator -(const float& val) { return { x - val, y - val, z - val }; }
		vec3 operator /(const float& val) { return { x / val, y / val, z / val }; }
		vec3 operator *(const float& val) { return { x * val, y * val, z * val }; }

		void operator +=(const float& val) { this->x += val; this->y += val; this->z += val; }
		void operator -=(const float& val) { this->x -= val; this->y -= val; this->z += val; }

		void operator =(const vec3& a) { this->x = a.x; this->y = a.y; this->z = a.z; }
		void operator =(const vec2& a) { this->x = a.x; this->y = a.y; }

		operator float* ()
		{
			return &x;
		}
	};
	struct vec4
	{
		float x, y, z, w;

		vec4() { x = 0; y = 0; z = 0; w = 0; }
		vec4(vec2 v) { x = v.x; y = v.y; z = 0; w = 0; }
		vec4(vec3 v) { x = v.x; y = v.y; z = v.z; w = 1; }
		vec4(float x1, float y1, float z1, float w1) { x = x1; y = y1; z = z1; w = w1; }

		vec4 operator +(const vec4& a) { return { x + a.x, y + a.y, z + a.z, w + a.w }; }
		vec4 operator -(const vec4& a) { return { x - a.x, y - a.y, z - a.z, w - a.w }; }
		vec4 operator /(const vec4& a) { return { x / a.x, y / a.y, z / a.z, w / a.w }; }
		vec4 operator *(const vec4& a) { return { x * a.x, y * a.y, z * a.z, w * a.w }; }

		void operator +=(const vec4& a) { this->x += a.x; this->y += a.y; this->z += a.z; this->w += a.w; }
		void operator -=(const vec4& a) { this->x -= a.x; this->y -= a.y; this->z -= a.z; this->w -= a.w; }

		vec4 operator -() { return { -x, -y, -z, -w }; }
		void operator ++(int) { this->x++; this->y++; this->z++; this->w++; }

		vec4 operator +(const float& val) { return { x + val, y + val, z + val, w + val }; }
		vec4 operator -(const float& val) { return { x - val, y - val, z - val, w - val }; }
		vec4 operator /(const float& val) { return { x / val, y / val, z / val, w / val }; }
		vec4 operator *(const float& val) { return { x * val, y * val, z * val, w * val }; }

		void operator +=(const float& val) { this->x += val; this->y += val; this->z += val; this->w += val; }
		void operator -=(const float& val) { this->x -= val; this->y -= val; this->z += val; this->w -= val; }

		void operator =(const vec2& a) { this->x = a.x; this->y = a.y; }
		void operator =(const vec3& a) { this->x = a.x; this->y = a.y; this->z = a.z; }
		void operator =(const vec4& a) { this->x = a.x; this->y = a.y; this->z = a.z; this->w = a.w; }

		operator float* ()
		{
			return &x;
		}
	};


	float dot(vec2 v1, vec2 v2);
	float dot(vec3 v1, vec3 v2);
	float dot(vec4 v1, vec4 v2);

	float cross(vec2 v1, vec2 v2);
	vec3 cross(vec3 v1, vec3 v2);

	float magnitude(vec2 v);
	float magnitude(vec3 v);
	float magnitude(vec4 v);

	vec2 normalize(vec2 v);
	vec3 normalize(vec3 v);
	vec4 normalize(vec4 v);


	struct mat2
	{
		vec2 x, y;

		vec2 getX();
		vec2 getY();

		mat2 operator *(const mat2& a)
		{
			vec2 x1 = getX();
			vec2 y1 = getY();
			mat2 out = {};
			out.x.x = dot(x, x1);
			out.x.y = dot(x, y1);
			out.y.x = dot(y, x1);
			out.y.y = dot(y, y1);

			return out;
		}
		vec2 operator *(const vec2& a)
		{
			vec2 out = {};
			out.x = dot(x, a);
			out.y = dot(y, a);

			return out;
		}

		void operator =(const mat2& a) { this->x = a.x; this->y = a.y; }

		operator float* ()
		{
			return &x.x;
		}
	};
	struct mat3
	{
		vec3 x, y, z;

		mat3() {}
		mat3(vec3 x1, vec3 y1, vec3 z1) { x = x1; y = y1; z = z1; }
		mat3(mat2 matrix) { x = matrix.x; y = matrix.y; }

		vec3 getX();
		vec3 getY();
		vec3 getZ();

		mat3 operator *(const mat3& a)
		{
			vec3 x1 = getX();
			vec3 y1 = getY();
			vec3 z1 = getZ();
			mat3 out = {};

			out.x.x = dot(x, x1);
			out.x.y = dot(x, y1);
			out.x.z = dot(x, z1);

			out.x.x = dot(y, x1);
			out.x.y = dot(y, y1);
			out.x.z = dot(y, z1);

			out.z.x = dot(z, x1);
			out.z.y = dot(z, y1);
			out.z.z = dot(z, z1);

			return out;
		}

		vec3 operator *(const vec3& a)
		{
			vec3 out = {};
			out.x = dot(x, a);
			out.y = dot(y, a);
			out.z = dot(z, a);

			return out;
		}

		void operator =(const mat2& a) { this->x = a.x; this->y = a.y; }
		void operator =(const mat3& a) { this->x = a.x; this->y = a.y; this->z = a.z; }

		operator float* ()
		{
			return &x.x;
		}
	};
	struct mat4
	{
		vec4 x, y, z, w;

		mat4() {}
		mat4(vec4 x1, vec4 y1, vec4 z1, vec4 w1) { x = x1; y = y1; z = z1; w = w1; }
		mat4(mat2 matrix) { x = matrix.x; y = matrix.y; }
		mat4(mat3 matrix) { x = matrix.x; y = matrix.y; z = matrix.z; }

		vec4 getX();
		vec4 getY();
		vec4 getZ();
		vec4 getW();

		mat4 operator *(const mat4& a)
		{
			vec4 x1 = getX();
			vec4 y1 = getY();
			vec4 z1 = getZ();
			vec4 w1 = getW();
			mat4 out = {};

			out.x.x = dot(x, x1);
			out.x.y = dot(x, y1);
			out.x.z = dot(x, z1);
			out.x.w = dot(x, w1);

			out.x.x = dot(y, x1);
			out.x.y = dot(y, y1);
			out.x.z = dot(y, z1);
			out.x.w = dot(y, w1);

			out.z.x = dot(z, x1);
			out.z.y = dot(z, y1);
			out.z.z = dot(z, z1);
			out.z.w = dot(z, w1);

			out.w.x = dot(w, x1);
			out.w.y = dot(w, y1);
			out.w.z = dot(w, z1);
			out.w.w = dot(w, w1);

			return out;
		}

		vec4 operator *(const vec4& a)
		{
			vec4 out = {};
			out.x = dot(x, a);
			out.y = dot(y, a);
			out.z = dot(z, a);
			out.w = dot(w, a);

			return out;
		}

		void operator =(const mat2& a) { this->x = a.x; this->y = a.y; }
		void operator =(const mat3& a) { this->x = a.x; this->y = a.y; this->z = a.z; }
		void operator =(const mat4& a) { this->x = a.x; this->y = a.y; this->z = a.z; this->w = a.w; }

		operator float* ()
		{
			return &x.x;
		}
	};

	mat4 orthographic(float left, float right, float bottom, float top, float back, float front);
	mat4 perspective(float fov, float near, float far, float ratio = 1.0f);
	mat4 lookAt(vec3 at, vec3 eye, vec3 up = { 0, 1, 0 });
	mat3 eulerRotation(vec3 degree);

	mat4 transform(vec3 position, vec3 rotation);

	vec3 cutVec3(vec4 vec);
	vec2 cutVec2(vec4 vec);
	vec2 cutVec2(vec3 vec);

	mat2 cutMat2(mat4 matrix);
	mat2 cutMat2(mat3 matrix);
	mat3 cutMat3(mat4 matrix);
}

std::ostream& operator <<(std::ostream& os, ln::vec2 vec);
std::ostream& operator <<(std::ostream& os, ln::vec3 vec);
std::ostream& operator <<(std::ostream& os, ln::vec4 vec);

struct Vertex
{
	ln::vec3 position;
	ln::vec3 normal;
	ln::vec2 uv;
	ln::vec4 color;
};