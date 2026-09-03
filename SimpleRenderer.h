#ifndef SIMPLERENDERER_H // only defines if variable has not been defined yet
#define SIMPLERENDERER_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include <thread>
#include "defs.h"

// declare R1 so rastSdlRender.cpp can use it
class SimpleRenderer {
public:
	void init(int* ScreenWidth, int* ScreenHeight, int* ThreadsAllocated);
	bool DepthBufferShown = false;
	void render(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue);
	void Get_TTF_Fonts();
	TTF_Font* ReportFont;
	vector<float> DepthBuffer;
	PhysicsObject Camera = PhysicsObject(Pos(-2.0f, 1.0f, -2.0f), Pos(0.0f, 0.0f, 0.0f), 1.0f);
	float CameraPitch = 0.0f;
	float CameraYaw = 0.0f;
	SDL_Renderer* Create_Renderer(SDL_Window* window);
	SDL_Window* Create_Window(string title);
	TTF_TextEngine* Create_TextEngine(SDL_Renderer* renderer);
	void GetScreenData(int* ScreenWidth, int* ScreenHeight);
	void draw(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue);
	void DrawSphere(Pos A, float r, RGBA_int c);
	ScreenPos Projection(Pos* A3D);
	bool DepthBufferPoint(ScreenPos A, vector<float>* ThreadDepthBuffer);
	void DrawPoint(Point* point);
	void TextRender();
	void GetVector(float Vector[3], float A[3], float B[3]); 
	void DrawLine(Pos* A3D, Pos* B3D, RGBA_int* c);
	void DrawTriangle(Pos* A3D, Pos* B3D, Pos* C3D, RGBA_int* Color, vector<Uint32>* ThreadPixels, vector<float>* ThreadDepthBuffer);
	void DrawScanLine(float* y, float* leftx, float* leftz, float* rightx, float* rightz, RGBA_int* Color, float* minZ, float* maxZ, float* shadeIntensity, vector<Uint32>* ThreadPixels, vector<float>* ThreadDepthBuffer);
	float DistBetweenPoints(Pos a, Pos b);
	float RenderScale;
	float ScreenDist(ScreenPos A, ScreenPos B);
	RGBA_int ModifyColor(float modifier, float strength, RGBA_int c);
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* canvas;
	TTF_TextEngine* TextEngine;
	int RenderStartTime = 0;
	int RenderEndTime = 0;
	int RenderTime = 0;
	bool CheckScreenPos(ScreenPos A);
	vector<Uint32> pixels;
	vector<vector<Uint32>> ThreadedPixels;
	vector<vector<float>> ThreadedDepthBuffer;
	void DrawPixel(float* x, float* y, RGBA_int* c, vector<Uint32>* ThreadPixels);
	int ScreenWidth;
	int ScreenHeight;
	float ScreenWidthF;
	float ScreenHeightF;
	bool active = true;
	int ThreadAllocation = 1;
	vector<thread> threads;
	void TriangleRenderThread(int ThreadIndex, vector<Triangle>* TriangleQueue, int start, int end);
	int PolyGonsRenderedTotal = 0;
	vector<int> PolyGonsRenderedPerThread;
	int TriangleDrawingTime = 0;
	int ThreadMergingTime = 0;
	vector<int> PerThreadTriangleTime;
	int DepthBufferMergingTime = 0;
	void TriangleRenderThreadInitialisation(int ThreadIndex, int TrianglesPerThread, vector<Triangle>* TriangleQueue);
	float NearPlane = 0.1f;
	float FarPlane = 50.0f;
	vector<Uint32> EmptyScreen;
	vector<float> EmptyDepthBuffer;
	float DepthBufferMin = 0.0f;
	float DepthBufferMax = 0.0f;
	void DepthBufferThread(int offset, int end, vector<Uint32>* pixels, vector<float>* DepthBuffer, int ThreadIndex, int ThreadsUsed);
};
// declare r1 so the R1 functions can be called elsewhere
extern SimpleRenderer simple;

#endif // SIMPLERENDERER_H