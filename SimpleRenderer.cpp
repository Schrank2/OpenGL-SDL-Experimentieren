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

void SimpleRenderer::init(int* ScreenWidth, int* ScreenHeight, int* ThreadsAllocated) {
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

	ThreadAllocation = *ThreadsAllocated;
	cout << "Threads Allocated: " << ThreadAllocation << endl;
	threads.resize(*ThreadsAllocated);
	PerThreadTriangleTime.resize(*ThreadsAllocated, 0);
}

void SimpleRenderer::render(vector<Line>* LineQueue, vector<SpaceTriangle>* TriangleQueue, vector<Point>* PointQueue) {
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
		float i, j = 0;
		int index = 0;
		int r, g, b, a = 0;
		RGBA_int Color = RGBA_int(0, 0, 0, 255);
		Uint32 CachedColor = 0;
		float ColorModifier = 0.0f;
		float Depth = 0.0f;
		float DepthBufferRange = DepthBufferMax - DepthBufferMin;
		for (i = 0; i < ScreenWidthF; i++) {
			for (j=0; j< ScreenHeightF; j++) {
				index = static_cast<int>(j * ScreenWidth + i);
				Depth = DepthBuffer[index];
				if (Depth < FarPlane) {
					ColorModifier = (Depth - DepthBufferMin) / DepthBufferRange;
					ColorModifier = 1.0f - ColorModifier;
					//cout << fixed << setprecision(3) << a << " " << DepthBuffer[i][j] << endl;
					//cout << fixed << setprecision(2) << a << endl;
					CachedColor = pixels[index];
					r = (CachedColor >> 24) & 0xFF;
					g = (CachedColor >> 16) & 0xFF;
					b = (CachedColor >> 8) & 0xFF;
					//a = (CachedColor >> 24) & 0xFF;
					Color = RGBA_int(r * ColorModifier, g * ColorModifier, b * ColorModifier, 255);
					DrawPixel(&i, &j, &Color, &pixels);
				}
			}
		}
		if (debug == true) {
			cout << "DepthBufferMax: " << FarPlane << endl;
			cout << "DepthBufferMin: " << NearPlane << endl;
		}
	}
	SDL_UpdateTexture(simple.canvas, 0, pixels.data(), ScreenWidth * sizeof(Uint32));
	SDL_SetRenderTarget(simple.renderer, NULL);
	SDL_RenderTexture(simple.renderer, simple.canvas, 0, 0);
	if (mainInput[8].active) simple.TextRender();
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

void SimpleRenderer::TriangleRenderThread(int ThreadIndex, vector<ScreenTriangle>* ProjectedTriangleQueue, int CanvasSnippetStart, int CanvasSnippetEnd) {
	PerThreadTriangleTime[ThreadIndex] = SDL_GetTicks();
	for (int j = 0; j < ProjectedTriangleQueue->size(); j++) {
		DrawTriangle(&(*ProjectedTriangleQueue)[j].p1, &(*ProjectedTriangleQueue)[j].p2, &(*ProjectedTriangleQueue)[j].p3, &(*ProjectedTriangleQueue)[j].color, &pixels, &DepthBuffer, CanvasSnippetStart, CanvasSnippetEnd);
	}
	PerThreadTriangleTime[ThreadIndex] = SDL_GetTicks() - PerThreadTriangleTime[ThreadIndex];
}

