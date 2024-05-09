#pragma once

#include "../Utils/Mat4.h"
#include "../Utils/Vector.h"

struct Camera
{
	Vec3 mPosition;
	Vec3 mDirection;
	Vec3 mForward;
	float yaw;
};