#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <vector>
#include <cmath>

Pos Camera = Pos(0.0f, 0.0f, -5.0f);

void SimpleRenderer::open_window() {
	if (debug == true) { cout << "[DEBUG] function simple.open_window() from SimpleRenderer.cpp" << endl; }
	string WindowTitle;
	const SDL_DisplayMode* info = SDL_GetDesktopDisplayMode(1);
	if (!info)
	{
		cout << "Video query failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// Get Screen Information
	ScreenWidthF = static_cast<float>(info->w * 0.75);
	ScreenHeightF = static_cast<float>(info->h * 0.75);
	ScreenWidth = static_cast<int>(ScreenWidthF);
	ScreenHeight = static_cast<int>(ScreenHeightF);
	
	flags = SDL_WINDOW_RESIZABLE;
	// creating the title for the application window
	WindowTitle = "Simple Renderer " + to_string(ScreenWidth) + "x" + to_string(ScreenHeight);
	const char* WindowTitleChar = WindowTitle.c_str();
	// creating the window
	simple.window = SDL_CreateWindow(WindowTitleChar,ScreenWidth, ScreenHeight, static_cast<Uint32>(flags));
	if (!simple.window)
	{
		cout << "Window creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	// creating the renderer
	simple.renderer = SDL_CreateRenderer(simple.window,NULL);
	if (!simple.renderer)
	{
		cout << "Renderer creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	simple.RenderScale = (min(ScreenWidthF,ScreenHeightF)) * 1.0f;
}

void SimpleRenderer::render() {
	if (debug == true) { cout << "[DEBUG] function simple.render() from SimpleRenderer.cpp" << endl; }
	SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
	SDL_RenderClear(simple.renderer);
	simple.draw();
	SDL_RenderPresent(simple.renderer);
}

void SimpleRenderer::draw() {
	if (debug == true) { cout << "[DEBUG] function simple.draw() from SimpleRenderer.cpp" << endl; }
	// Red line from top left to top right
	SDL_SetRenderDrawColor(simple.renderer, 255, 0, 0, 255);
	SDL_RenderLine(simple.renderer, 0.0f, 0.0f, ScreenWidthF, 0.0f);
	// Green line from top left to bottom left
	SDL_SetRenderDrawColor(simple.renderer, 0, 255, 0, 255);
	SDL_RenderLine(simple.renderer, 0, 0, 0, ScreenHeightF);
	// Blue line from top right to bottom right
	SDL_SetRenderDrawColor(simple.renderer, 0, 0, 255, 255);
	SDL_RenderLine(simple.renderer, ScreenWidthF, 0, ScreenWidthF, ScreenHeightF);
	// Magenta line from bottom left to bottom right
	SDL_SetRenderDrawColor(simple.renderer, 255, 0, 255, 255);
	SDL_RenderLine(simple.renderer, 0, ScreenHeightF, ScreenWidthF, ScreenHeightF);
	// Draw coordinate system lines
	float temp = 1000.0f;
	simple.DrawLine(Pos(0.0f, 0.0f, 0.0f), Pos(temp, 0.0f, 0.0f), RGBA_int(128,0,0,255));
	simple.DrawLine(Pos(0.0f, 0.0f, 0.0f), Pos(0.0f, temp, 0.0f), RGBA_int(0,128,0,255));
	simple.DrawLine(Pos(0.0f, 0.0f, 0.0f), Pos(0.0f, 0.0f, temp), RGBA_int(0,0,128,255));
	

	// Draw all points from world
	for (int i = 0; i < static_cast<int>(world.Points.size()); i++) {
		simple.DrawPoint(world.Points[i]);
	}
	// Draw all triangles from world
	for (int j = 0; j < static_cast<int>(world.Triangles.size()); j++) {
		simple.DrawTriangle(world.Triangles[j]);
	}
}

void SimpleRenderer::DrawCircle(float x, float y, float r, RGBA_int c) {
	cout << "Drawing Circle at (" << x << ", " << y << ") with radius " << r << endl;
	float X;
	float TopY;
	float BotY;
	bool fill = true;
	for (float i = -r; i <= r; i++) {
			// get where the circle begins and ends
			X = x + i;
			TopY = y + sqrt(r * r - i * i);
			BotY = y - sqrt(r * r - i * i);
			// Fill the circle
			if (fill == true) {
				for (float j = BotY; j <= TopY; j++) {
					SDL_RenderPoint(simple.renderer, X, j);
				}
			}
			// Draw the outline of the circle
			SDL_RenderPoint(simple.renderer, X, TopY);
			SDL_RenderPoint(simple.renderer, X, BotY);
	}
}

void SimpleRenderer::DrawSphere(float x, float y, float z, float r, RGBA_int c) {
	// initialising variables
	float maxY;
	float maxZ;
	RGBA_int cr = c;
	float shade;
	//if (debug = true) { cout << "x: " << x << " y: " << y << " z: " << z << " cd: " << cd << endl; }
	// loop
	float step = 0.003;
	for (float i = -r; i <= r; i += step) {
		maxY = sqrt(r * r - i * i);
		for (float j = -maxY; j <= maxY; j += step) {
			maxZ = sqrt(r * r - i * i - j * j);
			for (float k = -maxZ; k <= maxZ; k += step) {
				Pos P = Pos(x + i, y + j, z + k);
				shade = k / (2 * r);
				cr.r = static_cast<UINT8>(c.r * shade);
				cr.g = static_cast<UINT8>(c.g * shade);
				cr.b = static_cast<UINT8>(c.b * shade);
				// Drawing the Point
				simple.DrawPosition({ P.x, P.y, P.z }, cr);
			}
		}
	}
	// x + y + z = r
	// z = r - x - y
	// x = r - y - z
	// y = r - x - z
}

void SimpleRenderer::DrawLine(Pos A, Pos B, RGBA_int c) {
	ScreenPos ScreenA = Projection(A);
	ScreenPos ScreenB = Projection(B);
	SDL_SetRenderDrawColor(simple.renderer,c.r,c.g,c.b,c.a);
	SDL_RenderLine(simple.renderer, ScreenA.x, ScreenA.y, ScreenB.x, ScreenB.y);
}

ScreenPos SimpleRenderer::Projection(Pos A) {
	float x = A.x - Camera.x;
	float y = A.y - Camera.y;
	float z = A.z - Camera.z;
	y = -y;
	float screenx = (x / z) * simple.RenderScale + ScreenWidthF / 2.0f;
	float screeny = (y / z) * simple.RenderScale + ScreenHeightF / 2.0f;
	return ScreenPos(screenx, screeny);
}


void SimpleRenderer::DrawTriangle(Triangle T) {
	RGBA_int ColorInt = FloatToIntColor(T.color);
	SDL_SetRenderDrawColor(simple.renderer, ColorInt.r, ColorInt.g, ColorInt.b, ColorInt.a);
	// Get Screen Coordinates
	ScreenPos ScreenA = Projection(T.p1.position);
	ScreenPos ScreenB = Projection(T.p2.position);
	ScreenPos ScreenC = Projection(T.p3.position);
	// Convert to SDL_FPoint
	SDL_FPoint SDLPOSA = { ScreenA.x, ScreenA.y };
	SDL_FPoint SDLPOSB = { ScreenB.x, ScreenB.y };
	SDL_FPoint SDLPOSC = { ScreenC.x, ScreenC.y };
	// Get Color
	SDL_FColor color = { T.color.r / 255.0f, T.color.g / 255.0f, T.color.b / 255.0f, T.color.a / 255.0f };
	//SDL_FColor colorB = { T.color.r, T.color.g, T.color.b, T.color.a };
	//SDL_FColor colorC = { T.color.r, T.color.g, T.color.b, T.color.a };
	//SDL_FPoint tex_coord;
	cout << "A " << ScreenA.x << " " << ScreenA.y << " B " << ScreenB.x << " " << ScreenB.y << " C " << ScreenC.x << " " << ScreenC.y << endl;
	SDL_Vertex TriangleVertex[3] = { {SDLPOSA, color, NULL }, { SDLPOSB, color, NULL }, { SDLPOSC, color, NULL } };
	SDL_RenderGeometry(simple.renderer, NULL, TriangleVertex, 3, NULL, 0);
	simple.DrawScreenLine(ScreenA, ScreenB, ColorInt);
	simple.DrawScreenLine(ScreenB, ScreenC, ColorInt);
	simple.DrawScreenLine(ScreenC, ScreenA, ColorInt);
}

void SimpleRenderer::DrawScreenLine(ScreenPos A, ScreenPos B, RGBA_int c) {
	SDL_SetRenderDrawColor(simple.renderer, c.r, c.g, c.b, c.a);
	SDL_RenderLine(simple.renderer, A.x, A.y, B.x, B.y);
}

float SimpleRenderer::DistBetweenPoints(Pos a, Pos b) {
	Pos v = Pos(a.x - b.x, a.y - b.y, a.z - b.z);
	return sqrt(v.x*v.x + v.y * v.y + v.z * v.z);
}

void SimpleRenderer::DrawPosition(Pos A, RGBA_int c) {
	if (A.z <= 0) { return; }
	SDL_SetRenderDrawColor(simple.renderer,c.r,c.g,c.b,c.a);
	// calculating the screen coordinates for the point
	ScreenPos ScreenA = Projection(A);
	// drawing the point on the screen
	SDL_RenderPoint(simple.renderer, ScreenA.x, ScreenA.y);
}

void SimpleRenderer::DrawPoint(Point A) {
	// calculating the screen coordinates for the point
	ScreenPos ScreenA = Projection(A.position);
	RGBA_int Color = FloatToIntColor(A.color);
	SDL_SetRenderDrawColor(simple.renderer, Color.r,Color.g,Color.b, Color.a);
	cout << "Drawing Point: " << A.letter << " on Canvas at (" << ScreenA.x << ", " << ScreenA.y << ")" << endl;
	SDL_RenderPoint(simple.renderer, ScreenA.x, ScreenA.y);
	//simple.DrawCircle(screenx, screeny, 10.0f, Color);
	simple.DrawSphere(A.position.x, A.position.y, A.position.z, 0.1, Color);
}

SimpleRenderer simple;