void SimpleRenderer::draw(vector<Line>* LineQueue, vector<SpaceTriangle>* TriangleQueue, vector<Point>* PointQueue) {
	if (debug == true) { cout << "[DEBUG] function simple.draw() from SimpleRenderer.cpp" << endl; }
	TriangleThreadSetupTime = SDL_GetTicks();

	// Draw all triangles from world
	int TriangleQueueSize = TriangleQueue->size();
	vector<ScreenTriangle> ProjectedTriangleQueue;
	ScreenPos temp = ScreenPos(0.0f, 0.0f, 0.0f, true);
	ScreenTriangle TempTriangle = ScreenTriangle(temp, temp, temp, RGBA_int(0, 0, 0, 0));
	ProjectedTriangleQueue.resize(TriangleQueueSize, TempTriangle);
	
	int start = 0;
	int end = 0;
	int ThreadsUsed = ThreadAllocation < TriangleQueueSize ? ThreadAllocation : TriangleQueueSize;
	int TrianglesPerThread = floor(ProjectedTriangleQueue.size() / static_cast<float>(ThreadsUsed));
	if (TrianglesPerThread < 1) TrianglesPerThread = 1;
	for (int i = 0; i < ThreadsUsed; i++) {
		start = i * TrianglesPerThread;
		end = (i + 1) * TrianglesPerThread;
		threads[i] = thread(&SimpleRenderer::ProjectTriangleCoords, this, start,end,i,TriangleQueue,&ProjectedTriangleQueue);
	}
	for (int j = 0; j < ThreadsUsed; j++) {
		threads[j].join();
	}

	//cout << "TriangleQueue Size: " << (*TriangleQueue).size() << endl;
	//cout << "ProjectedTriangleQueue Size: " << ProjectedTriangleQueue.size() << endl;

	TriangleDrawingTime = SDL_GetTicks();
	int PixelsPerThread = floor(ScreenHeightF / static_cast<float>(ThreadAllocation));
	for (int i = 0; i < ThreadAllocation; i++) {
		TriangleRenderThreadInitialisation(i, PixelsPerThread, &ProjectedTriangleQueue);
	}
	TriangleThreadSetupTime = SDL_GetTicks() - TriangleThreadSetupTime;
	
	int j = 0;
	for (; j < ThreadAllocation; j++) {
		threads[j].join();
	}
	TriangleDrawingTime = SDL_GetTicks() - TriangleDrawingTime;
	DepthBufferMergingTime = SDL_GetTicks();
	DepthBufferMin = FarPlane;
	
	ThreadedPixels.clear();
	ThreadedDepthBuffer.clear();
	DepthBufferMergingTime = SDL_GetTicks() - DepthBufferMergingTime;
}

void SimpleRenderer::ProjectTriangleCoords(int start, int stop, int CurrentThread, vector<SpaceTriangle>* TriangleQueue, vector<ScreenTriangle>* ProjectedTriangleQueue) {
	if (CurrentThread >= ThreadAllocation) {
		stop = TriangleQueue->size();
	}
	for(int i = start; i < stop; i++) {
		ScreenPos A = Projection(&(*TriangleQueue)[i].A);
		ScreenPos B = Projection(&(*TriangleQueue)[i].B);
		ScreenPos C = Projection(&(*TriangleQueue)[i].C);
		Mutex.lock();
		(*ProjectedTriangleQueue)[i] = ScreenTriangle(A, B, C, (*TriangleQueue)[i].color);
		Mutex.unlock();
	}
}

