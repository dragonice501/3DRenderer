#include "Managers/InputManager.h"
#include "Managers/GraphicsManager.h"

#include "upng/upng.h"

#include "Utils/Clipping.h"
#include "Utils/Mat4.h"
#include "Utils/Vector.h"

#include "Objects/Camera.h"
#include "Objects/Light.h"
#include "Objects/Mesh.h"

#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <iostream>

#include <SDL_image.h>

bool mIsRunning = false;
int mPreviousFrameTime = 0;
float mDeltaTime = 0.0f;

Mat4 mWorldMatrix;
Mat4 mViewMatrix;
Mat4 mPerspecitveMatrix;

Camera mCamera =
{
	{0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 1.0f}
};
Light mLight =
{
	{ 0.0f, 0.0f, 1.0f }
};

const int MAX_NUM_MESHES = 10;
Mesh mMeshes[MAX_NUM_MESHES];
int mMeshCount = 0;

int mNumTrianglesToRender = 0;
const int mMaxMeshTriangles = 8192;
Triangle mTrianglesToRender[mMaxMeshTriangles];

const int NUM_PLANES = 6;
Plane mFrustumPlanes[NUM_PLANES];

void LoadOBJData(const std::string& filePath, Mesh& mesh)
{
	std::string basePath = SDL_GetBasePath();
	std::ifstream file(basePath + filePath);
	std::string type;

	std::vector<Tex2> textureCoordinates;

	while (file >> type)
	{
		if (type == "#")
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (type == "mtllib")
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (type == "o")
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (type == "v")
		{
			Vec3 vertex;
			file >> vertex.x >> vertex.y >> vertex.z;
			mesh.vertices.push_back(vertex);
		}
		else if (type == "vt")
		{
			Tex2 vertexTexture;
			file >> vertexTexture.u >> vertexTexture.v;

			textureCoordinates.push_back(vertexTexture);
		}
		else if (type == "vn")
		{
			Vec3 vertexNormal;
			file >> vertexNormal.x >> vertexNormal.y >> vertexNormal.z;
		}
		else if (type == "g")
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (type == "usemtl")
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (type == "s")
		{
			file.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		}
		else if (type == "f")
		{
			Face face;

			int vertexIndeces[3];
			int textureIndeces[3];
			int normalIndeces[3];
			
			std::string vertexAData;
			std::string vertexBData;
			std::string vertexCData;
			
			file >> vertexAData >> vertexBData >> vertexCData;

			sscanf_s(vertexAData.c_str(), "%d/%d/%d", &vertexIndeces[0], &textureIndeces[0], &normalIndeces[0]);
			sscanf_s(vertexBData.c_str(), "%d/%d/%d", &vertexIndeces[1], &textureIndeces[1], &normalIndeces[1]);
			sscanf_s(vertexCData.c_str(), "%d/%d/%d", &vertexIndeces[2], &textureIndeces[2], &normalIndeces[2]);

			face.a = vertexIndeces[0] - 1;
			face.b = vertexIndeces[1] - 1;
			face.c = vertexIndeces[2] - 1;
			face.aUV = textureCoordinates[textureIndeces[0] - 1];
			face.bUV = textureCoordinates[textureIndeces[1] - 1];
			face.cUV = textureCoordinates[textureIndeces[2] - 1];
			face.color = 0xFFFFFFFF;

			mesh.faces.push_back(face);
		}
	}
}

void LoadPNGTextureData(const std::string& filePath, Mesh& mesh)
{
	std::string basePath = SDL_GetBasePath();
	std::string path = basePath + filePath;
	upng_t* pngImage = upng_new_from_file(path.c_str());
	if (pngImage != NULL)
	{
		upng_decode(pngImage);
		if (upng_get_error(pngImage) == UPNG_EOK)
		{
			mesh.texture = pngImage;
		}
	}
}

