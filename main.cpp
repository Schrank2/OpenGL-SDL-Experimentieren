#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <iomanip> // for more precise floats in cout
using namespace std;
bool debug = false;
bool report = true;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main() from main.cpp" << endl; }
	// World Setup
	world.init();
	// Initialising Libraries and Rendering
	init_libs();
	simple.init();
	//TEST 3002016
	SDL_Event event;
	bool running = true;
	int Frametime = 0;
	int FrameRateTarget = 60;
	int FrameStartTime = 0;
	int CurrentTime = 0;
	int LastReportTime = 0;
	float FPS = 0.0f;
	while (running) {
		// Rendering and Showing a Plane
		CurrentTime = SDL_GetTicks();
		if (CurrentTime> FrameStartTime + (1000 / FrameRateTarget)) {
			simple.render();
			Frametime = CurrentTime - FrameStartTime;
			FrameStartTime = CurrentTime;
		}
		if (CurrentTime >= LastReportTime + 1000) {
			LastReportTime = CurrentTime;
			if (report == true) {
				FPS = 1000.0f / static_cast<float>(Frametime);
				Report = "Frametime: " + to_string(Frametime) + "ms Framerate: " + to_string(static_cast<int>(FPS)) + " per Second RenderTime: " + to_string(simple.RenderTime) + "ms";
				const char* ReportTemp = Report.c_str();
				//SDL_SetWindowTitle(simple.window, ReportTemp);
			}
		}

		// Checking for Key inputs
		if (SDL_PollEvent(&event) && event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_W) {
				simple.Camera.z += 0.1f;
			}
			if (event.key.key == SDLK_A) {
				simple.Camera.x -= 0.1f;
			}
			if (event.key.key == SDLK_S) {
				simple.Camera.z -= 0.1f;
			}
			if (event.key.key == SDLK_D) {
				simple.Camera.x += 0.1f;
			}
			if (event.key.key == SDLK_SPACE) {
				simple.Camera.y += 0.1f;
			}
			if (event.key.key == SDLK_LSHIFT) {
				simple.Camera.y -= 0.1f;
			}
			if (event.key.key == SDLK_P) {
				running = false;
			}
			if (event.key.key == SDLK_O) {
				if (simple.DepthBufferShown == true) simple.DepthBufferShown = false;
					else simple.DepthBufferShown = true;
			}
		}
	}
}