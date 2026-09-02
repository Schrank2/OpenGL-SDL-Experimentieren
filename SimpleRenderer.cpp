#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include "defs.h"
#include "SimpleRenderer.h"
#include <vector>
#include <thread>
#include <mutex>
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
	flags = NULL; // SDL_WINDOW_ALWAYS_ON_TOP;
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
	simple.DepthBuffer.resize(simple.ScreenHeight * simple.ScreenWidth, FarPlane);
	simple.TextEngine = Create_TextEngine(simple.renderer);
	simple.Get_TTF_Fonts();
	simple.EmptyScreen = simple.pixels;
	simple.EmptyDepthBuffer = simple.DepthBuffer;
	int i = 0;
	for (; i < simple.ThreadAllocation; i++) {
		//simple.threads.push_back(thread());
		//simple.ThreadedPixels.push_back(vector<Uint32>(simple.ScreenHeight * simple.ScreenWidth, 0));
		//simple.ThreadedDepthBuffer.push_back(vector<float>(simple.ScreenHeight * simple.ScreenWidth, 0));
	}
}

void SimpleRenderer::render(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue) {
	if (debug == true) { cout << "[DEBUG] function simple.render() from SimpleRenderer.cpp" << endl; }
	RenderStartTime = SDL_GetTicks();
	SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
	SDL_RenderClear(simple.renderer);
	SDL_SetRenderTarget(simple.renderer, simple.canvas);
	SDL_RenderClear(simple.renderer);
	pixels = simple.EmptyScreen;
	DepthBuffer = simple.EmptyDepthBuffer;
	simple.draw(LineQueue, TriangleQueue, PointQueue);
	// Draw the Depth Buffer
	if (mainInput[7].active == true) {
		// Clear the Main Window
		SDL_SetRenderDrawColor(simple.renderer, 255, 255, 255, 255);
		SDL_RenderClear(simple.renderer);
		float i, j;
		RGBA_int c = RGBA_int(0, 0, 0, 255);
		float a;
		float DepthBufferRange = FarPlane - NearPlane;
		for (i = 0; i < ScreenWidth; i++) {
			for (j=0; j< ScreenHeight; j++) {
				a = (DepthBuffer[j * ScreenWidth + i] - NearPlane) / (DepthBufferRange);
				//cout << fixed << setprecision(3) << a << " " << DepthBuffer[i][j] << endl;
				//cout << fixed << setprecision(2) << a << endl;
				a = a / DepthBufferRange;
				SDL_SetRenderDrawColorFloat(simple.renderer, a, a, a, 1.0f);
				c = RGBA_int(255 * a, 255 * a, 255 * a, 255);
				DrawPixel(&i, &j, &c, &pixels);
			}
		}
		if (debug == true) {
			cout << "DepthBufferMax: " << FarPlane << endl;
			cout << "DepthBufferMin: " << NearPlane << endl;
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

void SimpleRenderer::TriangleRenderThread(int thread, vector<Triangle> ThreadedTriangleQueue) {
	vector<Uint32> ThreadPixels = EmptyScreen;
	vector<float> ThreadDepthBuffer = EmptyDepthBuffer;
	for (int j = 0; j < ThreadedTriangleQueue.size(); j++) {
		simple.DrawTriangle(&ThreadedTriangleQueue[j].p1.position, &ThreadedTriangleQueue[j].p2.position, &ThreadedTriangleQueue[j].p3.position, &ThreadedTriangleQueue[j].color, &ThreadPixels, &ThreadDepthBuffer);
		PolyGonsRenderedPerThread[thread]++;
	}
	ThreadedPixels[thread] = std::move(ThreadPixels);
	ThreadedDepthBuffer[thread] = std::move(ThreadDepthBuffer);
}

void SimpleRenderer::draw(vector<Line>* LineQueue, vector<Triangle>* TriangleQueue, vector<Point>* PointQueue) {
	if (debug == true) { cout << "[DEBUG] function simple.draw() from SimpleRenderer.cpp" << endl; }
	
	// Draw all triangles from world
	int ThreadsUsed = TriangleQueue->size() < ThreadAllocation ? TriangleQueue->size() : ThreadAllocation;
	ThreadsUsed = 1;
	int TrianglesPerThread = TriangleQueue->size() / ThreadsUsed;
	int start = 0;
	int end = 0;
	if (debug) cout << "Threads used: " << ThreadsUsed << endl;
	PolyGonsRenderedTotal = 0;
	PolyGonsRenderedPerThread.clear();
	TriangleDrawingTime = SDL_GetTicks();
	for (int i = 0; i < ThreadsUsed; i++) {
		TriangleRenderThreadInitialisation(i, TrianglesPerThread, TriangleQueue);
	}
	
	for(thread& t: threads) {
		t.join();
	}
	threads.clear();
	TriangleDrawingTime = SDL_GetTicks() - TriangleDrawingTime;

	for (int i = 0; i < ThreadsUsed; i++) {
		PolyGonsRenderedTotal += PolyGonsRenderedPerThread[i];
	}
	DepthBufferMergingTime = SDL_GetTicks();
	DepthBufferMin = FarPlane;
	int index = 1;
	float CurrentValue = 0.0f;
	float OldValue = 0.0f;
	for(int i = 0; i < ScreenWidth; i++) {
		for(int j = 0; j < ScreenHeight; j++) {
			index = j * ScreenWidth + i;
			for(int t = 0; t < ThreadsUsed; t++) {
				CurrentValue = ThreadedDepthBuffer[t][index];
				OldValue = DepthBuffer[index];
				if(CurrentValue > NearPlane and CurrentValue < FarPlane) 
					if (CurrentValue < OldValue) {
						DepthBuffer[index] = CurrentValue;
						pixels[index] = ThreadedPixels[t][index];
						if (DepthBufferMin > CurrentValue) DepthBufferMin = CurrentValue;
					}
			}
		}
	}	
	ThreadedPixels.clear();
	ThreadedDepthBuffer.clear();
	DepthBufferMergingTime = SDL_GetTicks() - DepthBufferMergingTime;
}

void SimpleRenderer::TriangleRenderThreadInitialisation(int thread, int TrianglesPerThread, vector<Triangle>* TriangleQueue) {
	vector<Triangle> ThreadTriangles;
	ThreadedPixels.push_back(EmptyScreen);
	ThreadedDepthBuffer.push_back(EmptyDepthBuffer);
	PolyGonsRenderedPerThread.push_back(0);
	
	int start = thread * TrianglesPerThread;
	int end = (thread + 1) * TrianglesPerThread;
	// special case for the last thread to take any remaining triangles
	end = end > TriangleQueue->size() ? TriangleQueue->size() : end;
	start = start > end ? end : start;

	ThreadTriangles.clear();
	for (int j = start; j < end; j++) {
		ThreadTriangles.push_back((*TriangleQueue)[j]);
	}
	threads.emplace_back(&SimpleRenderer::TriangleRenderThread, this, thread, ThreadTriangles);
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
	cout << "Sphere Radius Screen Space: " << R << endl;
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
				if (DepthBufferPoint(L, &DepthBuffer)) { // checking if the point is in front in the depth Buffer
					DrawPixel(&L.x, &L.y, &Localc, &pixels);
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

ScreenPos SimpleRenderer::Projection(Pos* A3D) {
	float pi = 3.14f;
	float x1 = A3D->x - simple.Camera.pos.x;
	float y1 = A3D->y - simple.Camera.pos.y;
	float z1 = A3D->z - simple.Camera.pos.z;
	float Yaw = CameraYaw * (pi / 180.0f);
	float Pitch = CameraPitch * (pi / 180.0f);
	float x2 = cos(Yaw) * x1 - sin(Yaw) * z1;
	float z2 = cos(Yaw) * z1 + sin(Yaw) * x1;
	float y2 = y1;
	if (z2 <= NearPlane) return ScreenPos(0, 0, 0, false);
	y2 *= -1;
	float screenx = (x2 / z2) * simple.RenderScale + ScreenWidthF / 2.0f;
	float screeny = (y2 / z2) * simple.RenderScale + ScreenHeightF / 2.0f;
	return ScreenPos(screenx, screeny, z2, true);
}

bool SimpleRenderer::CheckScreenPos(ScreenPos A) {
	if (A.x < 0.0f or A.x >= ScreenWidthF or A.y < 0.0f or A.y >= ScreenHeightF) {
		return false;
	}
	return true;
}

void SimpleRenderer::DrawTriangle(Pos* A3D, Pos* B3D, Pos* C3D, RGBA_int* Color, vector<Uint32>* ThreadPixels, vector<float>* ThreadDepthBuffer) {
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
	diffZ = diffZ <= 0.0f ? 0.000001f : diffZ;
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
	int x;
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
		rx = rx < ScreenWidth ? rx : ScreenWidth; // Clipping if maxX > ScreenWidth
		lx = lx > 0 ? lx : 0; // Clipping if minX < 0
		if (y > 0 and y < ScreenHeight) {
			DrawScanLine(&y, &lx, &lz, &rx, &rz, Color, &diffZ, &shadeIntensity, ThreadPixels, ThreadDepthBuffer);
		}
	}
}

void SimpleRenderer::DrawScanLine(int* y, int* leftx, float* leftz, int* rightx, float* rightz, RGBA_int* Color, float* DiffZ, float* shadeIntensity, vector<Uint32>* ThreadPixels, vector<float>* ThreadDepthBuffer) {
	int x;
	float z,r, shade;
	RGBA_int LocalColor = *Color;
	int span = static_cast<int>(*rightx - *leftx);
	if (span < 1) return;
	ScreenPos P = { static_cast<float>(*leftx), static_cast<float>(*y), static_cast<float>(*leftz), true };
	if (!P.valid) return;
	P = { static_cast<float>(*rightx), static_cast<float>(*y), static_cast<float>(*rightz), true };
	if (!P.valid) return;

	for (x = *leftx; x < *rightx; x++) {
			P.x = x;
			P.y = *y;
			r = static_cast<float>(x - *leftx) / static_cast<float>(*rightx - *leftx);
			P.z = *leftz + r * (*rightz - *leftz);
			if (DepthBufferPoint(P, ThreadDepthBuffer)) {
				shade = fabs(P.z - *leftz) / *DiffZ;
				LocalColor = ModifyColor(1.0f - shade, *shadeIntensity, *Color);
				LocalColor.a = 255;
				DrawPixel(&P.x, &P.y, &LocalColor, ThreadPixels);
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
		if (DepthBufferPoint(CurrentPos, &DepthBuffer)) DrawPixel(&CurrentPos.x, &CurrentPos.y, c, &pixels);
		return;
	}
	ScreenPos StepVector = ScreenPos(DirectionVectorAB.x * StepSize, DirectionVectorAB.y * StepSize, DirectionVectorAB.z * StepSize, true);

	for (float i = 0; i <= StepCount; i++) {
		if (DepthBufferPoint(CurrentPos, &DepthBuffer)) {
			DrawPixel(&CurrentPos.x, &CurrentPos.y, c, &pixels);
		}
		CurrentPos.x += StepVector.x;
		CurrentPos.y += StepVector.y;
		CurrentPos.z += StepVector.z;
	}
	
}

void SimpleRenderer::DrawPixel(float* x, float* y, RGBA_int* c, vector<Uint32>* ThreadPixels) {
	int index = static_cast<int>(*y * ScreenWidth + *x);
	if (index < ScreenWidth * ScreenHeight and index >= 0) {
		(*ThreadPixels)[index] = ((*c).r << 24U) | ((*c).g << 16U) | ((*c).b << 8U) | (*c).a;
	}
}

void SimpleRenderer::GetVector(float Vector[3], float Start[3], float End[3]) {
	Vector[0] = End[0] - Start[0];
	Vector[1] = End[1] - Start[1];
	Vector[2] = End[2] - Start[2];
}


bool SimpleRenderer::DepthBufferPoint(ScreenPos A, vector<float>* ThreadDepthBuffer) {
	CheckScreenPos(A);
	if (!A.valid or A.z > FarPlane or A.z < NearPlane) return false;
	int x = static_cast<int>(A.x);
	int y = static_cast<int>(A.y);
	int index = y * ScreenWidth + x;
	if (A.z < (*ThreadDepthBuffer)[index]) {
		(*ThreadDepthBuffer)[index] = A.z;
		return true;
	}
	return false;
}

float SimpleRenderer::DistBetweenPoints(Pos a, Pos b) {
	Pos v = Pos(a.x - b.x, a.y - b.y, a.z - b.z);
	return sqrt(v.x*v.x + v.y * v.y + v.z * v.z);
}

void SimpleRenderer::DrawPoint(Point* A) {
	ScreenPos ScreenA = Projection(&A->position);
	if (debug == true) { cout << "[DEBUG] Drawing Point: " << A->letter << " on Canvas at (" << ScreenA.x << ", " << ScreenA.y << ")" << endl; }
	//simple.DrawSphere(A.pos, 0.05f, A.color);
}

SimpleRenderer simple;