void InitFrustumPlanes(float fovX, float fovY, float zNear, float zFar)
{
	float cosHalfFOVX = cos(fovX / 2.0f);
	float sinHalfFOVX = sin(fovX / 2.0f);

	float cosHalfFOVY = cos(fovY / 2.0f);
	float sinHalfFOVY = sin(fovY / 2.0f);

	Vec3 origin = { 0.0f, 0.0f, 0.0f };

	mFrustumPlanes[FP_LEFT_FRUSTUM].point = origin;
	mFrustumPlanes[FP_LEFT_FRUSTUM].normal.x = cosHalfFOVX;
	mFrustumPlanes[FP_LEFT_FRUSTUM].normal.y = 0.0f;
	mFrustumPlanes[FP_LEFT_FRUSTUM].normal.z = sinHalfFOVX;

	mFrustumPlanes[FP_RIGHT_FRUSTUM].point = origin;
	mFrustumPlanes[FP_RIGHT_FRUSTUM].normal.x = -cosHalfFOVX;
	mFrustumPlanes[FP_RIGHT_FRUSTUM].normal.y = 0.0f;
	mFrustumPlanes[FP_RIGHT_FRUSTUM].normal.z = sinHalfFOVX;

	mFrustumPlanes[FP_TOP_FRUSTUM].point = origin;
	mFrustumPlanes[FP_TOP_FRUSTUM].normal.x = 0.0f;
	mFrustumPlanes[FP_TOP_FRUSTUM].normal.y = -cosHalfFOVY;
	mFrustumPlanes[FP_TOP_FRUSTUM].normal.z = sinHalfFOVY;

	mFrustumPlanes[FP_BOTTOM_FRUSTUM].point = origin;
	mFrustumPlanes[FP_BOTTOM_FRUSTUM].normal.x = 0.0f;
	mFrustumPlanes[FP_BOTTOM_FRUSTUM].normal.y = cosHalfFOVY;
	mFrustumPlanes[FP_BOTTOM_FRUSTUM].normal.z = sinHalfFOVY;

	mFrustumPlanes[FP_NEAR_FRUSTUM].point = { 0.0f, 0.0f, zNear };
	mFrustumPlanes[FP_NEAR_FRUSTUM].normal.x = 0.0f;
	mFrustumPlanes[FP_NEAR_FRUSTUM].normal.y = 0.0f;
	mFrustumPlanes[FP_NEAR_FRUSTUM].normal.z = 1.0f;

	mFrustumPlanes[FP_FAR_FRUSTUM].point = { 0.0f, 0.0f, zFar };
	mFrustumPlanes[FP_FAR_FRUSTUM].normal.x = 0.0f;
	mFrustumPlanes[FP_FAR_FRUSTUM].normal.y = 0.0f;
	mFrustumPlanes[FP_FAR_FRUSTUM].normal.z = -1.0f;
}

void LoadMesh(const std::string& filePath, const std::string& pngPath, const Vec3& scale, const Vec3& translation, const Vec3& rotation)
{
	LoadOBJData(filePath, mMeshes[mMeshCount]);
	LoadPNGTextureData(pngPath, mMeshes[mMeshCount]);

	mMeshes[mMeshCount].scale = scale;
	mMeshes[mMeshCount].translation = translation;
	mMeshes[mMeshCount].rotation = rotation;

	mMeshCount++;
}

void Setup()
{
	float aspectY = static_cast<float>(GraphicsManager::WindowHeight()) / static_cast<float>(GraphicsManager::WindowWidth());
	float aspectX = static_cast<float>(GraphicsManager::WindowWidth()) / static_cast<float>(GraphicsManager::WindowHeight());
	float fovY = M_PI / 3.0f;
	float fovX = 2.0f * atan(tan(fovY / 2.0f) * aspectX);
	float zNear = 0.1f;
	float zFar = 20.0f;

	mPerspecitveMatrix = Mat4MakePerspective(fovY, aspectY, zNear, zFar);

	InitFrustumPlanes(fovX, fovY, zNear, zFar);

	LoadMesh("assets\\efa.obj", "assets\\efa.png", Vec3New(1, 1, 1), Vec3New(3, 0, 5), Vec3New(0, 0, 0));
	LoadMesh("assets\\f22.obj", "assets\\f22.png", Vec3New(1, 1, 1), Vec3New(-3, 0, 5), Vec3New(0, 0, 0));
}

