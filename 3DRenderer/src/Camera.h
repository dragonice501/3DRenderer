#pragma once

#include "Mat4.h"
#include "Vector.h"

struct Camera
{
	Vec3 mPosition;
	Vec3 mDirection;
	Vec3 mForward;
	float yaw;
	float pitch;
};