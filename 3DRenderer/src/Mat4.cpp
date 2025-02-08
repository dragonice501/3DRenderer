#include "Mat4.h"

#include <math.h>

Mat4 Mat4Identity()
{
	Mat4 identity =
	{
		{
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	return identity;
}

Mat4 Mat4MakeScale(float sx, float sy, float sz)
{
	Mat4 m = Mat4Identity();
	m.mat[0][0] = sx;
	m.mat[1][1] = sy;
	m.mat[2][2] = sz;

	return m;
}

Mat4 Mat4MakeTranslation(float tx, float ty, float tz)
{
	Mat4 m = Mat4Identity();
	m.mat[0][3] = tx;
	m.mat[1][3] = ty;
	m.mat[2][3] = tz;

	return m;
}

Mat4 Mat4MakeRotationX(float a)
{
	Mat4 m = Mat4Identity();
	m.mat[1][1] = cosf(a);
	m.mat[1][2] = -sinf(a);
	m.mat[2][1] = sinf(a);
	m.mat[2][2] = cosf(a);

	return m;
}

Mat4 Mat4MakeRotationY(float a)
{
	Mat4 m = Mat4Identity();
	m.mat[0][0] = cosf(a);
	m.mat[0][2] = sinf(a);
	m.mat[2][0] = -sinf(a);
	m.mat[2][2] = cosf(a);

	return m;
}

Mat4 Mat4MakeRotationZ(float a)
{
	Mat4 m = Mat4Identity();
	m.mat[0][0] = cosf(a);
	m.mat[0][1] = -sinf(a);
	m.mat[1][0] = sinf(a);
	m.mat[1][1] = cosf(a);

	return m;
}

Vec4 Mat4MultiplyVec4(const Mat4& m, const Vec4& v)
{
	Vec4 result =
	{
		m.mat[0][0] * v.x + m.mat[0][1] * v.y + m.mat[0][2] * v.z + m.mat[0][3] * v.w,
		m.mat[1][0] * v.x + m.mat[1][1] * v.y + m.mat[1][2] * v.z + m.mat[1][3] * v.w,
		m.mat[2][0] * v.x + m.mat[2][1] * v.y + m.mat[2][2] * v.z + m.mat[2][3] * v.w,
		m.mat[3][0] * v.x + m.mat[3][1] * v.y + m.mat[3][2] * v.z + m.mat[3][3] * v.w,
	};

	return result;
}

Mat4 Mat4MultiplyMat4(const Mat4& a, const Mat4& b)
{
	Mat4 m = Mat4Identity();

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			m.mat[i][j] = a.mat[i][0] * b.mat[0][j] + a.mat[i][1] * b.mat[1][j] + a.mat[i][2] * b.mat[2][j] + a.mat[i][3] * b.mat[3][j];
		}
	}

	return m;
}

Mat4 Mat4LookAt(const Vec3& eye, const Vec3& target, const Vec3& up)
{
	Vec3 z = Vec3Subtract(target, eye);
	Vec3Normalize(z);
	Vec3 x = Vec3Cross(up, z);
	Vec3Normalize(x);
	Vec3 y = Vec3Cross(z, x);

	Mat4 viewMatrix =
	{
		{
			{x.x, x.y, x.z, -Vec3Dot(x, eye)},
			{y.x, y.y, y.z, -Vec3Dot(y, eye)},
			{z.x, z.y, z.z, -Vec3Dot(z, eye)},
			{0.0f, 0.0f, 0.0f, 1.0f}
		}
	};

	return viewMatrix;
}

Mat4 Mat4MakePerspective(float fov, float aspect, float znear, float zfar)
{
	Mat4 m = { {{0.0f}} };

	m.mat[0][0] = aspect * (1 / tanf(fov / 2));
	m.mat[1][1] = 1 / tanf(fov / 2);
	m.mat[2][2] = zfar / (zfar - znear);
	m.mat[2][3] = (-zfar * znear) / (zfar - znear);
	m.mat[3][2] = 1.0f;

	return m;
}

Vec4 Mat4MultiplyVec4Project(const Mat4& m, const Vec4& v)
{
	Vec4 result = Mat4MultiplyVec4(m, v);

	if (result.w != 0.0f)
	{
		result.x /= result.w;
		result.y /= result.w;
		result.z /= result.w;
	}

	return result;
}