void Input()
{
	InputManager::Update(0.0f);

	if (InputManager::KeyHeldW())
	{
		mCamera.mPosition.y += 3.0f * mDeltaTime;
	}
	else if (InputManager::KeyHeldS())
	{
		mCamera.mPosition.y -= 3.0f * mDeltaTime;
	}

	if (InputManager::KeyHeldD())
	{
		mCamera.yaw += 1.0f * mDeltaTime;
	}
	else if (InputManager::KeyHeldA())
	{
		mCamera.yaw -= 1.0f * mDeltaTime;
	}

	if (InputManager::KeyPressedSpace())
	{
		mCamera.mForward = Vec3Multiply(mCamera.mDirection, 5.0f * mDeltaTime);
		mCamera.mPosition = Vec3Add(mCamera.mPosition, mCamera.mForward);
	}

	if (InputManager::KeyPressedB()) GraphicsManager::SetBackfaceCulling(!GraphicsManager::GetBackfaceCullingEnabled());

	if (InputManager::KeyPressedEscape()) mIsRunning = false;

	if (InputManager::KeyPressedOne()) GraphicsManager::SetRenderMethod(GraphicsManager::EWireframe);
	else if (InputManager::KeyPressedTwo()) GraphicsManager::SetRenderMethod(GraphicsManager::EWireframeVertex);
	else if (InputManager::KeyPressedThree()) GraphicsManager::SetRenderMethod(GraphicsManager::ETriangleFill);
	else if (InputManager::KeyPressedFour()) GraphicsManager::SetRenderMethod(GraphicsManager::ETriangleFillWireframe);
	else if (InputManager::KeyPressedFive()) GraphicsManager::SetRenderMethod(GraphicsManager::ETriangleTextured);
	else if (InputManager::KeyPressedSix()) GraphicsManager::SetRenderMethod(GraphicsManager::ETriangleTexturedWireframe);
}

