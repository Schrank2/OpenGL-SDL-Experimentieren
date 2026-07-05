#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
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

TTF_TextEngine* SimpleRenderer::Create_TextEngine(SDL_Renderer* renderer) {
	if (debug == true) { cout << "[DEBUG] function simple.Create_TextRenderer() from SimpleRenderer.cpp" << endl; }
	TTF_TextEngine* TextEngine = TTF_CreateRendererTextEngine(renderer);
	if (!TextEngine)
	{
		cout << "Text Engine creation failed: " << SDL_GetError() << endl;
		exit(1);
	}
	return TextEngine;
}

void SimpleRenderer::Get_TTF_Fonts() {
	ReportFont = TTF_OpenFont("C:/Windows/Fonts/arial.ttf", 12);
	if (!ReportFont) {
		cout << "TTF_OpenFont failed: " << SDL_GetError() << endl;
		return;
	}
}

void SimpleRenderer::init() {
	// Get Screen Data for Window creation
	simple.GetScreenData();
	// Creating the Camera
	// Creating the Main Window
	simple.window = Create_Window("Simple Render Main");
	simple.renderer = Create_Renderer(simple.window);
	// Creating the Depth Buffer
	simple.DepthBuffer = simple.CreateDepthBuffer();
	// Creating the Text Renderer
	simple.TextEngine = Create_TextEngine(simple.renderer);
	simple.Get_TTF_Fonts();
}

void SimpleRenderer::render() {
	if (debug == true) { cout << "[DEBUG] function simple.render() from SimpleRenderer.cpp" << endl; }
	RenderStartTime = SDL_GetTicks();
	// Clear the Main Window
	SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
	SDL_RenderClear(simple.renderer);
	for (auto& row : DepthBuffer)
		fill(row.begin(), row.end(), NULL);
	DepthBufferMax = 0.0f;
	DepthBufferMin = 1000000.0f;
	// Draw the Main Window
	simple.draw();
	// Render the Depth Buffer
	if (DepthBufferShown == true) {
		// Clear the Main Window
		SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
		SDL_RenderClear(simple.renderer);
		int i, j;
		float a;
		for (i = 0; i < DepthBuffer.size(); i++) {
			for (j = 0; j < DepthBuffer[0].size(); j++) {
				a = (DepthBuffer[i][j] - DepthBufferMin) / (DepthBufferMax - DepthBufferMin);
				//cout << fixed << setprecision(3) << a << " " << DepthBuffer[i][j] << endl;
				if (a < 0.0f) { a = 1.0f; }
				if (a > 1.0f) { a = 0.0f; }
				//cout << fixed << setprecision(2) << a << endl;
				a = 1.0f - a;
				SDL_SetRenderDrawColorFloat(simple.renderer, a, a, a, 1.0f);
				SDL_RenderPoint(simple.renderer, i, j);
			}
		}
		if (debug == true) {
			cout << "DepthBufferMax: " << DepthBufferMax << endl;
			cout << "DepthBufferMin: " << DepthBufferMin << endl;
		}
	}
	simple.TextRender();
	SDL_RenderPresent(simple.renderer);
	RenderEndTime = SDL_GetTicks();
	RenderTime = RenderEndTime - RenderStartTime;
}

void SimpleRenderer::TextRender() {
	if (debug == true) { cout << "[DEBUG] function simple.TextRender() from SimpleRenderer.cpp" << endl; }
	const char* ReportChar = Report.c_str();
	TTF_Text* ReportText = TTF_CreateText(simple.TextEngine, simple.ReportFont, ReportChar, strlen(ReportChar));
	TTF_SetTextColor(ReportText, 0, 0, 0, 255);
	TTF_DrawRendererText(ReportText, 0, 0);
}

