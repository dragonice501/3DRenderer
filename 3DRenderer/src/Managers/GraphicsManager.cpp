#include "GraphicsManager.h"

#include <iostream>

SDL_Window* GraphicsManager::mWindow = nullptr;
SDL_Renderer* GraphicsManager::mRenderer = nullptr;
uint32_t* GraphicsManager::mColorBuffer = nullptr;
SDL_Texture* GraphicsManager::mScreenTexture = nullptr;
float* GraphicsManager::mZBuffer = nullptr;

int GraphicsManager::mWindowWidth = 1920 / 2;
int GraphicsManager::mWindowHeight = 1080 / 2;

GraphicsManager::ERenderMode GraphicsManager::mRenderMode = GraphicsManager::EWireframe;
bool GraphicsManager::mBackfaceCulling = true;

bool GraphicsManager::InitializeWindow()
{
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
	{
		std::cout << "Error initializing SDL.\n";
		return false;
	}

	SDL_DisplayMode mode;
	SDL_GetCurrentDisplayMode(0, &mode);
	mWindow = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w, mode.h, SDL_WINDOW_BORDERLESS);
	//mWindow = SDL_CreateWindow(NULL, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, mode.w / 2, mode.h / 2, SDL_WINDOW_BORDERLESS);
	if (!mWindow)
	{
		std::cout << "Error creating window.\n";
		return false;
	}
	//SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);

	mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
	if (!mRenderer)
	{
		std::cout << "Error creating renderer.\n";
		return false;
	}

	mColorBuffer = new uint32_t[mWindowWidth * mWindowHeight];
	mScreenTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, mWindowWidth, mWindowHeight);

	mZBuffer = new float[mWindowWidth * mWindowHeight];

	return true;
}

void GraphicsManager::DestroyWindow()
{
	delete mColorBuffer;
	delete mZBuffer;

	SDL_DestroyRenderer(mRenderer);
	SDL_DestroyWindow(mWindow);
	SDL_Quit();
}

void GraphicsManager::ClearColorBuffer(uint32_t color)
{
	for (int i = 0; i < mWindowWidth * mWindowHeight; i++)
	{
		mColorBuffer[i] = color;
	}
}

void GraphicsManager::ClearZBuffer()
{
	for (int i = 0; i < mWindowWidth * mWindowHeight; i++)
	{
		mZBuffer[i] = 1.0f;
	}
}

void GraphicsManager::DrawPixel(int x, int y, uint32_t color)
{
	if (x < 0 || x >= mWindowWidth || y < 0 || y >= mWindowHeight) return;

	mColorBuffer[x + y * mWindowWidth] = color;
}

void GraphicsManager::DrawGrid()
{
	for (int y = 0; y < mWindowHeight; y += 10)
	{
		for (int x = 0; x < mWindowWidth; x += 10)
		{
			DrawPixel(x, y, 0xFF333333);
		}
	}
}

void GraphicsManager::DrawRectangle(int x, int y, int w, int h, uint32_t color)
{
	for (int i = y; i < y + h; i++)
	{
		for (int j = x; j < x + w; j++)
		{
			DrawPixel(j, i, color);
		}
	}
}

void GraphicsManager::DrawRectangle(const Vec2& vector, int w, int h, uint32_t color)
{
	for (int i = vector.y; i < vector.y + h; i++)
	{
		for (int j = vector.x; j < vector.x + w; j++)
		{
			DrawPixel(j, i, color);
		}
	}
}

void GraphicsManager::DrawLineDDA(int x0, int y0, int x1, int y1, uint32_t color)
{
	int deltaX = x1 - x0;
	int deltaY = y1 - y0;

	int longestSideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

	float xIncrement = deltaX / static_cast<float>(longestSideLength);
	float yIncrement = deltaY / static_cast<float>(longestSideLength);

	float currentX = x0;
	float currentY = y0;
	for (int i = 0; i <= longestSideLength; i++)
	{
		DrawPixel(round(currentX), round(currentY), color);
		currentX += xIncrement;
		currentY += yIncrement;
	}
}

