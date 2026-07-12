#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "functions.h"
#include "defs.h"
#include "SimpleRenderer.h"
#include "world.h"
#include "input.h"
#include <vector>
#include <iomanip> // basically settings for cout
#include <algorithm> // for clamp()

void SimpleRenderer::GetScreenData(int* ScreenWidth, int* ScreenHeight) {
	const SDL_DisplayMode* info = SDL_GetDesktopDisplayMode(1);
	if (!info)
	{
		cout << "Video query failed: " << SDL_GetError() << endl;
		exit(1);
	}
	*ScreenWidth = info->w;
	*ScreenHeight = info->h;
}

SDL_Window* SimpleRenderer::Create_Window(string title) {
	if (debug == true) { cout << "[DEBUG] function simple.Create_Window() from SimpleRenderer.cpp" << endl; }
	string WindowTitle;
	flags = SDL_WINDOW_ALWAYS_ON_TOP;
	WindowTitle = title + " " + to_string(ScreenWidth) + "x" + to_string(ScreenHeight);
	const char* WindowTitleChar = WindowTitle.c_str();
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

void SimpleRenderer::init(int* ScreenWidth, int* ScreenHeight) {
	simple.ScreenWidth = *ScreenWidth;
	simple.ScreenHeight = *ScreenHeight;
	simple.ScreenWidthF = static_cast<float>(simple.ScreenWidth);
	simple.ScreenHeightF = static_cast<float>(simple.ScreenHeight);
	simple.window = Create_Window("Simple Render Main");
	simple.renderer = Create_Renderer(simple.window);
	simple.canvas = SDL_CreateTexture(simple.renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, simple.ScreenWidth, simple.ScreenHeight);
	simple.pixels.resize(simple.ScreenHeight * simple.ScreenWidth, 0);
	simple.DepthBuffer.resize(simple.ScreenHeight * simple.ScreenWidth, 0);
	simple.TextEngine = Create_TextEngine(simple.renderer);
	simple.Get_TTF_Fonts();
}

void SimpleRenderer::render(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue) {
	if (debug == true) { cout << "[DEBUG] function simple.render() from SimpleRenderer.cpp" << endl; }
	RenderStartTime = SDL_GetTicks();
	SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
	SDL_RenderClear(simple.renderer);
	SDL_SetRenderTarget(simple.renderer, simple.canvas);
	SDL_RenderClear(simple.renderer);
	fill(pixels.begin(), pixels.end(), 0);
	fill(DepthBuffer.begin(), DepthBuffer.end(), 0);
	DepthBufferMax = 0.0f;
	DepthBufferMin = 1000000.0f;
	simple.draw(LineQueue, TriangleQueue, PointQueue);
	// Draw the Depth Buffer
	if (mainInput[7].active == true) {
		// Clear the Main Window
		SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
		SDL_RenderClear(simple.renderer);
		float i, j;
		RGBA_int c = RGBA_int(0, 0, 0, 255);
		float a;
		for (i = 0; i < ScreenWidth; i++) {
			for (j=0; j< ScreenHeight; j++) {
				if (DepthBufferMax == NULL) DepthBufferMax = 0.0f;
				if (DepthBufferMin == NULL) DepthBufferMin = 1000.0f;
				a = (DepthBuffer[j * ScreenWidth + i] - DepthBufferMin) / (DepthBufferMax - DepthBufferMin);
				//cout << fixed << setprecision(3) << a << " " << DepthBuffer[i][j] << endl;
				if (a < 0.0f) { a = 1.0f; }
				if (a > 1.0f) { a = 0.0f; }
				//cout << fixed << setprecision(2) << a << endl;
				a = 1.0f - a;
				SDL_SetRenderDrawColorFloat(simple.renderer, a, a, a, 1.0f);
				c = RGBA_int(255 * a, 255 * a, 255 * a, 255);
				DrawPixel(&i, &j, &c);
			}
		}
		if (debug == true) {
			cout << "DepthBufferMax: " << DepthBufferMax << endl;
			cout << "DepthBufferMin: " << DepthBufferMin << endl;
		}
	}
	SDL_UpdateTexture(simple.canvas, 0, pixels.data(), ScreenWidth * sizeof(Uint32));
	if (mainInput[8].active) simple.TextRender();
	SDL_SetRenderTarget(simple.renderer, NULL);
	SDL_RenderTexture(simple.renderer, simple.canvas, 0, 0);
	SDL_RenderPresent(simple.renderer);
	RenderEndTime = SDL_GetTicks();
	RenderTime = RenderEndTime - RenderStartTime;
}

void SimpleRenderer::TextRender() {
	if (debug == true) { cout << "[DEBUG] function simple.TextRender() from SimpleRenderer.cpp" << endl; }
	int LastHeight = 0;
	for (int i = 0; i < Report.size(); i++) {
		const char* ReportChar = Report[i].c_str();
		TTF_Text* ReportText = TTF_CreateText(simple.TextEngine, simple.ReportFont, ReportChar, strlen(ReportChar));
		TTF_SetTextColor(ReportText, 0, 0, 0, 255);
		TTF_DrawRendererText(ReportText, 1, LastHeight);
		int w, h;
		TTF_GetTextSize(ReportText, &w, &h);
		LastHeight += h;
	}
}

void SimpleRenderer::draw(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue) {
	if (debug == true) { cout << "[DEBUG] function simple.draw() from SimpleRenderer.cpp" << endl; }
	// Draw all Lines from world
	for (int i = 0; i < static_cast<int>(LineQueue->size()); i++) {
		simple.DrawLine(&(*LineQueue)[i].p1.pos, &(*LineQueue)[i].p2.pos, &(*LineQueue)[i].color);
	}
	// Draw all points from world
	for (int i = 0; i < static_cast<int>(PointQueue->size()); i++) {
		simple.DrawPoint(&(*PointQueue)[i]);
	}
	// Draw all triangles from world
	for (int j = 0; j < static_cast<int>(TriangleQueue->size()); j++) {
		simple.DrawTriangle(&(*TriangleQueue)[j].p1.pos, &(*TriangleQueue)[j].p2.pos, &(*TriangleQueue)[j].p3.pos, &(*TriangleQueue)[j].color);
	}
}

void SimpleRenderer::DrawSphere(Pos A, float r, RGBA_int c) {
	ScreenPos As = Projection(&A);
	if (As.z + 0.3 <= r) return;
	Pos Front = Pos(A.x, A.y, A.z - r);
	ScreenPos ScreenFront = Projection(&Front);
	float FrontDepth = ScreenFront.z;
	// weirdly adjusting the radius for depth of A
	Pos Temp3D = Pos(A.x, A.y - r, A.z);
	ScreenPos Temp = Projection(&Temp3D);
	float R = As.y - Temp.y;
	cout << R << endl;
	// where the sphere is lit most brightly (temporary, will later be replaced)
	ScreenPos Light = ScreenPos(As.x - (R/2), As.y - (R/2), As.z, true);

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
				float center = ScreenDist(As, ScreenPos(X,j,0.0f, true)); // Distance Between Lightspot and Poin
				float x = center / R; // Distance Between Center and Point displayed between 0.0f and 1.0f.
				if (x < 0.0f) x = 0.0f;
				if (x > 1.0f) x = 1.0f;
				float z = sqrt(1.0f - x * x) * r; // Tiefenunterschied zwischen Punkt und Zentrum
				ScreenPos L = ScreenPos(X,j, FrontDepth - z,true);
				// shading the point
				float d = ScreenDist(Light, L); // Distance Between Lightspot and Point
				lshade = 1.0f - (d / R);
				RGBA_int Localc = ModifyColor(lshade, 0.4f, c);
				Localc.a = 255;
				// drawing
				if (DepthBufferPoint(L)) { // checking if the point is in front in the depth Buffer
					DrawPixel(&L.x, &L.y, &Localc);
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

ScreenPos SimpleRenderer::Projection(Pos* A) {
	float pi = 3.14f;
	float x1 = A->x - simple.Camera.pos.x; 
	float y1 = A->y - simple.Camera.pos.y;
	float z1 = A->z - simple.Camera.pos.z;
	float Yaw = CameraYaw * (pi / 180.0f);
	float Pitch = CameraPitch * (pi / 180.0f);
	float x2 = cos(Yaw) * x1 - sin(Yaw) * z1;
	float z2 = cos(Yaw) * z1 + sin(Yaw) * x1;
	float y2 = y1;
	if (z2 <= 0.1) return ScreenPos(0,0,0,false);
	y2 *= -1;
	float screenx = (x2 / z2) * simple.RenderScale + ScreenWidthF / 2.0f;
	float screeny = (y2 / z2) * simple.RenderScale + ScreenHeightF / 2.0f;
	return ScreenPos(screenx, screeny, z2, true);
}

bool SimpleRenderer::CheckScreenPos(ScreenPos A) {
	if (A.x < 0.0f or A.x > ScreenWidthF or A.y < 0.0f or A.y > ScreenHeightF) {
		return false;
	}
	return true;
}

void SimpleRenderer::DrawTriangle(Pos* A3D, Pos* B3D, Pos* C3D, RGBA_int* Color) {
	ScreenPos A = Projection(A3D);
	ScreenPos B = Projection(B3D);
	ScreenPos C = Projection(C3D);
	// Culling if fully behind camera
	if (!A.valid and !B.valid and !C.valid) return;
	// Sort by smallest y
	ScreenPos temp = A;
	if (B.y < A.y) { temp = B; B = A; A = temp; }
	if (C.y < A.y) { temp = C; C = A; A = temp; }
	if (C.y < B.y) { temp = C; C = B; B = temp; }
	if (debug == true) cout << "sort result: " << A.y << " " << B.y << " " << C.y << endl;

	// Drawing the WireFrame
	// Get Direction Vectors for AB,BC and AC
	ScreenPos DV_AB = ScreenPos(B.x - A.x, B.y - A.y, B.z - A.z, true);
	ScreenPos DV_BC = ScreenPos(C.x - B.x, C.y - B.y, C.z - B.z, true);
	ScreenPos DV_AC = ScreenPos(C.x - A.x, C.y - A.y, C.z - A.z, true);
	// SHADING PREREQUISITES
	float maxZ = max(A.z, max(B.z, C.z));
	float minZ = min(A.z, min(B.z, C.z));
	float diffZ = maxZ-minZ;
	float shade;
	float shadeIntensity = 0.4f;
	RGBA_int LocalColor = *Color;

	// Vectors
	ScreenPos AB = ScreenPos(B.x - A.x, B.y - A.y, B.z - A.z, true);
	ScreenPos AC = ScreenPos(C.x - A.x, C.y - A.y, C.z - A.z, true);
	ScreenPos BC = ScreenPos(C.x - B.x, C.y - B.y, C.z - B.z, true);
	ScreenPos f = AC;
	ScreenPos f0 = A;
	ScreenPos f1 = C;
	ScreenPos g = AB;
	ScreenPos g0 = A;
	ScreenPos g1 = B;
	float r; // parameter

	int a = 0;
	// Drawing the Triangle
	int x, maxX;
	int y = A.y > 1 ? A.y + 1 : 1; // Clipping if minY < 0
	int lx, rx, dx, dz;
	float lz, rz;
	ScreenPos P = A; // Current Position to Draw
	int maxY = C.y < ScreenHeight ? C.y : ScreenHeight; // Clipping if maxY > ScreenHeight
	for (; y <= maxY; y++) {
		if (y >= B.y) { g = BC; g0 = B; g1 = C; } // switch line g to BC
		// get x and z for line f = AC
		if (y - f0.y != 0) {
			r =  static_cast<float>(y - f0.y) / static_cast<float>(f1.y - f0.y);
			lx = f0.x + r * f.x;
			lz = f0.z + r * f.z;
		}
		else { lx = f0.x; lz = f0.z; }

		// get x and z for line g = AB, later BC
		if (y - g0.y != 0) {
			r = static_cast<float>(y - g0.y) / static_cast<float>(g1.y - g0.y);
			rx = g0.x + r * g.x;
			rz = g0.z + r * g.z;
		}
		else { rx = g0.x; rz = g0.z; }
		if (lx > rx) { dx = lx; lx = rx; rx = dx; dz = lz; lz = rz; rz = dz; }
		maxX = rx < ScreenWidth ? rx : ScreenWidth; // Clipping if maxX > ScreenWidth
		for (x = lx > 0 ? lx : 0; x < maxX; x++) { // Clipping if minX < 0
			P.x = x;
			P.y = y;
			r = static_cast<float>(x - lx) / static_cast<float>(rx - lx);
			P.z = lz + r * (rz - lz);
			if (DepthBufferPoint(P)) {
				shade = abs(P.z - minZ) / diffZ;
				LocalColor = ModifyColor(1.0f - shade, shadeIntensity, *Color);
				LocalColor.a = 255;
				DrawPixel(&P.x, &P.y, &LocalColor);
			}
		}
	}
}

void SimpleRenderer::DrawLine(Pos* A3D, Pos* B3D, RGBA_int* c) {
	ScreenPos A = Projection(A3D);
	ScreenPos B = Projection(B3D);
	ScreenPos DirectionVectorAB = ScreenPos(B.x - A.x, B.y - A.y, B.z - A.z, true);
	float StepCount = max(abs(DirectionVectorAB.x), abs(DirectionVectorAB.y));
	float StepSize = 1.0f / StepCount;
	ScreenPos CurrentPos = A;
	if (StepSize == 0) {
		if (DepthBufferPoint(CurrentPos)) DrawPixel(&CurrentPos.x, &CurrentPos.y, c);
		return;
	}
	ScreenPos StepVector = ScreenPos(DirectionVectorAB.x * StepSize, DirectionVectorAB.y * StepSize, DirectionVectorAB.z * StepSize, true);

	for (float i = 0; i <= StepCount; i++) {
		if (DepthBufferPoint(CurrentPos)) {
			DrawPixel(&CurrentPos.x, &CurrentPos.y, c);
		}
		CurrentPos.x += StepVector.x;
		CurrentPos.y += StepVector.y;
		CurrentPos.z += StepVector.z;
	}
	
}

void SimpleRenderer::DrawPixel(float* x, float* y, RGBA_int* c) {
	if (*y * ScreenWidth + *x < ScreenWidth * ScreenHeight) {
		pixels[static_cast<int>(*y * ScreenWidth + *x)] = ((*c).r << 24U) | ((*c).g << 16U) | ((*c).b << 8U) | (*c).a;
	}
}


bool SimpleRenderer::DepthBufferPoint(ScreenPos A) {
	if (!A.valid) return false;
	int x = static_cast<int>(A.x);
	int y = static_cast<int>(A.y);
	if (x < 0 or y < 0 or x >= ScreenWidth or y >= ScreenHeight) return false; // Check if Point is on screen
	if (DepthBuffer[y * ScreenWidth + x] == NULL or DepthBuffer[y * ScreenWidth + x] > A.z) {
		if (A.z > DepthBufferMax or DepthBufferMax == NULL) DepthBufferMax = A.z;
		if (A.z < DepthBufferMin or DepthBufferMin == NULL) DepthBufferMin = A.z;
		DepthBuffer[y * ScreenWidth + x] = A.z;
		return true;
	}
	return false;
}

float SimpleRenderer::DistBetweenPoints(Pos a, Pos b) {
	Pos v = Pos(a.x - b.x, a.y - b.y, a.z - b.z);
	return sqrt(v.x*v.x + v.y * v.y + v.z * v.z);
}

void SimpleRenderer::DrawPoint(Point* A) {
	ScreenPos ScreenA = Projection(&A->pos);
	if (debug == true) { cout << "[DEBUG] Drawing Point: " << A->letter << " on Canvas at (" << ScreenA.x << ", " << ScreenA.y << ")" << endl; }
	//simple.DrawSphere(A.pos, 0.05f, A.color);
}

SimpleRenderer simple;