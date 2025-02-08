#include "Vector.h"

#include <math.h>

Vec2 Vec2New(float x, float y)
{
    Vec2 result = { x, y };
    return result;
}

Vec2 Vec2Add(const Vec2& v0, const Vec2& v1)
{
    Vec2 result = { v0.x + v1.x, v0.y + v1.y };
    return result;
}

Vec2 Vec2Subtract(const Vec2& v0, const Vec2& v1)
{
    Vec2 result = { v0.x - v1.x, v0.y - v1.y };
    return result;
}

Vec2 Vec2Multiply(const Vec2& v, float scalar)
{
    Vec2 result = { v.x * scalar, v.y * scalar };
    return result;
}

Vec2 Vec2Divide(const Vec2& v, float scalar)
{
    Vec2 result = { v.x / scalar, v.y / scalar };
    return result;
}

float Vec2Magnitude(const Vec2& v)
{
    return sqrt(v.x * v.x + v.y * v.y);
}

float Vec2MagnitudeSqr(const Vec2& v)
{
    return v.x * v.x + v.y + v.y;
}

float Vec2Dot(const Vec2& a, const Vec2& b)
{
    return a.x * b.x + a.y * b.y;
}

void Vec2Normalize(Vec2& v)
{
    v = Vec2Divide(v, Vec2Magnitude(v));
}

Vec3 Vec3New(float x, float y, float z)
{
    Vec3 result = { x, y, z };
    return result;
}

Vec3 Vec3Clone(const Vec3* v)
{
    Vec3 newVector = { v->x, v->y, v->z };
    
    return newVector;
}

Vec3 Vec3Add(const Vec3& v0, const Vec3& v1)
{
    Vec3 result = { v0.x + v1.x, v0.y + v1.y, v0.z + v1.z };
    return result;
}

Vec3 Vec3Subtract(const Vec3& v0, const Vec3& v1)
{
    Vec3 result = { v0.x - v1.x, v0.y - v1.y, v0.z - v1.z };
    return result;
}

Vec3 Vec3Multiply(const Vec3& v, float scalar)
{
    Vec3 result = { v.x * scalar, v.y * scalar, v.z * scalar };
    return result;
}

Vec3 Vec3Divide(const Vec3& v, float scalar)
{
    Vec3 result = { v.x / scalar, v.y / scalar, v.z / scalar };
    return result;
}

float Vec3Magnitude(const Vec3& v)
{
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

float Vec3MagnitudeSqr(const Vec3& v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float Vec3Dot(const Vec3& a, const Vec3& b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vec3 Vec3Cross(const Vec3& a, const Vec3& b)
{
    Vec3 result =
    {
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    };
    return result;
}

void Vec3Normalize(Vec3& v)
{
    v = Vec3Divide(v, Vec3Magnitude(v));
}

Vec3 Vec3RotateX(const Vec3& v, float angle)
{
    Vec3 rotatedVector =
    {
        v.x,
        v.y * cosf(angle) - v.z * sinf(angle),
        v.y * sinf(angle) + v.z * cosf(angle)
    };

    return rotatedVector;
}

Vec3 Vec3RotateY(const Vec3& v, float angle)
{
    Vec3 rotatedVector =
    {
        v.x * cosf(angle) - v.z * sinf(angle),
        v.y,
        v.x * sinf(angle) + v.z * cosf(angle)
    };

    return rotatedVector;
}

Vec3 Vec3RotateZ(const Vec3& v, float angle)
{
    Vec3 rotatedVector =
    {
        v.x * cosf(angle) - v.y * sinf(angle),
        v.x * sinf(angle) + v.y * cosf(angle),
        v.z
    };

    return rotatedVector;
}

Vec2 Vec2FromVec4(const Vec4& v)
{
    Vec2 result = { v.x, v.y };
    return result;
}

Vec3 Vec3FromVec4(const Vec4& v)
{
    Vec3 result =
    {
        v.x,
        v.y,
        v.z
    };

    return result;
}

Vec4 Vec4FromVec3(const Vec3& v)
{
    Vec4 result =
    {
        v.x,
        v.y,
        v.z,
        1.0f
    };

    return result;
}