void SimpleRenderer::TriangleRenderThreadInitialisation(int ThreadIndex, int PixelsPerThread, vector<ScreenTriangle>* ProjectedTriangleQueue) {
	int CanvasSnippetStart = ThreadIndex * PixelsPerThread;
	int CanvasSnippetEnd = (ThreadIndex + 1) * PixelsPerThread;
	threads[ThreadIndex] = thread( &SimpleRenderer::TriangleRenderThread, this, ThreadIndex, ProjectedTriangleQueue, CanvasSnippetStart, CanvasSnippetEnd);
	//threads.emplace_back(&SimpleRenderer::TriangleRenderThread, this, thread, ThreadTriangles);
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
	float pi = 3.14159265358979323846f;
	float x1 = A3D->x - simple.Camera.pos.x;
	float y1 = A3D->y - simple.Camera.pos.y;
	float z1 = A3D->z - simple.Camera.pos.z;
	float Yaw = CameraYaw * (pi / 180.0f);
	float Pitch = CameraPitch * (pi / 180.0f);
	float CosYaw = cos(Yaw);
	float SinYaw = sin(Yaw);
	float x2 = CosYaw * x1 - SinYaw * z1;
	float z2 = CosYaw * z1 + SinYaw * x1;
	float y2 = y1;
	if (z2 <= NearPlane) return ScreenPos(0, 0, 0, false);
	y2 *= -1.0f;
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

void SimpleRenderer::DrawTriangle(ScreenPos* A, ScreenPos* B, ScreenPos* C, RGBA_int* Color, vector<Uint32>* Canvas, vector<float>* DepthBuffer, int CanvasSnippetStart, int CanvasSnippetEnd) {
	// Culling if fully behind camera
	bool AOnSnippet = IsOnScreenSnippet(A, 0, ScreenWidth, 0, ScreenHeight);
	bool BOnSnippet = IsOnScreenSnippet(B, 0, ScreenWidth, 0, ScreenHeight);
	bool COnSnippet = IsOnScreenSnippet(C, 0, ScreenWidth, 0, ScreenHeight);
	if (!AOnSnippet and !BOnSnippet and !COnSnippet) return;
	
	// Sort by smallest y
	ScreenPos temp = *A;
	if (B->y < A->y) { temp = *B; *B = *A; *A = temp; }
	if (C->y < A->y) { temp = *C; *C = *A; *A = temp; }
	if (C->y < B->y) { temp = *C; *C = *B; *B = temp; }
	if (debug == true) cout << "sort result: " << A->y << " " << B->y << " " << C->y << endl;

	// Drawing the WireFrame
	// Get Direction Vectors for AB,BC and AC
	ScreenPos DV_AB = ScreenPos(B->x - A->x, B->y - A->y, B->z - A->z, true);
	ScreenPos DV_BC = ScreenPos(C->x - B->x, C->y - B->y, C->z - B->z, true);
	ScreenPos DV_AC = ScreenPos(C->x - A->x, C->y - A->y, C->z - A->z, true);
	// SHADING PREREQUISITES
	float maxZ = max(A->z, max(B->z, C->z));
	float minZ = min(A->z, min(B->z, C->z));
	float shadeIntensity = 0.4f;
	RGBA_int LocalColor = *Color;

	// Vectors
	ScreenPos AB = ScreenPos(B->x - A->x, B->y - A->y, B->z - A->z, true);
	ScreenPos AC = ScreenPos(C->x - A->x, C->y - A->y, C->z - A->z, true);
	ScreenPos BC = ScreenPos(C->x - B->x, C->y - B->y, C->z - B->z, true);
	ScreenPos f = AC;
	ScreenPos f0 = *A;
	ScreenPos f1 = *C;
	ScreenPos g = AB;
	ScreenPos g0 = *A;
	ScreenPos g1 = *B;
	float r; // parameter

	int a = 0;
	// Drawing the Triangle
	float y = A->y;
	y = y > CanvasSnippetStart ? y : static_cast<float>(CanvasSnippetStart); // Clipping if minY < CanvasSnippetStart
	float lx, rx, dx, dz;
	float lz, rz;
	ScreenPos P = *A; // Current Position to Draw
	int maxY = C->y < CanvasSnippetEnd ? C->y : CanvasSnippetEnd; // Clipping if maxY > ScreenHeight
	for (; y <= maxY; y++) {
		if (y >= B->y) { g = BC; g0 = *B; g1 = *C; } // switch line g to BC
		// get x and z for line f = AC
		if (y - f0.y != 0) {
			r =  (y - f0.y) / (f.y);
			lx = f0.x + r * f.x;
			lz = f0.z + r * f.z;
		}
		else { lx = f0.x; lz = f0.z; }

		// get x and z for line g = AB, later BC
		if (y - g0.y != 0) {
			r = (y - g0.y) / (g1.y - g0.y);
			rx = g0.x + r * g.x;
			rz = g0.z + r * g.z;
		}
		else { rx = g0.x; rz = g0.z; }
		if (lx > rx) { dx = lx; lx = rx; rx = dx; dz = lz; lz = rz; rz = dz; }
		if (rx >= ScreenWidthF) rx = ScreenWidthF - 1.0f;
		lx = lx > 0.0f ? lx : 0.0f; // Clipping if minX < 0
		if (lx <= 0.0f) lx = 0.0f;
		DrawScanLine(&y, &lx, &lz, &rx, &rz, Color, &minZ, &maxZ, &shadeIntensity, Canvas, DepthBuffer);
	}
}

bool SimpleRenderer::IsOnScreenSnippet(ScreenPos* A, int MinX, int MaxX, int MinY, int MaxY) {
	if (A->z < NearPlane or A->z > FarPlane) return false;
	if (A->x < MinX or A->x >= MaxX or A->y < MinY or A->y >= MaxY) {
		return false;
	}
	return true;
}

void SimpleRenderer::DrawScanLine(float* y, float* leftx, float* leftz, float* rightx, float* rightz, RGBA_int* Color, float* minZ, float* maxZ, float* shadeIntensity, vector<Uint32>* Canvas, vector<float>* DepthBuffer) {
	float yf = floor(*y);
	int x;
	float progress;
	RGBA_int LocalColor = *Color;
	float spanX = *rightx - *leftx;
	float spanZ = *rightz - *leftz;
	if (spanX < 1.0f) return;
	float stepZ = spanZ / spanX;
	ScreenPos P = ScreenPos(*leftx, yf, *leftz, true);
	for (x = *leftx; x < *rightx; x++) {
			P.x = floor(x);
			progress = (x - *leftx) / spanX;
			P.z += stepZ;
			if (DepthBufferPoint(P, DepthBuffer)) {
				LocalColor = ModifyColor(1.0f - progress, *shadeIntensity, *Color);
				LocalColor.a = 255;
				DrawPixel(&P.x, &P.y, &LocalColor, Canvas);
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
	if (!CheckScreenPos(A) or !A.valid or A.z > FarPlane or A.z < NearPlane) return false;
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