void SimpleRenderer::draw() {
	if (debug == true) { cout << "[DEBUG] function simple.draw() from SimpleRenderer.cpp" << endl; }
	// Draw coordinate system lines
	float temp = 1.5f;
	ScreenPos Origin = Projection(Pos(0.0f, 0.0f, 0.0f));
	if(0.0f > Camera.pos.z + 0.25f) {
		simple.DrawLine(Origin, Projection(Pos(temp, 0.0f, 0.0f)), RGBA_int(255,0,0,255));
		simple.DrawLine(Origin, Projection(Pos(0.0f, temp, 0.0f)), RGBA_int(0,255,0,255));
		simple.DrawLine(Origin, Projection(Pos(0.0f, 0.0f, temp)), RGBA_int(0,0,255,255));
	}
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
	float FrontDepth = A.z - simple.Camera.pos.z - r;
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
	float x = A.x - simple.Camera.pos.x;
	float y = A.y - simple.Camera.pos.y;
	float z = A.z - simple.Camera.pos.z;
	y *= -1;
	float screenx = (x / z) * simple.RenderScale + ScreenWidthF / 2.0f;
	float screeny = (y / z) * simple.RenderScale + ScreenHeightF / 2.0f;
	return ScreenPos(screenx, screeny, z);
}

bool SimpleRenderer::CheckScreenPos(ScreenPos A) {
	if (A.x < 0.0f or A.x > ScreenWidthF or A.y < 0.0f or A.y > ScreenHeightF) {
		return false;
	}
	return true;
}