void GraphicsManager::DrawLineDDA(const Vec2& v0, const Vec2& v1, uint32_t color)
{
	int deltaX = v1.x - v0.x;
	int deltaY = v1.y - v0.y;

	int longestSideLength = abs(deltaX) >= abs(deltaY) ? abs(deltaX) : abs(deltaY);

	float xIncrement = deltaX / static_cast<float>(longestSideLength);
	float yIncrement = deltaY / static_cast<float>(longestSideLength);

	float currentX = v0.x;
	float currentY = v0.y;
	for (int i = 0; i <= longestSideLength; i++)
	{
		DrawPixel(round(currentX), round(currentY));
		currentX += xIncrement;
		currentY += yIncrement;
	}
}

void GraphicsManager::DrawLineBresenham(int x0, int y0, int x1, int y1, uint32_t color)
{
	int m_new = 2 * (y1 - y0);
	int slope_error_new = m_new - (x1 - x0);
	for (int x = x0, y = y0; x <= x1; x++)
	{
		DrawPixel(x, y, color);
		// Add slope to increment angle formed 
		slope_error_new += m_new;

		// Slope error reached limit, time to 
		// increment y and update slope error. 
		if (slope_error_new >= 0) {
			y++;
			slope_error_new -= 2 * (x1 - x0);
		}
	}
}

void GraphicsManager::DrawLineBresenham(const Vec2& p0, const Vec2& p1, uint32_t color)
{
	int m_new = 2 * (p1.y - p0.y);
	int slope_error_new = m_new - (p1.x - p0.x);
	for (int x = p0.x, y = p0.y; x <= p1.x; x++)
	{
		DrawPixel(x, y, color);
		// Add slope to increment angle formed 
		slope_error_new += m_new;

		// Slope error reached limit, time to 
		// increment y and update slope error. 
		if (slope_error_new >= 0) {
			y++;
			slope_error_new -= 2 * (p1.x - p0.x);
		}
	}
}

void GraphicsManager::DrawTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	DrawLineDDA(x0, y0, x1, y1, color);
	DrawLineDDA(x1, y1, x2, y2, color);
	DrawLineDDA(x2, y2, x0, y0, color);
}

void GraphicsManager::DrawTriangle(const Vec2& v0, const Vec2& v1, const Vec2& v2, uint32_t color)
{
	DrawLineDDA(v0, v1, color);
	DrawLineDDA(v1, v2, color);
	DrawLineDDA(v2, v0, color);
}

void GraphicsManager::DrawTriangleFilled(
	int x0, int y0, float z0, float w0,
	int x1, int y1, float z1, float w1,
	int x2, int y2, float z2, float w2,
	uint32_t color)
{
	if (y0 > y1)
	{
		SwapInt(x0, x1);
		SwapInt(y0, y1);
		SwapFloat(z0, z1);
		SwapFloat(w0, w1);
	}
	if (y1 > y2)
	{
		SwapInt(x1, x2);
		SwapInt(y1, y2);
		SwapFloat(z1, z2);
		SwapFloat(w1, w2);
	}
	if (y0 > y1)
	{
		SwapInt(x0, x1);
		SwapInt(y0, y1);
		SwapFloat(z0, z1);
		SwapFloat(w0, w1);
	}

	Vec4 a = { static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(z0), static_cast<float>(w0) };
	Vec4 b = { static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(z1), static_cast<float>(w1) };
	Vec4 c = { static_cast<float>(x2), static_cast<float>(y2), static_cast<float>(z2), static_cast<float>(w2) };

	float slope1 = 0.0f;
	float slope2 = 0.0f;

	if (y1 - y0 != 0) slope1 = static_cast<float>(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) slope2 = static_cast<float>(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0)
	{
		for (int y = y0; y <= y1; y++)
		{
			int xStart = x1 + (y - y1) * slope1;
			int xEnd = x0 + (y - y0) * slope2;

			if (xEnd < xStart)
			{
				SwapInt(xEnd, xStart);
			}

			for (int x = xStart; x < xEnd; x++)
			{
				DrawTrianglePixel(x, y, a, b, c, color);
			}
		}
	}

	slope1 = 0.0f;

	if (y2 - y1 != 0) slope1 = static_cast<float>(x2 - x1) / abs(y2 - y1);

	if (y2 - y1 != 0)
	{
		for (int y = y1; y <= y2; y++)
		{
			int xStart = x1 + (y - y1) * slope1;
			int xEnd = x0 + (y - y0) * slope2;

			if (xEnd < xStart)
			{
				SwapInt(xEnd, xStart);
			}

			for (int x = xStart; x < xEnd; x++)
			{
				DrawTrianglePixel(x, y, a, b, c, color);
			}
		}
	}
}

