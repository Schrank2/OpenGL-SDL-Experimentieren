#ifndef SIMPLERENDERER_H // only defines if variable has not been defined yet
#define SIMPLERENDERER_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "defs.h"

// declare R1 so rastSdlRender.cpp can use it
class SimpleRenderer {
public:
	void init(int* ScreenWidth, int* ScreenHeight);
	bool DepthBufferShown = false;
	void render(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue);
	void Get_TTF_Fonts();
	TTF_Font* ReportFont;
	vector<float> DepthBuffer;
	PhysicsObject Camera = PhysicsObject(Pos(-2.0f, 1.0f, -2.0f), Pos(0.0f, 0.0f, 0.0f), 1.0f);
	float CameraPitch = 0.0f;
	float CameraYaw = 0.0f;
	float DepthBufferMax = NULL;
	float DepthBufferMin = NULL;
	SDL_Renderer* Create_Renderer(SDL_Window* window);
	SDL_Window* Create_Window(string title);
	TTF_TextEngine* Create_TextEngine(SDL_Renderer* renderer);
	void GetScreenData(int* ScreenWidth, int* ScreenHeight);
	void draw(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue);
	void DrawSphere(Pos A, float r, RGBA_int c);
	ScreenPos Projection(float A[3]);
	ScreenPos Projection(Pos* A3D);
	bool DepthBufferPoint(ScreenPos A);
	bool DepthBufferPoint(float A[3]);
	void DrawPoint(Point* point);
	void TextRender();
	void GetVector(float Vector[3], float A[3], float B[3]); 
	void DrawLine(Pos* A3D, Pos* B3D, RGBA_int* c);
	void DrawTriangle(Pos* A, Pos* B, Pos* C, RGBA_int* Color);
	void DrawTriangle(float A3D[3], float B3D[3], float C3D[3], RGBA_int* Color);
	void DrawScanLine(int* y, int* leftx, float* leftz, int* rightx, float* rightz, RGBA_int* Color, float* DiffZ, float* shadeIntensity);
	float DistBetweenPoints(Pos a, Pos b);
	float RenderScale;
	float ScreenDist(ScreenPos A, ScreenPos B);
	RGBA_int ModifyColor(float modifier, float strength, RGBA_int c);
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Texture* canvas;
	TTF_TextEngine* TextEngine;
	int RenderStartTime;
	int RenderEndTime;
	int RenderTime;
	bool CheckScreenPos(ScreenPos A);
	bool CheckScreenPos(float A[3]);
	vector<Uint32> pixels;
	void DrawPixel(float* x, float* y, RGBA_int* c);
	float ScreenWidth;
	float ScreenHeight;
	int ScreenWidthF;
	int ScreenHeightF;
	bool run = true;
};
// declare r1 so the R1 functions can be called elsewhere
extern SimpleRenderer simple;

#endif // SIMPLERENDERER_H