#include "Clipping.h"

Polygon CreatePolygonFromTriange(const Vec3& v0, const Vec3& v1, const Vec3& v2, const Tex2 uv0, const Tex2 uv1, const Tex2 uv2)
{
    Polygon polygon =
    {
        { v0, v1, v2 },
        { uv0, uv1, uv2 },
        3
    };

    return polygon;
}

void ClipPolygon(Polygon& polygon, const Plane* clippingPlanes)
{
    ClipPolygonAgainstPlane(polygon, FP_LEFT_FRUSTUM, clippingPlanes);
    ClipPolygonAgainstPlane(polygon, FP_RIGHT_FRUSTUM, clippingPlanes);
    ClipPolygonAgainstPlane(polygon, FP_TOP_FRUSTUM, clippingPlanes);
    ClipPolygonAgainstPlane(polygon, FP_BOTTOM_FRUSTUM, clippingPlanes);
    ClipPolygonAgainstPlane(polygon, FP_NEAR_FRUSTUM, clippingPlanes);
    ClipPolygonAgainstPlane(polygon, FP_FAR_FRUSTUM, clippingPlanes);
}

void ClipPolygonAgainstPlane(Polygon& polygon, EFrustumPlane plane, const Plane* clippingPlanes)
{
    Vec3 point = clippingPlanes[plane].point;
    Vec3 normal = clippingPlanes[plane].normal;

    Vec3 insideVertices[MAX_NUM_POLY_VERTICES];
    Tex2 insideTexCoords[MAX_NUM_POLY_VERTICES];
    int numInsideVertices = 0;

    Vec3* currentVertex = &polygon.vertices[0];
    Tex2* currentTexCoord = &polygon.texCoords[0];

    Vec3* previousVertex = &polygon.vertices[polygon.numVertices - 1];
    Tex2* previousTexCoord = &polygon.texCoords[polygon.numVertices - 1];

    float currentDot = 0;
    float previousDot = Vec3Dot(clippingPlanes[plane].normal, Vec3Subtract(*previousVertex, clippingPlanes[plane].point));

    while (currentVertex != &polygon.vertices[polygon.numVertices])
    {
        currentDot = Vec3Dot(clippingPlanes[plane].normal, Vec3Subtract(*currentVertex, clippingPlanes[plane].point));

        if (currentDot * previousDot < 0)
        {
            float t = previousDot / (previousDot - currentDot);

            Vec3 intersectionPoint =
            {
                Lerpf(previousVertex->x, currentVertex->x, t),
                Lerpf(previousVertex->y, currentVertex->y, t),
                Lerpf(previousVertex->z, currentVertex->z, t)
            };

            Tex2 interpolatedCoord =
            {
                Lerpf(previousTexCoord->u, currentTexCoord->u, t),
                Lerpf(previousTexCoord->v, currentTexCoord->v, t)
            };

            insideVertices[numInsideVertices] = intersectionPoint;
            insideTexCoords[numInsideVertices] = interpolatedCoord;
            numInsideVertices++;
        }

        if (currentDot > 0)
        {
            insideVertices[numInsideVertices] = Vec3Clone(currentVertex);
            insideTexCoords[numInsideVertices] = Tex2Clone(currentTexCoord);
            numInsideVertices++;
        }

        previousDot = currentDot;
        previousVertex = currentVertex;
        previousTexCoord = currentTexCoord;
        currentVertex++;
        currentTexCoord++;
    }

    for (int i = 0; i < numInsideVertices; i++)
    {
        polygon.vertices[i] = insideVertices[i];
        polygon.texCoords[i] = insideTexCoords[i];
    }
    polygon.numVertices = numInsideVertices;
}

void CreateTrianglesFromPolygon(const Polygon& polygon, Triangle* triangles, int& numTriangles)
{
    for (int i = 0; i < polygon.numVertices - 2; i++)
    {
        int index1 = i + 1;
        int index2 = i + 2;

        triangles[i].points[0] = Vec4FromVec3(polygon.vertices[0]);
        triangles[i].points[1] = Vec4FromVec3(polygon.vertices[index1]);
        triangles[i].points[2] = Vec4FromVec3(polygon.vertices[index2]);

        triangles[i].texCoords[0] = polygon.texCoords[0];
        triangles[i].texCoords[1] = polygon.texCoords[index1];
        triangles[i].texCoords[2] = polygon.texCoords[index2];
    }

    numTriangles = polygon.numVertices - 2;
}

float Lerpf(const float a, const float b, const float t)
{
    return a + t * (b - a);
}
