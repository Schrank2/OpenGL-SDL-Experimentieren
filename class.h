#ifndef CLASS_H // only defines if variable has not been defined yet
#define CLASS_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>
#include <vector>
#include "defs.h"

// declare R1 so rastSdlRender.cpp can use it
class SimpleRenderer {
public:
	void init();
	void render();
	vector<vector<float>> CreateDepthBuffer();
	vector<vector<float>> DepthBuffer;
	Pos Camera = Pos(0.0f, 0.0f, -3.0f);
	float DepthBufferMax;
	SDL_Renderer* Create_Renderer(SDL_Window* window);
	SDL_Window* Create_Window(string title);
	void GetScreenData();
	void draw();
	void DrawCircle(float x, float y, float r, RGBA_int c);
	void DrawSphere(float x, float y, float z, float r, RGBA_int c);
	void DrawSphere2(Pos A, float r, RGBA_int c);
	void DrawPosition(Pos A, RGBA_int c);
	ScreenPos Projection(Pos A);
	bool DepthBufferPoint(ScreenPos A);
	void DrawPoint(Point point);
	void DrawLine(ScreenPos A, ScreenPos B, RGBA_int c);
	void DrawTriangle(Triangle A);
	float DistBetweenPoints(Pos a, Pos b);
	float RenderScale;
	float ScreenDist(ScreenPos A, ScreenPos B);
	RGBA_int ModifyColor(float modifier, float strength, RGBA_int c);
	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Window* DepthBufferWindow;
	SDL_Renderer* DepthBufferRenderer;
};
// declare r1 so the R1 functions can be called elsewhere
extern SimpleRenderer simple;

// declare game so game.cpp can use it
class WORLD {
public:
	vector<Point> Points;
	vector<Line> Lines;
	vector<Triangle> Triangles;
	vector<Plane> Planes;
	void init();
	void tick();
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;

#endif // CLASS_H