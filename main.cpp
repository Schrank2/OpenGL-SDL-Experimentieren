#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include "input.h"
#include <iomanip> // for more precise floats in cout
#include <thread>
#include <format> // for to_string floats to not show too many decimal numbers
using namespace std;
bool debug = false;
bool report = true;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main() from main.cpp" << endl; }
	world.init();
	init_libs();
	simple.init();
	bool running = true;
	float Frametime = 0.0f;
	float FrameRateTarget = 240.0f;
	float FrameTimeTarget = static_cast<int>(1000 / FrameRateTarget);
	float FrameStartTime = 0.0f;
	float CurrentTime = 0.0f;
	float LastReportTime = 0.0f;
	float FPS = 0.0f;
	float Ticktime = 0.0f;
	float TickRateTarget = 40.0f;
	float TickTimeTarget = 1000.0f / TickRateTarget;
	world.TickStrength = TickRateTarget / 1000.0f;
	float TickStartTime = 0.0f;
	float TPS = 0.0f;
	bool Pause = false;
	InputMK KEYBOARD;

	while (running) {
		CurrentTime = SDL_GetTicks();

		if (CurrentTime > TickStartTime + TickTimeTarget) {
			Ticktime = CurrentTime - TickStartTime;
			world.TickStrength = Ticktime / 1000.0f;
			world.tick();
			TickStartTime = SDL_GetTicks();
		}
		world.tick();
		// Rendering and Showing a Plane
		if (CurrentTime> FrameStartTime + FrameTimeTarget) {
			simple.render();
			Frametime = CurrentTime - FrameStartTime;
			FrameStartTime = SDL_GetTicks();
		}
		if (CurrentTime >= LastReportTime + TickRateTarget) {
			LastReportTime = CurrentTime;
			if (report == true) {
				FPS = 1000.0f / Frametime;
				Report.clear();
				int SDLVersion = SDL_GetVersion();
				int TTFVersion = TTF_Version();
				if (Pause) Report.push_back("---GAME PAUSED---");
				Report.push_back("SDL Version: " + to_string(SDL_VERSIONNUM_MAJOR(SDLVersion)) + "." + to_string(SDL_VERSIONNUM_MINOR(SDLVersion)) + "." + to_string(SDL_VERSIONNUM_MICRO(SDLVersion)));
				Report.push_back("TTF Version: " + to_string(SDL_VERSIONNUM_MAJOR(TTFVersion)) + "." + to_string(SDL_VERSIONNUM_MINOR(TTFVersion)) + "." + to_string(SDL_VERSIONNUM_MICRO(TTFVersion)));
				Report.push_back("Camera Position x: " + format("{:.1f}",simple.Camera.pos.x) + " y: " + format("{:.1f}", simple.Camera.pos.y) + " z: " + format("{:.1f}", simple.Camera.pos.z));
				Report.push_back("Camera Rotation Yaw: " + format("{:.1f}", simple.CameraYaw) + " Pitch: " + format("{:.1f}", simple.CameraPitch));
				Report.push_back("Camera Velocity x: " + format("{:.1f}", simple.Camera.velocity.x) + " y: " + format("{:.1f}", simple.Camera.velocity.y) + " z: " + format("{:.1f}", simple.Camera.velocity.z));
				Report.push_back("Rendering Performance: " + to_string(FPS) + "fps | " + to_string(Frametime) + "ms");
			}
		}

		input.pollInput(&KEYBOARD);
		
		// Get Mouse Data
		//SDL_GetMouseState(&world.Input0.MouseX, &world.Input0.MouseY);
		//simple.CameraYaw += (world.Input0.LastMouseX - world.Input0.MouseX) / ScreenWidthF * world.Input0.MouseSensitivity;
		//world.Input0.LastMouseX = world.Input0.MouseX;
		//world.Input0.LastMouseY = world.Input0.MouseY;
	
		// Keep Rotation within 0.0f to 360.0f
		if (simple.CameraYaw > 360.0f) simple.CameraYaw -= 360.0f;
		if (simple.CameraYaw < 0.0f) simple.CameraYaw += 360.0f;
		if (simple.CameraPitch > 360.0f) simple.CameraPitch -= 360.0f;
		if (simple.CameraPitch < 0.0f) simple.CameraPitch += 360.0f;

		// Handle Reading Results of Key Inputs
		CurrentTime = SDL_GetTicks();
		if (CurrentTime > KEYBOARD.o_LastTime + 100) {
			if (KEYBOARD.o == true) {
				KEYBOARD.o_LastTime = CurrentTime;
				simple.DepthBufferShown = simple.DepthBufferShown ? false : true;
			}
		}
		if (CurrentTime > KEYBOARD.esc_LastTime + 100) {
			if (KEYBOARD.esc == true) {
				KEYBOARD.esc_LastTime = CurrentTime;
				Pause = Pause ? false : true;
			}
		}
		if (KEYBOARD.f3 == true) {
			if (KEYBOARD.f3_lastState == false) {
				KEYBOARD.f3_lastState = true;
				world.DebugMenuShown = world.DebugMenuShown ? false : true;
			}
		}
		if (KEYBOARD.f3 == false) {
			if (KEYBOARD.f3_lastState == true) {
				KEYBOARD.f3_lastState = false;
			}
		}
		// Movement
		if (KEYBOARD.w == true) simple.Camera.velocity.z += 0.001f * world.TickStrength;
		if (KEYBOARD.a == true) simple.Camera.velocity.x -= 0.001f * world.TickStrength;
		if (KEYBOARD.s == true) simple.Camera.velocity.z -= 0.001f * world.TickStrength;
		if (KEYBOARD.d == true) simple.Camera.velocity.x += 0.001f * world.TickStrength;
		if (KEYBOARD.space == true) simple.Camera.velocity.y += 0.001f * world.TickStrength;
		if (KEYBOARD.lshift == true) simple.Camera.velocity.y -= 0.001f * world.TickStrength;
	}
}