void GraphicsManager::DrawTriangleFilled(Vec2& p0, Vec2& p1, Vec2& p2, uint32_t color)
{
	if (p0.y > p1.y)
	{
		SwapVec2(p0, p1);
	}
	if (p1.y > p2.y)
	{
		SwapVec2(p1, p2);
	}
	if (p0.y > p1.y)
	{
		SwapVec2(p0, p1);
	}

	if (p1.y == p2.y)
	{
		if (p1.x > p2.x)
			SwapFloat(p1.x, p2.x);

		FillFlatBottomTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
	}
	else if (p0.y == p1.y)
	{
		if (p0.x > p1.x)
			SwapFloat(p0.x, p1.x);

		FillFlatTopTriangle(p0.x, p0.y, p1.x, p1.y, p2.x, p2.y, color);
	}
	else
	{
		int mY = p1.y;
		int mX = (((p2.x - p0.x) * (p1.y - p0.y)) / (p2.y - p0.y)) + p0.x;

		if (mX < p1.x)
		{
			int tempX = mX;
			mX = p1.x;
			p1.x = tempX;
		}

		FillFlatBottomTriangle(p0.x, p0.y, p1.x, p1.y, mX, mY, color);
		FillFlatTopTriangle(p0.x, p0.y, mX, mY, p2.x, p2.y, color);
	}
}