void Update()
{
	// Delta Time
	int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - mPreviousFrameTime);
	if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME)
	{
		SDL_Delay(timeToWait);
	}
	mDeltaTime = (SDL_GetTicks() - mPreviousFrameTime) / 1000.0f;

	mPreviousFrameTime = SDL_GetTicks();

	mNumTrianglesToRender = 0;
	for (int meshIndex = 0; meshIndex < mMeshCount; meshIndex++)
	{
		Mesh& mesh = mMeshes[meshIndex];

		mesh.rotation.x += 0.01f;

		// Transformation Matrices Creation
		Mat4 scaleMatrix = Mat4MakeScale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
		Mat4 rotationXMatrix = Mat4MakeRotationX(mesh.rotation.x);
		Mat4 rotationYMatrix = Mat4MakeRotationY(mesh.rotation.y);
		Mat4 rotationZMatrix = Mat4MakeRotationZ(mesh.rotation.z);
		Mat4 translationMatrix = Mat4MakeTranslation(mesh.translation.x, mesh.translation.y, mesh.translation.z);

		// Create View Matrix
		Vec3 up = { 0.0f, 1.0f, 0.0f };
		Vec3 target = { 0.0f, 0.0f, 1.0f };
		Mat4 cameraYawRotation = Mat4MakeRotationY(mCamera.yaw);
		mCamera.mDirection = Vec3FromVec4(Mat4MultiplyVec4(cameraYawRotation, Vec4FromVec3(target)));

		target = Vec3Add(mCamera.mPosition, mCamera.mDirection);

		mViewMatrix = Mat4LookAt(mCamera.mPosition, target, up);

		// 
		Vec3 faceVertices[3];
		Triangle triangleToRender;
		Vec4 transformedVertices[3];
		Vec4 projectedPoints[3];
		float depth;
		float lightIntensity;

		// Loop Meshes
		for (int i = 0; i < mesh.faces.size(); i++)
		{
			Face& face = mesh.faces[i];

			faceVertices[0] = mesh.vertices[face.a];
			faceVertices[1] = mesh.vertices[face.b];
			faceVertices[2] = mesh.vertices[face.c];

			// Transform from Model Space into World Space
			for (int j = 0; j < 3; j++)
			{
				Vec4 transformedVertex = Vec4FromVec3(faceVertices[j]);

				mWorldMatrix = Mat4Identity();
				mWorldMatrix = Mat4MultiplyMat4(scaleMatrix, mWorldMatrix);
				mWorldMatrix = Mat4MultiplyMat4(rotationXMatrix, mWorldMatrix);
				mWorldMatrix = Mat4MultiplyMat4(rotationYMatrix, mWorldMatrix);
				mWorldMatrix = Mat4MultiplyMat4(rotationZMatrix, mWorldMatrix);
				mWorldMatrix = Mat4MultiplyMat4(translationMatrix, mWorldMatrix);

				transformedVertex = Mat4MultiplyVec4(mWorldMatrix, transformedVertex);
				transformedVertex = Mat4MultiplyVec4(mViewMatrix, transformedVertex);

				transformedVertices[j] = transformedVertex;
			}

			// Backface Culling
			Vec3 ab = Vec3Subtract(Vec3FromVec4(transformedVertices[1]), Vec3FromVec4(transformedVertices[0]));
			Vec3 ac = Vec3Subtract(Vec3FromVec4(transformedVertices[2]), Vec3FromVec4(transformedVertices[0]));

			Vec3 normal = Vec3Cross(ab, ac);
			Vec3Normalize(normal);

			Vec3 origin = { 0.0f, 0.0f, 0.0f };
			Vec3 cameraRay = Vec3Subtract(origin, Vec3FromVec4(transformedVertices[0]));

			if (GraphicsManager::GetBackfaceCullingEnabled())
			{
				if (Vec3Dot(normal, cameraRay) < 0.0f) continue;
			}

			// Clipping
			Polygon polygon = CreatePolygonFromTriange(
				Vec3FromVec4(transformedVertices[0]),
				Vec3FromVec4(transformedVertices[1]),
				Vec3FromVec4(transformedVertices[2]),
				face.aUV, face.bUV, face.cUV);

			ClipPolygon(polygon, mFrustumPlanes);

			// Triangulate Polygon
			Triangle clippedTriangles[MAX_NUM_POLY_TRIANGLES];
			int numClippedTriangles = 0;

			CreateTrianglesFromPolygon(polygon, clippedTriangles, numClippedTriangles);

			// Perspective Projection
			for (int t = 0; t < numClippedTriangles; t++)
			{
				Triangle clippedTriangle = clippedTriangles[t];

				for (int j = 0; j < 3; j++)
				{
					projectedPoints[j] = Mat4MultiplyVec4Project(mPerspecitveMatrix, clippedTriangle.points[j]);

					projectedPoints[j].x *= GraphicsManager::WindowWidth() * 0.5f;
					projectedPoints[j].y *= GraphicsManager::WindowHeight() * 0.5f;

					projectedPoints[j].y *= -1;

					projectedPoints[j].x += GraphicsManager::WindowWidth() * 0.5f;
					projectedPoints[j].y += GraphicsManager::WindowHeight() * 0.5f;
				}

				lightIntensity = -Vec3Dot(normal, mLight.direction);

				uint32_t color = GraphicsManager::ApplyLightIntensity(face.color, lightIntensity);

				triangleToRender =
				{
					{
						{ projectedPoints[0].x, projectedPoints[0].y, projectedPoints[0].z, projectedPoints[0].w },
						{ projectedPoints[1].x, projectedPoints[1].y, projectedPoints[1].z, projectedPoints[1].w },
						{ projectedPoints[2].x, projectedPoints[2].y, projectedPoints[2].z, projectedPoints[2].w },
					},
					{
						{ clippedTriangle.texCoords[0].u, clippedTriangle.texCoords[0].v },
						{ clippedTriangle.texCoords[1].u, clippedTriangle.texCoords[1].v },
						{ clippedTriangle.texCoords[2].u, clippedTriangle.texCoords[2].v },
					},
					color,
					mesh.texture
				};

				if (mNumTrianglesToRender < mMaxMeshTriangles)
				{
					mTrianglesToRender[mNumTrianglesToRender] = triangleToRender;
					mNumTrianglesToRender++;
				}
			}
		}
	}
}

