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

void SimpleRenderer::DrawSphere2(Pos A, float r, RGBA_int c) {
	ScreenPos As = Projection(A);
	float FrontDepth = A.z - simple.Camera.z - r;
	// weirdly adjusting the radius for depth of A
	ScreenPos Temp = Projection({ A.y, r + A.y, A.z });
	float R = As.y - Temp.y;
	// where the sphere is lit most brightly (temporary, will later be replaced)
	ScreenPos Light = ScreenPos(As.x - (R/2), As.y - (R/2));

	int X;
	float TopY;
	float BotY;
	bool fill = true;

	float lshade;
	float BufferDepth;

	int i;
	for (i = - R; i <= R; i++) {
		X = As.x + i;
		TopY = As.y + sqrt(R * R - i * i);
		BotY = As.y - sqrt(R * R - i * i);
		// Fill the circle
		if (fill == true) {
			for (int j = BotY; j <= TopY; j++) {
				ScreenPos L = {X,j};
				if (L.x >= 0 and L.x <= DepthBuffer.size() and L.y >= 0 and L.y <= DepthBuffer[0].size()) {
					//cout << L.x << " " << L.y << " " << DepthBuffer.size() << " " << DepthBuffer[0].size() << endl;
					// shading the point
					float d = ScreenDist(Light, L); // Distance Between Lightspot and Point
					float center = ScreenDist(As, L); // Distance Between Lightspot and Point
					lshade = 1.0f - (d / R);
					//lshade = lshade * lshade;
					//cout << lshade << endl;ss
					RGBA_int Localc = ModifyColor(lshade, 0.5f, c);
					// Estimating the distance of the point from the camera
					// mithilfe von Kugel Rotationskörper Funktion (f(x) = sqrt(1-(x*x))
					float x = center / R; // Distance Between Center and Point displayed between 0.0f and 1.0f.
					//cout << "x: " << x << " center: " << center << " R: " << R << endl;
					if (x < 0.0f)
						x = 0.0f;
					if (x > 1.0f)
						x = 1.0f;
					float z = sqrt(1.0f - x * x) * r; // Tiefenunterschied zwischen Punkt und Zentrum
					BufferDepth = FrontDepth - z;
					//cout << BufferDepth << " " << fixed << setprecision(5) << A.z << endl;
					if (DepthBuffer[L.x][L.y] == NULL or DepthBuffer[L.x][L.y] > BufferDepth) { // checking if the point is in front in the depth Buffer
						// changing the Depth Buffer
						if (BufferDepth > DepthBufferMax)
							DepthBufferMax = BufferDepth;
						DepthBuffer[L.x][L.y] = BufferDepth;
						// drawing the point
						SDL_SetRenderDrawColor(simple.renderer, Localc.r, Localc.g, Localc.b, Localc.a);
						SDL_RenderPoint(simple.renderer, L.x, L.y);
					}
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

void SimpleRenderer::DrawSphere(float x, float y, float z, float r, RGBA_int c) {
	// initialising variables
	float maxY;
	float maxZ;
	RGBA_int cr = c;
	float shade;
	//if (debug = true) { cout << "x: " << x << " y: " << y << " z: " << z << " cd: " << cd << endl; }
	// loop
	float step = 0.003f;
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
	float x = A.x - simple.Camera.x;
	float y = A.y - simple.Camera.y;
	float z = A.z - simple.Camera.z;
	y = -y;
	float screenx = (x / z) * simple.RenderScale + ScreenWidthF / 2.0f;
	float screeny = (y / z) * simple.RenderScale + ScreenHeightF / 2.0f;
	return ScreenPos(screenx, screeny);
}

void SimpleRenderer::DrawTriangle(Triangle T) {
	RGBA_int ColorInt = FloatToIntColor(T.color);
	SDL_SetRenderDrawColor(simple.renderer, ColorInt.r, ColorInt.g, ColorInt.b, ColorInt.a);
	// Get Screen Coordinates
	ScreenPos ScA = Projection(T.p1.pos);
	ScreenPos ScB = Projection(T.p2.pos);
	ScreenPos ScC = Projection(T.p3.pos);
	// Drawing the WireFrame
	DrawScreenLineInterpolation(ScA, ScB, ColorInt);
	DrawScreenLineInterpolation(ScB, ScC, ColorInt);
	DrawScreenLineInterpolation(ScC, ScA, ColorInt);
	//DrawScreenLine(ScA, ScB, ColorInt);
	//DrawScreenLine(ScB, ScC, ColorInt);
	//DrawScreenLine(ScC, ScA, ColorInt);	
}
void SimpleRenderer::DrawScreenLineInterpolation(ScreenPos A, ScreenPos B, RGBA_int c) {
	SDL_SetRenderDrawColor(simple.renderer, c.r, c.g, c.b, c.a);
	// Using Bresenhams line Algorithm
	int x0 = A.x, y0 = A.y; // Set A as coordinate origin
	int x1 = B.x, y1 = B.y; // Set B as target point
	int dx = abs(x1 - x0); // x-Distance between A and B
	int sx = x0 < x1 ? 1 : -1; // get direction of the line in the x-axis
	int dy = -abs(y1 - y0); // y-Distance between A and B
	int sy = y0 < y1 ? 1 : -1; // get direction of the line in the y-axis
	int err = dx + dy; // "absolute-ish" Manhattan-Distance between A and B
	int e2; // (later) doubled distance to avoid floating point math
	while (true) {
		SDL_RenderPoint(simple.renderer, x0, y0);
		if (x0 == x1 && y0 == y1) break; // stop drawing points if both values progressed to the target value
		e2 = 2 * err; // double the distance to avoid floating point math
		if (e2 >= dy) { err += dy; x0 += sx; } // iterate x0 by 1 or -1 if Manhattan Distance is larger than y distance
		if (e2 <= dx) { err += dx; y0 += sy; } // iterate y0 by 1 or -1 if Manhattan Distance is larger than x distance
	}
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
	ScreenPos ScreenA = Projection(A.pos);
	RGBA_int Color = FloatToIntColor(A.color);
	SDL_SetRenderDrawColor(simple.renderer, Color.r,Color.g,Color.b, Color.a);
	if (debug == true) { cout << "[DEBUG] Drawing Point: " << A.letter << " on Canvas at (" << ScreenA.x << ", " << ScreenA.y << ")" << endl; }
	SDL_RenderPoint(simple.renderer, ScreenA.x, ScreenA.y);
	//simple.DrawCircle(screenx, screeny, 10.0f, Color);
	//simple.DrawSphere(A.position.x, A.position.y, A.position.z, 0.1, Color);
	simple.DrawSphere2(A.pos, 0.1f, Color);
}

SimpleRenderer simple;