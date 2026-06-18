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
	void render();
	void open_window();
	void draw();
	void DrawCircle(float x, float y, float r, RGBA_int c);
	void DrawSphere(float x, float y, float z, float r, RGBA_int c);
	void DrawPosition(Pos pos, RGBA_int c);
	float GetScreenCoordX(float x, float Depth);
	float GetScreenCoordY(float y, float Depth);
	float GetScreenDepth(Pos Position);
	void DrawPoint(Point point);
	void DrawLine(Pos A, Pos B, RGBA_int c);
	float DistBetweenPoints(Pos a, Pos b);
	float RenderScale;
	SDL_Window* window;
	SDL_Renderer* renderer;
};
// declare r1 so the R1 functions can be called elsewhere
extern SimpleRenderer simple;

// declare game so game.cpp can use it
class WORLD {
public:
	vector<Point> Points;
	void init();
	void tick();
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;

#endif // CLASS_H