void Render()
{
	GraphicsManager::ClearColorBuffer();
	GraphicsManager::ClearZBuffer();

	GraphicsManager::DrawGrid();

	for (int i = 0; i < mNumTrianglesToRender; i++)
	{
		Triangle& triangle = mTrianglesToRender[i];

		if (GraphicsManager::GetRenderMode() == GraphicsManager::ETriangleFill ||
			GraphicsManager::GetRenderMode() == GraphicsManager::ETriangleFillWireframe)
		{
			GraphicsManager::DrawTriangleFilled(
				triangle.points[0].x, triangle.points[0].y ,triangle.points[0].z, triangle.points[0].w,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w,
				triangle.color);
		}

		if (GraphicsManager::GetRenderMode() == GraphicsManager::ETriangleTextured ||
			GraphicsManager::GetRenderMode() == GraphicsManager::ETriangleTexturedWireframe)
		{
			GraphicsManager::DrawTriangleTextured(
				triangle.points[0].x, triangle.points[0].y, triangle.points[0].z, triangle.points[0].w, triangle.texCoords[0].u, triangle.texCoords[0].v,
				triangle.points[1].x, triangle.points[1].y, triangle.points[1].z, triangle.points[1].w, triangle.texCoords[1].u, triangle.texCoords[1].v,
				triangle.points[2].x, triangle.points[2].y, triangle.points[2].z, triangle.points[2].w, triangle.texCoords[2].u, triangle.texCoords[2].v,
				triangle.texture
			);
		}

		if (GraphicsManager::GetRenderMode() == GraphicsManager::EWireframe ||
			GraphicsManager::GetRenderMode() == GraphicsManager::EWireframeVertex ||
			GraphicsManager::GetRenderMode() == GraphicsManager::ETriangleFillWireframe ||
			GraphicsManager::GetRenderMode() == GraphicsManager::ETriangleTexturedWireframe)
		{
			GraphicsManager::DrawTriangle(
				triangle.points[0].x, triangle.points[0].y,
				triangle.points[1].x, triangle.points[1].y,
				triangle.points[2].x, triangle.points[2].y,
				0xFFFFFFFF);
		}

		if (GraphicsManager::GetRenderMode() == GraphicsManager::EWireframeVertex)
		{
			GraphicsManager::DrawRectangle(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6, 0xFFFFFF00);
			GraphicsManager::DrawRectangle(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6, 0xFFFFFF00);
			GraphicsManager::DrawRectangle(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6, 0xFFFFFF00);
		}
	}

	GraphicsManager::RenderColorBuffer();
	GraphicsManager::PresetRender();
}

int main(int argc, char* argv[])
{
	mIsRunning = GraphicsManager::InitializeWindow();

	Setup();

	while (mIsRunning)
	{
		Input();
		Update();
		Render();
	}

	for (int i = 0; i < mMeshCount; i++)
	{
		upng_free(mMeshes[i].texture);
	}

	GraphicsManager::DestroyWindow();

	return 0;
}