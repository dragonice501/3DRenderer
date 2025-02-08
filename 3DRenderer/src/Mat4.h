#pragma once

#include "Vector.h";

struct Mat4
{
	float mat[4][4];
};

Mat4 Mat4Identity();
Mat4 Mat4MakeScale(float sx, float sy, float sz);
Mat4 Mat4MakeTranslation(float tx, float ty, float tz);
Mat4 Mat4MakeRotationX(float a);
Mat4 Mat4MakeRotationY(float a);
Mat4 Mat4MakeRotationZ(float a);

Vec4 Mat4MultiplyVec4(const Mat4& m, const Vec4& v);
Mat4 Mat4MultiplyMat4(const Mat4& a, const Mat4& b);

Mat4 Mat4LookAt(const Vec3& eye, const Vec3& target, const Vec3& up);

Mat4 Mat4MakePerspective(float fov, float aspect, float znear, float zfar);
Vec4 Mat4MultiplyVec4Project(const Mat4& m, const Vec4& v);