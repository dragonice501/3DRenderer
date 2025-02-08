#pragma once

#include "Vector.h"
#include "Texture.h"

#include "upng.h"

#include <vector>
#include <stdint.h>

struct Triangle
{
	Vec4 points[3];
	Tex2 texCoords[3];
	uint32_t color;
	upng_t* texture;
};

struct Face
{
	int a;
	int b;
	int c;
	Tex2 aUV;
	Tex2 bUV;
	Tex2 cUV;
	uint32_t color;
};

struct Mesh
{
	std::vector<Vec3> vertices;
	std::vector<Face> faces;
	upng_t* texture;
	
	Vec3 rotation = { 0.0f, 0.0f, 0.0f };
	Vec3 scale = { 1.0f, 1.0f, 1.0f };
	Vec3 translation = { 0.0f, 0.0f, 0.0f };
};