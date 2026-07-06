#ifndef CLASS_H // only defines if variable has not been defined yet
#define CLASS_H

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
	void init();
	bool DepthBufferShown = false;
	void render();
	void Get_TTF_Fonts();
	TTF_Font* ReportFont;
	vector<float> DepthBuffer;
	PhysicsObject Camera = PhysicsObject(Pos(2.0f, 1.0f, -3.0f), Pos(0.0f, 0.0f, 0.0f), 1.0f);
	float CameraPitch = 0.0f;
	float CameraYaw = 45.0f;
	float DepthBufferMax, DepthBufferMin;
	SDL_Renderer* Create_Renderer(SDL_Window* window);
	SDL_Window* Create_Window(string title);
	TTF_TextEngine* Create_TextEngine(SDL_Renderer* renderer);
	void GetScreenData();
	void draw();
	void DrawSphere(Pos A, float r, RGBA_int c);
	ScreenPos Projection(Pos A);
	bool DepthBufferPoint(ScreenPos A);
	void DrawPoint(Point point);
	void TextRender();
	void DrawLine(ScreenPos A, ScreenPos B, RGBA_int c);
	void DrawTriangle(Triangle A);
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
	vector<Uint32> pixels;
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
	KeyBoardState KeyBoard;
	float TickStrength = 0.0f;
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;

#endif // CLASS_H