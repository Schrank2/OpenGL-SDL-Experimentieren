#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <vector>
#include <cmath>
#include <iomanip> // basically settings for cout
#include <algorithm> // for clamp()

vector<vector<float>> SimpleRenderer::CreateDepthBuffer() {
	vector<vector<float>> D;
	D.resize(ScreenWidth, vector<float>(ScreenHeight));
	return D;
	}

void SimpleRenderer::GetScreenData() {
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
}

SDL_Window* SimpleRenderer::Create_Window(string title) {
	if (debug == true) { cout << "[DEBUG] function simple.Create_Window() from SimpleRenderer.cpp" << endl; }
	string WindowTitle;
	flags = SDL_WINDOW_RESIZABLE;
	// creating the title for the application window
	WindowTitle = title + " " + to_string(ScreenWidth) + "x" + to_string(ScreenHeight);
	const char* WindowTitleChar = WindowTitle.c_str();
	// creating the window
	SDL_Window* window = SDL_CreateWindow(WindowTitleChar, ScreenWidth, ScreenHeight, static_cast<Uint32>(flags));
	if (!window)
	{
		cout << "Window creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	return window;
}

SDL_Renderer* SimpleRenderer::Create_Renderer(SDL_Window* window){
	if (debug == true) { cout << "[DEBUG] function simple.Create_Renderer() from SimpleRenderer.cpp" << endl; }
	// creating the renderer
	SDL_Renderer* renderer = SDL_CreateRenderer(window,NULL);
	if (!renderer)
	{
		cout << "Renderer creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	simple.RenderScale = (min(ScreenWidthF,ScreenHeightF)) * 1.0f;
	return renderer;
}

void SimpleRenderer::init() {
	// Get Screen Data for Window creation
	simple.GetScreenData();
	// Creating the Camera
	// Creating the Main Window
	simple.window = Create_Window("Simple Render Main");
	simple.renderer = Create_Renderer(simple.window);
	// Creating the Depth Buffer Window
	simple.DepthBufferWindow = Create_Window("Simple Render Depth Buffer");
	simple.DepthBufferRenderer = Create_Renderer(simple.DepthBufferWindow);
	simple.DepthBuffer = simple.CreateDepthBuffer();
}

void SimpleRenderer::render() {
	if (debug == true) { cout << "[DEBUG] function simple.render() from SimpleRenderer.cpp" << endl; }
	// Clear the Main Window
	SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
	SDL_RenderClear(simple.renderer);
	// Clear the Depth Buffer
	SDL_SetRenderDrawColor(simple.DepthBufferRenderer, 255, 255, 255, 255);
	SDL_RenderClear(simple.DepthBufferRenderer);
	for (auto& row : DepthBuffer)
		fill(row.begin(), row.end(), NULL);
	DepthBufferMax = 0.0f;
	// Draw the Main Window
	simple.draw();
	// Render the Depth Buffer
	int i, j;
	float a;
	cout << DepthBuffer.size() << endl;
	for (i = 0; i < DepthBuffer.size(); i++) {
		for (j = 0; j < DepthBuffer[0].size(); j++) {
			a = DepthBuffer[i][j] / DepthBufferMax;
			//cout << fixed << setprecision(3) << a << " " << DepthBuffer[i][j] << endl;
			if (a < 0.0f) { a = 0.0f; }
			if (a > 1.0f) { a = 1.0f; }
			//cout << fixed << setprecision(2) << a << endl;
			SDL_SetRenderDrawColorFloat(simple.DepthBufferRenderer, a, a, a, 1.0f);
			SDL_RenderPoint(simple.DepthBufferRenderer, i, j);
		}
	}
	SDL_RenderPresent(simple.renderer);
	SDL_RenderPresent(simple.DepthBufferRenderer);
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
	float temp = 1.5f;
	ScreenPos Origin = Projection(Pos(0.0f, 0.0f, 0.0f));
	simple.DrawLine(Origin, Projection(Pos(temp, 0.0f, 0.0f)), RGBA_int(255,0,0,255));
	simple.DrawLine(Origin, Projection(Pos(0.0f, temp, 0.0f)), RGBA_int(0,255,0,255));
	simple.DrawLine(Origin, Projection(Pos(0.0f, 0.0f, temp)), RGBA_int(0,0,255,255));

	// Draw all points from world
	for (int i = 0; i < static_cast<int>(world.Points.size()); i++) {
		simple.DrawPoint(world.Points[i]);
	}
	// Draw all triangles from world
	for (int j = 0; j < static_cast<int>(world.Triangles.size()); j++) {
		simple.DrawTriangle(world.Triangles[j]);
	}
}

void SimpleRenderer::DrawSphere(Pos A, float r, RGBA_int c) {
	ScreenPos As = Projection(A);
	float FrontDepth = A.z - simple.Camera.z - r;
	// weirdly adjusting the radius for depth of A
	ScreenPos Temp = Projection({ A.y, r + A.y, A.z });
	float R = As.y - Temp.y;
	// where the sphere is lit most brightly (temporary, will later be replaced)
	ScreenPos Light = ScreenPos(As.x - (R/2), As.y - (R/2), As.z);

	float X;
	float TopY;
	float BotY;
	bool fill = true;

	float lshade;
	float BufferDepth;

	float i;
	for (i = - R; i <= R; i++) {
		X = As.x + i;
		TopY = As.y + sqrt(R * R - i * i);
		BotY = As.y - sqrt(R * R - i * i);
		// Fill the circle
		if (fill == true) {
			for (float j = BotY; j <= TopY; j++) {
				float center = ScreenDist(As, ScreenPos(X,j,0.0f)); // Distance Between Lightspot and Poin
				float x = center / R; // Distance Between Center and Point displayed between 0.0f and 1.0f.
				if (x < 0.0f) x = 0.0f;
				if (x > 1.0f) x = 1.0f;
				float z = sqrt(1.0f - x * x) * r; // Tiefenunterschied zwischen Punkt und Zentrum
				ScreenPos L = { X,j, FrontDepth - z };
				// shading the point
				float d = ScreenDist(Light, L); // Distance Between Lightspot and Point
				lshade = 1.0f - (d / R);
				RGBA_int Localc = ModifyColor(lshade, 0.5f, c);
				// drawing
				if (DepthBufferPoint(L)) { // checking if the point is in front in the depth Buffer
					SDL_SetRenderDrawColor(simple.renderer, Localc.r, Localc.g, Localc.b, Localc.a);
					SDL_RenderPoint(simple.renderer, L.x, L.y);
				}
			}
		}
	}
}

RGBA_int SimpleRenderer::ModifyColor(float modifier, float strength, RGBA_int c) {
	float strengthO = 1.0f - strength;
	int r = static_cast<int>(c.r * strengthO + c.r * strength * modifier);
	int g = static_cast<int>(c.g * strengthO + c.g * strength * modifier);
	int b = static_cast<int>(c.b * strengthO + c.b * strength * modifier);
	int a = static_cast<int>(c.a * strengthO + c.a * strength * modifier);
	return RGBA_int(r, g, b, a);
}

float SimpleRenderer::ScreenDist(ScreenPos A, ScreenPos B) {
	float LineX = A.x - B.x;
	float LineY = A.y - B.y;
	return abs(sqrt(LineX * LineX + LineY * LineY));
}

ScreenPos SimpleRenderer::Projection(Pos A) {
	float x = A.x - simple.Camera.x;
	float y = A.y - simple.Camera.y;
	float z = A.z - simple.Camera.z;
	y *= -1;
	float screenx = (x / z) * simple.RenderScale + ScreenWidthF / 2.0f;
	float screeny = (y / z) * simple.RenderScale + ScreenHeightF / 2.0f;
	return ScreenPos(screenx, screeny, z);
}

void SimpleRenderer::DrawTriangle(Triangle T) {
	RGBA_int ColorInt = FloatToIntColor(T.color);
	SDL_SetRenderDrawColor(simple.renderer, ColorInt.r, ColorInt.g, ColorInt.b, ColorInt.a);
	// Get Screen Coordinates
	ScreenPos A = Projection(T.p1.pos);
	ScreenPos B = Projection(T.p2.pos);
	ScreenPos C = Projection(T.p3.pos);
	// Sort by smallest x
	ScreenPos temp = A;
	if (B.y < A.y) { temp = B; B = A; A = temp; }
	if (C.y < A.y) { temp = C; C = A; A = temp; }
	if (C.y < B.y) { temp = C; C = B; B = temp; }
	cout << "sort result: " << A.y << " " << B.y << " " << C.y << endl;

	// Drawing the WireFrame
	DrawLine(A, B, ColorInt);
	DrawLine(B, C, ColorInt);
	DrawLine(C, A, ColorInt);
	// AB Line
	float ABm = static_cast<float>(B.x - A.x) / static_cast<float>(B.y - A.y);
	float ABb = -A.y;
	// AC Line
	float ACm = static_cast<float>(C.x - A.x) / static_cast<float>(C.y - A.y);
	float ACb = -A.y;
	// BC Line
	float BCm = static_cast<float>(C.x - B.x) / static_cast<float>(C.y - B.y);
	float BCb = -B.y;

	int Y1;
	int Y2;
	for (int x = A.x; x <= B.x; x++) {
		Y1 = (ACm * x + ACb);
		Y2 = (ABm * x + ABb);
		for (int i = 0; i <= Y1 - Y2; i++) SDL_RenderPoint(simple.renderer, x, Y1 + i);
	}
	for (int x = B.x; x <= C.x; x++) {
		Y1 = (ACm * x + ACb);
		Y2 = (BCm * x + BCb);
		for (int i = 0; i <= Y1 - Y2; i++) SDL_RenderPoint(simple.renderer, x, Y1 + i);
	}
}

void SimpleRenderer::DrawLine(ScreenPos A, ScreenPos B, RGBA_int c) {
	SDL_SetRenderDrawColor(simple.renderer, c.r, c.g, c.b, c.a);
	// Direction Vector AB
	ScreenPos DV = ScreenPos(B.x - A.x, B.y - A.y, B.z - A.z);
	//Step Count
	float r = max(abs(DV.x), abs(DV.y));
	// Step Size
	float s = 1.0f / r;
	// Current Position
	ScreenPos C = A;
	// Just Draw a point if steps == 0;
	if (s == 0) {
		if (DepthBufferPoint(C)) SDL_RenderPoint(simple.renderer, C.x, C.y);
		return;
	}
	// Vector to add Between each step (SV Stepvector)
	ScreenPos SV = ScreenPos(DV.x * s, DV.y * s, DV.z * s);

	for (float i = 0; i <= r; i++) { // r starts at -s because s is instantly added so it starts at 0.0f
		C.x += SV.x;
		C.y += SV.y;
		C.z += SV.z;
		if (DepthBufferPoint(C)) {
			SDL_RenderPoint(simple.renderer, C.x, C.y);
		}
	}
	
}

bool SimpleRenderer::DepthBufferPoint(ScreenPos A) {
	int x = static_cast<int>(A.x);
	int y = static_cast<int>(A.y);
	if (x < 0 or y < 0 or x >= DepthBuffer.size() or y >= DepthBuffer[0].size()) return false; // Check if Point is on screen
	if (DepthBuffer[x][y] == NULL or DepthBuffer[x][y] > A.z) {
		if (A.z > DepthBufferMax) DepthBufferMax = A.z;
		DepthBuffer[x][y] = A.z;
		return true;
	}
	return false;
}

float SimpleRenderer::DistBetweenPoints(Pos a, Pos b) {
	Pos v = Pos(a.x - b.x, a.y - b.y, a.z - b.z);
	return sqrt(v.x*v.x + v.y * v.y + v.z * v.z);
}

void SimpleRenderer::DrawPoint(Point A) {
	// calculating the screen coordinates for the point
	ScreenPos ScreenA = Projection(A.pos);
	RGBA_int Color = FloatToIntColor(A.color);
	SDL_SetRenderDrawColor(simple.renderer, Color.r,Color.g,Color.b, Color.a);
	if (debug == true) { cout << "[DEBUG] Drawing Point: " << A.letter << " on Canvas at (" << ScreenA.x << ", " << ScreenA.y << ")" << endl; }
	SDL_RenderPoint(simple.renderer, ScreenA.x, ScreenA.y);
	simple.DrawSphere(A.pos, 0.1f, Color);
}

SimpleRenderer simple;