void GraphicsManager::DrawTrianglePixel(int x, int y, const Vec4& p0, const Vec4& p1, const Vec4& p2, uint32_t color)
{
	Vec2 p = { static_cast<float>(x), static_cast<float>(y) };
	Vec2 pA = { p0.x, p0.y };
	Vec2 pB = { p1.x, p1.y };
	Vec2 pC = { p2.x, p2.y };

	Vec3 weights = GetBarycentricWeights(pA, pB, pC, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolatedReciprocalW;

	interpolatedReciprocalW = (1 / p0.w) * alpha + (1 / p1.w) * beta + (1 / p2.w) * gamma;

	interpolatedReciprocalW = 1.0f - interpolatedReciprocalW;

	if (interpolatedReciprocalW < mZBuffer[x + y * mWindowWidth])
	{
		DrawPixel(x, y, color);

		mZBuffer[x + y * mWindowWidth] = interpolatedReciprocalW;
	}
}

void GraphicsManager::DrawTriangleTextured(
	int x0, int y0, float z0, float w0, float u0, float v0,
	int x1, int y1, float z1, float w1, float u1, float v1,
	int x2, int y2, float z2, float w2, float u2, float v2,
	upng_t* texture)
{
	if (y0 > y1)
	{
		SwapInt(x0, x1);
		SwapInt(y0, y1);
		SwapFloat(z0, z1);
		SwapFloat(w0, w1);
		SwapFloat(u0, u1);
		SwapFloat(v0, v1);
	}
	if (y1 > y2)
	{
		SwapInt(x1, x2);
		SwapInt(y1, y2);
		SwapFloat(z1, z2);
		SwapFloat(w1, w2);
		SwapFloat(u1, u2);
		SwapFloat(v1, v2);
	}
	if (y0 > y1)
	{
		SwapInt(x0, x1);
		SwapInt(y0, y1);
		SwapFloat(z0, z1);
		SwapFloat(w0, w1);
		SwapFloat(u0, u1);
		SwapFloat(v0, v1);
	}

	v0 = 1.0f - v0;
	v1 = 1.0f - v1;
	v2 = 1.0f - v2;

	Vec4 a = { static_cast<float>(x0), static_cast<float>(y0), static_cast<float>(z0), static_cast<float>(w0) };
	Vec4 b = { static_cast<float>(x1), static_cast<float>(y1), static_cast<float>(z1), static_cast<float>(w1) };
	Vec4 c = { static_cast<float>(x2), static_cast<float>(y2), static_cast<float>(z2), static_cast<float>(w2) };

	Vec2 uv0 = { u0, v0 };
	Vec2 uv1 = { u1, v1 };
	Vec2 uv2 = { u2, v2 };

	float slope1 = 0.0f;
	float slope2 = 0.0f;

	if (y1 - y0 != 0) slope1 = static_cast<float>(x1 - x0) / abs(y1 - y0);
	if (y2 - y0 != 0) slope2 = static_cast<float>(x2 - x0) / abs(y2 - y0);

	if (y1 - y0 != 0)
	{
		for (int y = y0; y < y1; y++)
		{
			int xStart = x1 + (y - y1) * slope1;
			int xEnd = x0 + (y - y0) * slope2;

			if (xEnd < xStart)
			{
				SwapInt(xEnd, xStart);
			}

			for (int x = xStart; x < xEnd; x++)
			{
				DrawTexel(x, y, texture, a, b, c, uv0, uv1, uv2);
			}
		}
	}

	slope1 = 0.0f;

	if (y2 - y1 != 0) slope1 = static_cast<float>(x2 - x1) / abs(y2 - y1);

	if (y2 - y1 != 0)
	{
		for (int y = y1; y < y2; y++)
		{
			int xStart = x1 + (y - y1) * slope1;
			int xEnd = x0 + (y - y0) * slope2;

			if (xEnd < xStart)
			{
				SwapInt(xEnd, xStart);
			}

			for (int x = xStart; x < xEnd; x++)
			{
				DrawTexel(x, y, texture, a, b, c, uv0, uv1, uv2);
			}
		}
	}

	/*DrawRectangle(a, 10, 10, 0xFFFF00FF);
	DrawRectangle(b, 10, 10, 0xFFFFFF00);
	DrawRectangle(c, 10, 10, 0xFF00FFFF);*/
}

void GraphicsManager::DrawTexel(
	int x, int y, upng_t* texture,
	const Vec4& a, const Vec4& b, const Vec4& c,
	const Vec2& uv0, const Vec2& uv1, const Vec2& uv2)
{
	Vec2 p = { static_cast<float>(x), static_cast<float>(y) };
	Vec2 pA = { a.x, a.y };
	Vec2 pB = { b.x, b.y };
	Vec2 pC = { c.x, c.y };

	Vec3 weights = GetBarycentricWeights(pA, pB, pC, p);

	float alpha = weights.x;
	float beta = weights.y;
	float gamma = weights.z;

	float interpolatedU;
	float interpolatedV;
	float interpolatedReciprocalW;

	interpolatedU = (uv0.x / a.w) * alpha + (uv1.x / b.w) * beta + (uv2.x / c.w) * gamma;
	interpolatedV = (uv0.y / a.w) * alpha + (uv1.y / b.w) * beta + (uv2.y / c.w) * gamma;

	interpolatedReciprocalW = (1 / a.w) * alpha + (1 / b.w) * beta + (1 / c.w) * gamma;

	interpolatedU /= interpolatedReciprocalW;
	interpolatedV /= interpolatedReciprocalW;

	int textureWidth = upng_get_width(texture);
	int textureHeight = upng_get_height(texture);

	int texX = abs(static_cast<int>(interpolatedU * textureWidth)) % textureWidth;
	int texY = abs(static_cast<int>(interpolatedV * textureHeight)) % textureHeight;

	interpolatedReciprocalW = 1.0f - interpolatedReciprocalW;

	if (interpolatedReciprocalW < mZBuffer[x + y * mWindowWidth])
	{
		uint32_t* textureBuffer = (uint32_t*)upng_get_buffer(texture);

		DrawPixel(x, y, textureBuffer[texX + texY * textureWidth]);
		//uint32_t color = GetBarycentricColor(weights.x, weights.y, weights.z);
		//DrawPixel(x, y, color);

		mZBuffer[x + y * mWindowWidth] = interpolatedReciprocalW;
	}
}

void GraphicsManager::RenderColorBuffer()
{
	SDL_UpdateTexture(mScreenTexture, NULL, mColorBuffer, sizeof(uint32_t) * mWindowWidth);
	SDL_RenderCopy(mRenderer, mScreenTexture, NULL, NULL);
}

void GraphicsManager::PresetRender()
{
	SDL_RenderPresent(mRenderer);
}

uint32_t GraphicsManager::ApplyLightIntensity(uint32_t color, float intensity)
{
	if (intensity < 0.0f) intensity = 0.0f;
	else if (intensity > 1.0f) intensity = 1.0f;

	uint32_t a = (color & 0xFF000000);
	uint32_t r = (color & 0x00FF0000) * intensity;
	uint32_t g = (color & 0x0000FF00) * intensity;
	uint32_t b = (color & 0x000000FF) * intensity;

	color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

	return color;
}

void GraphicsManager::SwapInt(int& a, int& b)
{
	int temp = a;
	a = b;
	b = temp;
}

void GraphicsManager::SwapFloat(float& a, float& b)
{
	float temp = a;
	a = b;
	b = temp;
}

void GraphicsManager::SwapVec2(Vec2& a, Vec2& b)
{
	Vec2 temp = a;
	a = b;
	b = temp;
}

void GraphicsManager::FillFlatBottomTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	float slope1 = static_cast<float>(x1 - x0) / (y1 - y0);
	float slope2 = static_cast<float>(x2 - x0) / (y2 - y0);

	float xStart = x0;
	float xEnd = x0;

	for (int y = y0; y <= y2; y++)
	{
		for (int x = xStart; x <= xEnd; x++)
		{
			DrawPixel(x, y, color);
		}

		xStart += slope1;
		xEnd += slope2;
	}
}

