#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <iomanip> // for more precise floats in cout
#include <thread>
#include <format> // for to_string floats to not show too many decimal numbers
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
	int FrameRateTarget = 240;
	int FrameStartTime = 0;
	int CurrentTime = 0;
	int LastReportTime = 0;
	int FPS = 0;

	int Ticktime = 0;
	float TickRateTarget = 40;
	world.TickStrength = TickRateTarget / 1000.0f;
	int TickStartTime = 0;
	float TPS = 0.0f;

	while (running) {
		CurrentTime = SDL_GetTicks();

		if (CurrentTime > TickStartTime + (1000 / TickRateTarget)) {
			Ticktime = CurrentTime - TickStartTime;
			world.TickStrength = static_cast<float>(Ticktime) / 1000.0f;
			world.tick();
			TickStartTime = SDL_GetTicks();
		}
		world.tick();
		// Rendering and Showing a Plane
		if (CurrentTime> FrameStartTime + (1000 / FrameRateTarget)) {
			simple.render();
			Frametime = CurrentTime - FrameStartTime;
			FrameStartTime = SDL_GetTicks();
		}
		if (CurrentTime >= LastReportTime + 1000) {
			LastReportTime = CurrentTime;
			if (report == true) {
				FPS = 1000.0f / static_cast<float>(Frametime);
				Report.clear();
				int SDLVersion = SDL_GetVersion();
				int TTFVersion = TTF_Version();
				Report.push_back("SDL Version: " + to_string(SDL_VERSIONNUM_MAJOR(SDLVersion)) + "." + to_string(SDL_VERSIONNUM_MINOR(SDLVersion)) + "." + to_string(SDL_VERSIONNUM_MICRO(SDLVersion)));
				Report.push_back("TTF Version: " + to_string(SDL_VERSIONNUM_MAJOR(TTFVersion)) + "." + to_string(SDL_VERSIONNUM_MINOR(TTFVersion)) + "." + to_string(SDL_VERSIONNUM_MICRO(TTFVersion)));
				Report.push_back("Camera Position x: " + std::format("{:.2f}",simple.Camera.pos.x) + " y: " + std::format("{:.2f}", simple.Camera.pos.y) + " z: " + std::format("{:.2f}", simple.Camera.pos.z));
				Report.push_back("Camera Velocity x: " + std::format("{:.2f}", simple.Camera.velocity.x) + " y: " + std::format("{:.2f}", simple.Camera.velocity.y) + " z: " + std::format("{:.2f}", simple.Camera.velocity.z));
				Report.push_back("Rendering Performance: " + to_string(FPS) + "fps | " + to_string(Frametime) + "ms");
			}
		}

		// Checking for Key inputs
		if (SDL_PollEvent(&event) && event.type == SDL_EVENT_KEY_DOWN) {
			if (event.key.key == SDLK_W) {
				world.KeyBoard.w = true;
			}
			if (event.key.key == SDLK_A) {
				world.KeyBoard.a = true;
			}
			if (event.key.key == SDLK_S) {
				world.KeyBoard.s = true;
			}
			if (event.key.key == SDLK_D) {
				world.KeyBoard.d = true;
			}
			if (event.key.key == SDLK_SPACE) {
				world.KeyBoard.space = true;
			}
			if (event.key.key == SDLK_LSHIFT) {
				world.KeyBoard.lshift = true;
			}
			if (event.key.key == SDLK_P) {
				world.KeyBoard.p = true;
			}
			if (event.key.key == SDLK_O) {
				world.KeyBoard.o = true;
			}
			if (event.key.key == SDLK_F3) {
				world.KeyBoard.f3 = true;
			}
		}
		if (event.type == SDL_EVENT_KEY_UP) {
			if (event.key.key == SDLK_W) {
				world.KeyBoard.w = false;
			}
			if (event.key.key == SDLK_A) {
				world.KeyBoard.a = false;
			}
			if (event.key.key == SDLK_S) {
				world.KeyBoard.s = false;
			}
			if (event.key.key == SDLK_D) {
				world.KeyBoard.d = false;
			}
			if (event.key.key == SDLK_SPACE) {
				world.KeyBoard.space = false;
			}
			if (event.key.key == SDLK_LSHIFT) {
				world.KeyBoard.lshift = false;
			}
			if (event.key.key == SDLK_P) {
				world.KeyBoard.p = false;
			}
			if (event.key.key == SDLK_O) {
				world.KeyBoard.o = false;
			}
			if (event.key.key == SDLK_F3) {
				world.KeyBoard.f3 = false;
			}
		}
		// Handle Reading Results of Key Inputs
		if (world.KeyBoard.o == true) {
			if (simple.DepthBufferShown == true) simple.DepthBufferShown = false;
			else simple.DepthBufferShown = true;
		}
		if (world.KeyBoard.f3 == true) world.DebugMenuShown = true;
		else world.DebugMenuShown = false;
		// Movement
		if (world.KeyBoard.w == true) simple.Camera.velocity.z += 0.001f * world.TickStrength;
		if (world.KeyBoard.a == true) simple.Camera.velocity.x -= 0.001f * world.TickStrength;
		if (world.KeyBoard.s == true) simple.Camera.velocity.z -= 0.001f * world.TickStrength;
		if (world.KeyBoard.d == true) simple.Camera.velocity.x += 0.001f * world.TickStrength;
		if (world.KeyBoard.space == true) simple.Camera.velocity.y += 0.001f * world.TickStrength;
		if (world.KeyBoard.lshift == true) simple.Camera.velocity.y -= 0.001f * world.TickStrength;
	}
}