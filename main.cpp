#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <iomanip> // for more precise floats in cout
using namespace std;
bool debug = true;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main() from main.cpp" << endl; }
	// World Setup
	world.init();
	// Rendering
	InitSdl();
	simple.init();
	simple.render();
	//TEST 3002016
	SDL_Event event;
	bool running = true;
	int FrameStartTime, FrameEndTime, FrameTime;
	int LastFrameRateReportTime = SDL_GetTicks();
	float Framerate;
	while (running) {
		// Rendering and Showing a Plane
		FrameStartTime = SDL_GetTicks();
		simple.render();
		FrameEndTime = SDL_GetTicks();
		if (FrameEndTime >= LastFrameRateReportTime + 1000) {
			LastFrameRateReportTime = FrameEndTime;
			FrameTime = FrameEndTime - FrameStartTime;
			Framerate = 1000.0f / static_cast<float>(FrameTime);
			cout << fixed << setprecision(2) <<  "[Report] Frametime: " << FrameTime << "ms Framerate: " << Framerate << " per Second" << endl;
		}

		// Checking for Key inputs
		if (SDL_PollEvent(&event) && event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_W) {
				simple.Camera.z += 0.5f;
				cout << "W" << endl;
			}
			if (event.key.key == SDLK_A) {
				simple.Camera.x -= 0.5f;
			}
			if (event.key.key == SDLK_S) {
				simple.Camera.z -= 0.5f;
			}
			if (event.key.key == SDLK_D) {
				simple.Camera.x += 0.5f;
			}
			if (event.key.key == SDLK_SPACE) {
				simple.Camera.y += 0.5f;
			}
			if (event.key.key == SDLK_LSHIFT) {
				simple.Camera.y -= 0.5f;
			}
			if (event.key.key == SDLK_P) {
				running = false;
			}
		}
	}
}