void GraphicsManager::FillFlatTopTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint32_t color)
{
	float slope1 = static_cast<float>(x2 - x0) / (y2 - y0);
	float slope2 = static_cast<float>(x2 - x1) / (y2 - y1);

	float xStart = x2;
	float xEnd = x2;

	for (int y = y2; y >= y0; y--)
	{
		for (int x = xStart; x <= xEnd; x++)
		{
			DrawPixel(x, y, color);
		}

		xStart -= slope1;
		xEnd -= slope2;
	}
}

Vec3 GraphicsManager::GetBarycentricWeights(const Vec2& a, const Vec2& b, const Vec2& c, const Vec2& p)
{
	Vec2 ac = Vec2Subtract(c, a);
	Vec2 ab = Vec2Subtract(b, a);
	Vec2 ap = Vec2Subtract(p, a);
	Vec2 pc = Vec2Subtract(c, p);
	Vec2 pb = Vec2Subtract(b, p);

	float areaABC = ac.x * ab.y - ac.y * ab.x;

	float alpha = (pc.x * pb.y - pc.y * pb.x) / areaABC;

	float beta = (ac.x * ap.y - ac.y * ap.x) / areaABC;

	float gamma = 1 - alpha - beta;

	Vec3 weights = { alpha, beta, gamma };

	return weights;
}

uint32_t GraphicsManager::GetBarycentricColor(float alpha, float beta, float gamma)
{
	uint32_t a = 0xFF000000;
	uint32_t r = 0x00FF0000 * alpha;
	uint32_t g = 0x0000FF00 * beta;
	uint32_t b = 0x000000FF * gamma;

	uint32_t color = a | (r & 0x00FF0000) | (g & 0x0000FF00) | (b & 0x000000FF);

	return color;
}
