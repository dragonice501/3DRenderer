#pragma once

#include <math.h>

struct Vec2Int
{
	int x;
	int y;
};

struct Vec2
{
	Vec2() { x = 0.0f, y = 0.0f; }
	Vec2(float x, float y) { this->x = x; this->y = y; }
	float x;
	float y;

	float Magnitude()
	{
		return sqrtf(x * x + y * y);
	}

	float SqrMagnitude()
	{
		return x * x + y * y;
	}

	void Normalize()
	{
		float mag = Magnitude();
		x /= mag;
		y /= mag;
	}
};

struct Vec3
{
	Vec3() { x = 0.0f, y = 0.0f, z = 0.0f; }
	Vec3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	float x;
	float y;
	float z;
};

struct Vec4
{
	float x;
	float y;
	float z;
	float w;
};

Vec2 Vec2New(float x, float y);

Vec2 Vec2Add(const Vec2& v0, const Vec2& v1);
Vec2 Vec2Subtract(const Vec2& v0, const Vec2& v1);
Vec2 Vec2Multiply(const Vec2& v, float scalar);
Vec2 Vec2Divide(const Vec2& v, float scalar);

float Vec2Magnitude(const Vec2& v);
float Vec2MagnitudeSqr(const Vec2& v);

float Vec2Dot(const Vec2& a, const Vec2& b);
void Vec2Normalize(Vec2& v);

Vec3 Vec3New(float x, float y, float z);
Vec3 Vec3Clone(const Vec3* v);

Vec3 Vec3Add(const Vec3& v0, const Vec3& v1);
Vec3 Vec3Subtract(const Vec3& v0, const Vec3& v1);
Vec3 Vec3Multiply(const Vec3& v, float scalar);
Vec3 Vec3Divide(const Vec3& v, float scalar);

float Vec3Magnitude(const Vec3& v);
float Vec3MagnitudeSqr(const Vec3& v);

float Vec3Dot(const Vec3& a, const Vec3& b);
Vec3 Vec3Cross(const Vec3& a, const Vec3& b);
void Vec3Normalize(Vec3& v);

Vec3 Vec3RotateX(const Vec3& v, float angle);
Vec3 Vec3RotateY(const Vec3& v, float angle);
Vec3 Vec3RotateZ(const Vec3& v, float angle);

Vec2 Vec2FromVec4(const Vec4& v);
Vec3 Vec3FromVec4(const Vec4& v);
Vec4 Vec4FromVec3(const Vec3& v);