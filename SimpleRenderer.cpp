#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <vector>
#include <cmath>

float CameraX = 0.0f;
float CameraY = 0.0f;
float CameraZ = -5.0f;

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
	simple.RenderScale = 10.0f;
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
	// Draw all points from world
	for (int i = 0; i < static_cast<int>(world.Points.size()); i++) {
		simple.DrawPoint(world.Points[i]);
	}
}
float SimpleRenderer::GetScreenDepth(float z) {
	return 1 + 0.03f * (CameraZ - z); // adjusting Depth for perspective
}

float SimpleRenderer::GetScreenCoordX(float x, float z, float Depth) {
	x = x - CameraX;
	z = z - CameraZ;
	return (x / Depth) * (ScreenWidthF / simple.RenderScale);
}
float SimpleRenderer::GetScreenCoordY(float y, float z, float Depth) {
	y = y - CameraY;
	z = z - CameraZ;
	return (y / Depth) * (ScreenHeightF / simple.RenderScale);
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
	float maxY;
	float maxZ;
	RGBA_int cr = c;
	for (float i = -r; i <= r; i += 0.01f) {
		maxY = sqrt(r * r - i * i);
		for (float j = -maxY; j <= maxY; j += 0.01f) {
			maxZ = sqrt(r * r - i * i - j * j);
			for (float k = -maxZ; k <= maxZ; k += 0.01f) {
				cr.r = static_cast<UINT8>(cr.r - (i + j + k) / 255);
				cr.g = static_cast<UINT8>(cr.g - (i + j + k) / 255);
				cr.b = static_cast<UINT8>(cr.b - (i + j + k) / 255);
				simple.DrawPosition({x + i, y + j, z + k}, cr);
			}
		}
	}
	// x + y + z = r
	// z = r - x - y
	// x = r - y - z
	// y = r - x - z
	
}
void SimpleRenderer::DrawPosition(Pos pos, RGBA_int c) {
	SDL_SetRenderDrawColor(simple.renderer,c.r,c.g,c.b,c.a);
	float x = pos.x;
	float y = pos.y;
	float z = pos.z;
	// calculating the screen coordinates for the point
	float Depth = GetScreenDepth(z);
	float screenx = GetScreenCoordX(x, z, Depth);
	float screeny = GetScreenCoordY(y, z, Depth);
	// drawing the point on the screen
	SDL_RenderPoint(simple.renderer, screenx, screeny);
	//simple.DrawCircle(screenx, screeny, 1.0f, c);
}

void SimpleRenderer::DrawPoint(Point point) {
	float x = point.position.x;
	float y = point.position.y;
	float z = point.position.z;
	// calculating the screen coordinates for the point
	float Depth = GetScreenDepth(z);
	float screenx = GetScreenCoordX(x, z, Depth);
	float screeny = GetScreenCoordY(y, z, Depth);
	RGBA_int Color = FloatToIntColor(point.color);
	SDL_SetRenderDrawColor(simple.renderer, Color.r,Color.g,Color.b, Color.a);
	cout << "Drawing Point: " << point.letter << " on Canvas at (" << screenx << ", " << screeny << ")" << endl;
	SDL_RenderPoint(simple.renderer, screenx, screeny);
	//simple.DrawCircle(screenx, screeny, 10.0f, Color);
	simple.DrawSphere(x, y, z, 0.3f, Color);
}

SimpleRenderer simple;