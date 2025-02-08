#pragma once

#include "Vector.h"

#include "upng.h"

#include <stdint.h>
#include <SDL.h>

#define FPS 30
#define FRAME_TARGET_TIME 1000 / FPS

class GraphicsManager
{
public:
	enum ERenderMode
	{
		EWireframe,
		EWireframeVertex,
		ETriangleFill,
		ETriangleFillWireframe,
		ETriangleTextured,
		ETriangleTexturedWireframe
	};

public:
	static bool InitializeWindow();
	static void DestroyWindow();

	static SDL_Renderer* GetRenderer() { return mRenderer; }
	
	inline static const int WindowWidth() { return mWindowWidth; }
	inline static const int WindowHeight() { return mWindowHeight; }

	inline static ERenderMode GetRenderMode() { return mRenderMode; }
	inline static bool GetBackfaceCullingEnabled() { return mBackfaceCulling; }

	inline static void SetRenderMethod(const ERenderMode mode) { mRenderMode = mode; }
	inline static void SetBackfaceCulling(const bool enable) { mBackfaceCulling = enable; }

	static void ClearColorBuffer(uint32_t color = 0xFF000000);
	static void ClearZBuffer();

	static void DrawPixel(int x, int y, uint32_t color = 0xFFFFFF00);
	static void DrawGrid();
	static void DrawRectangle(int x, int y, int w, int h, uint32_t color = 0xFFFFFF00);
	static void DrawRectangle(const Vec2& vector, int w, int h, uint32_t color = 0xFFFFFF00);

	static void DrawLineDDA(int x0, int y0, int x1, int y1, uint32_t color = 0xFFFFFF00);
	static void DrawLineDDA(const Vec2& v0, const Vec2& v1, uint32_t color = 0xFFFFFF00);
	static void DrawLineBresenham(int x0, int y0, int x1, int y1, uint32_t color = 0xFFFFFF00);
	static void DrawLineBresenham(const Vec2& p0, const Vec2& p1, uint32_t color = 0xFFFFFF00);

	static void DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color = 0xFFFFFF00);
	static void DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, uint32_t color = 0xFFFFFF00);
	static void DrawTriangleFilled(
		int x0, int y0, float z0, float w0,
		int x1, int y1, float z1, float w1,
		int x2, int y2, float z2, float w2,
		uint32_t color = 0xFFFFFF00);
	static void DrawTriangleFilled(Vec2& p0, Vec2& p1, Vec2& p2, uint32_t color = 0xFFFFFF00);
	static void DrawTrianglePixel(int x, int y, const Vec4& p0, const Vec4& p1, const Vec4& p2, uint32_t color = 0xFFFFFFFF);
	static void DrawTriangleTextured(
		int x0, int y0, float z0, float w0, float u0, float v0,
		int x1, int y1, float z1, float w1, float u1, float v1,
		int x2, int y2, float z2, float w2, float u2, float v2,
		upng_t* texture);

	static void DrawTexel(
		int x, int y, upng_t* texture,
		const Vec4& a, const Vec4& b, const Vec4& c,
		const Vec2& uv0, const Vec2& uv1, const Vec2& uv2);

	static void RenderColorBuffer();
	static void PresetRender();

	static uint32_t ApplyLightIntensity(uint32_t color, float intensity);

private:
	GraphicsManager() {}
	~GraphicsManager() {}

	static void SwapInt(int& a, int& b);
	static void SwapFloat(float& a, float& b);
	static void SwapVec2(Vec2& a, Vec2& b);

	static void FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color = 0xFFFFFF00);
	static void FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color = 0xFFFFFF00);

	static Vec3 GetBarycentricWeights(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p);
	static uint32_t GetBarycentricColor(float alpha, float beta, float gamma);

	static SDL_Window* mWindow;
	static SDL_Renderer* mRenderer;
	
	static int mWindowWidth;
	static int mWindowHeight;

	static uint32_t* mColorBuffer;
	static SDL_Texture* mScreenTexture;

	static float* mZBuffer;

	static ERenderMode mRenderMode;
	static bool mBackfaceCulling;
};