void SimpleRenderer::DrawTriangle(Triangle T) {
	if(T.p1.pos.z - Camera.pos.z < 0.3f or T.p2.pos.z - Camera.pos.z < 0.3f or T.p3.pos.z - Camera.pos.z < 0.3f) {
		if (debug == true) cout << "Triangle " << T.name << " is behind the camera and will not be drawn." << endl;
		return;
	}
	RGBA_int ColorInt = FloatToIntColor(T.color);
	SDL_SetRenderDrawColor(simple.renderer, ColorInt.r, ColorInt.g, ColorInt.b, ColorInt.a);
	// Get Screen Coordinates
	ScreenPos A = Projection(T.p1.pos);
	ScreenPos B = Projection(T.p2.pos);
	ScreenPos C = Projection(T.p3.pos);
	// Check if all Points are on Screen
	if (!CheckScreenPos(A) or !CheckScreenPos(B) or !CheckScreenPos(C)) {
		if (debug == true) cout << "Triangle " << T.name << " is partially off screen and will not be drawn." << endl;
		return;
	}
	// Sort by smallest y
	ScreenPos temp = A;
	if (B.y < A.y) { temp = B; B = A; A = temp; }
	if (C.y < A.y) { temp = C; C = A; A = temp; }
	if (C.y < B.y) { temp = C; C = B; B = temp; }
	if (debug == true) cout << "sort result: " << A.y << " " << B.y << " " << C.y << endl;

	// Drawing the WireFrame
	// Get Direction Vectors for AB,BC and AC
	ScreenPos DV_AB = ScreenPos(B.x - A.x, B.y - A.y, B.z - A.z);
	ScreenPos DV_BC = ScreenPos(C.x - B.x, C.y - B.y, C.z - B.z);
	ScreenPos DV_AC = ScreenPos(C.x - A.x, C.y - A.y, C.z - A.z);
	// SHADING PREREQUISITES
	float maxZ = max(A.z, max(B.z, C.z));
	float minZ = min(A.z, min(B.z, C.z));
	float diffZ = maxZ-minZ;
	float shade;
	float shadeIntensity = 0.4f;
	RGBA_int LocalColor = ColorInt;
	// Get Step Vectors for AB,BC and AC
	int sAB = abs(A.y - B.y); // Step Count between A and B
	ScreenPos SV_AB = ScreenPos(DV_AB.x / sAB, DV_AB.y / sAB, DV_AB.z / sAB);
	int sBC = abs(B.y - C.y); // Step Count between B and C
	ScreenPos SV_BC = ScreenPos(DV_BC.x / sBC, DV_BC.y / sBC, DV_BC.z / sBC);
	int sAC = abs(A.y - C.y); // Step Count between A and C
	ScreenPos SV_AC = ScreenPos(DV_AC.x / sAC, DV_AC.y / sAC, DV_AC.z / sAC);
	if (sAC > ScreenHeight / 2) {
		if (debug == true) cout << "Triangle " << T.name << " is too large and will not be drawn." << endl;
		return;
	}
	// Current Position for AB and AC
	ScreenPos C_AB = A;
	ScreenPos C_AC = A;
	// Drawing the Triangle from Ay to By
	// Handle if sAB == 0 to avoid division by zero
	if(sAB == 0) {
		sAB = 1; 
		DrawLine(A, C, ColorInt);
	}
	// Current Position for Scanline
	ScreenPos SC = C_AB;
	// Vector of Scanline
	ScreenPos SV_SC = A;
	SV_SC.y = 0.0f;
	// Actually Draw the Triangle from Ay to By
	int Ay = static_cast<int>(A.y);
	for (int i = 0; i <= static_cast<int>(sAB); i++) {
		// Interpolating the Scanline between C_AB and C_AC
		SC = C_AB; // Set Scanline to C_AB
		SV_SC.x = C_AC.x - C_AB.x;
		SV_SC.z = C_AC.z - C_AB.z;
		for (float j = 0.0f; j < abs(SV_SC.x); j++) {
			if (DepthBufferPoint(SC)) {
				shade = (SC.z - minZ) / diffZ;
				LocalColor = ModifyColor(1.0f - shade, shadeIntensity, ColorInt);
				SDL_SetRenderDrawColor(simple.renderer, LocalColor.r, LocalColor.g, LocalColor.b, LocalColor.a);
				SDL_RenderPoint(simple.renderer, SC.x, Ay+i);
			}
			SC.x += SV_SC.x / abs(SV_SC.x);
			SC.z += SV_SC.z / abs(SV_SC.x);
		}
		C_AB.x += SV_AB.x;
		C_AB.y += SV_AB.y;
		C_AB.z += SV_AB.z;
		C_AC.x += SV_AC.x;
		C_AC.y += SV_AC.y;
		C_AC.z += SV_AC.z;
	}
	// Drawing the Triangle from By to Cy
	// Handle if sAB == 0 to avoid division by zero
	if (sBC == 0) {
		sBC = 1;
		DrawLine(B, C, ColorInt);
	}
	// Draw the Triangle
	ScreenPos C_BC = B;
	// Current Position for Scanline
	SC = C_BC;
	// Vector of Scanline
	SV_SC = B;
	SV_SC.y = 0.0f;
	int By = static_cast<int>(B.y);
	// Actually Draw the Triangle from By to Cy
	for (int i = 0; i < static_cast<int>(sBC); i++) {
		// Interpolating the Scanline between C_AB and C_AC
		SC = C_BC; // Set Scanline to C_BC
		SV_SC.x = C_AC.x - C_BC.x;
		SV_SC.z = C_AC.z - C_BC.z;
		for (float j = 0.0f; j < abs(SV_SC.x); j++) {
			if (DepthBufferPoint(SC)) {
				shade = (SC.z - minZ) / diffZ;
				LocalColor = ModifyColor(1.0f - shade, shadeIntensity, ColorInt);
				SDL_SetRenderDrawColor(simple.renderer, LocalColor.r, LocalColor.g, LocalColor.b, LocalColor.a);
				SDL_RenderPoint(simple.renderer, SC.x, By+i);
			}
			SC.x += SV_SC.x / abs(SV_SC.x);
			SC.z += SV_SC.z / abs(SV_SC.x);
		}
		C_BC.x += SV_BC.x;
		C_BC.y += SV_BC.y;
		C_BC.z += SV_BC.z;
		C_AC.x += SV_AC.x;
		C_AC.y += SV_AC.y;
		C_AC.z += SV_AC.z;
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

	for (float i = 0; i <= r; i++) {
		if (DepthBufferPoint(C)) {
			SDL_RenderPoint(simple.renderer, C.x, C.y);
		}
		C.x += SV.x;
		C.y += SV.y;
		C.z += SV.z;
	}
	
}

bool SimpleRenderer::DepthBufferPoint(ScreenPos A) {
	int x = static_cast<int>(A.x);
	int y = static_cast<int>(A.y);
	if (x < 0 or y < 0 or x >= DepthBuffer.size() or y >= DepthBuffer[0].size()) return false; // Check if Point is on screen
	if (DepthBuffer[x][y] == NULL or DepthBuffer[x][y] > A.z) {
		if (A.z > DepthBufferMax) DepthBufferMax = A.z;
		if (A.z < DepthBufferMin) DepthBufferMin = A.z;
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
	//simple.DrawSphere(A.pos, 0.1f, Color);
}

SimpleRenderer simple;