#pragma once

#include "Vector.h"
#include "../Objects/Mesh.h"

const int MAX_NUM_POLY_VERTICES = 10;
const int MAX_NUM_POLY_TRIANGLES = 10;

enum EFrustumPlane
{
	FP_LEFT_FRUSTUM,
	FP_RIGHT_FRUSTUM,
	FP_TOP_FRUSTUM,
	FP_BOTTOM_FRUSTUM,	
	FP_NEAR_FRUSTUM,
	FP_FAR_FRUSTUM
};

struct Plane
{
	Vec3 point;
	Vec3 normal;
};

struct Polygon
{
	Vec3 vertices[MAX_NUM_POLY_VERTICES];
	Tex2 texCoords[MAX_NUM_POLY_VERTICES];
	int numVertices;
};

Polygon CreatePolygonFromTriange(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Tex2 uv0, const Tex2 uv1, const Tex2 uv2);
void ClipPolygon(Polygon& polygon, const Plane* clippingPlanes);
void ClipPolygonAgainstPlane(Polygon& polygon, EFrustumPlane plane, const Plane* clippingPlanes);

void CreateTrianglesFromPolygon(const Polygon& polygon, Triangle* triangles, int& numTriangles);

float Lerpf(const float a, const float b, const float t);