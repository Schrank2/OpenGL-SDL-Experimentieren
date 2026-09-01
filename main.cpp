#include <SDL3/SDL.h>
#include "functions.h"
#include "defs.h"
#include "SimpleRenderer.h"
#include "world.h"
#include "ray.h"
#include "input.h"
#include "OpenGLRenderer.h"
#include <iomanip> // for more precise floats in cout
#include <thread>
#include <format> // for to_string floats to not show too many decimal numbers
using namespace std;
bool debug = false;
bool report = true;
vector<Button> mainInput;
Mouse mainMouse;

int main(int argc, char* argv[])
{
	if (debug == true) { cout << "[DEBUG] function main() from main.cpp" << endl; }
	simple.active = true;
	open.active = false;
	ray.active = false;

	simple.ThreadAllocation = thread::hardware_concurrency() - 2;
	simple.ThreadAllocation = simple.ThreadAllocation < 1 ? 1 : simple.ThreadAllocation;

	float ScreenPercentage = 0.75;
	int MonitorWidth = 0;
	int MonitorHeight = 0;
	init_libs();
	simple.GetScreenData(&MonitorWidth, &MonitorHeight);
	int ScreenWidth = static_cast<int>(MonitorWidth * 0.75);
	int ScreenHeight = static_cast<int>(MonitorHeight * 0.75);
	if(simple.active) simple.init(&ScreenWidth, &ScreenHeight);
	if (open.active) open.init(&ScreenWidth, &ScreenHeight);
	if (ray.active) ray.init(&ScreenWidth, &ScreenHeight, &ScreenPercentage, "RayCastingRenderer");
	world.init(&ScreenWidth, &ScreenHeight);
	bool running = true;

	float Frametime = 0.0f;
	float FrameRateTarget = 240.0f;
	float FrameTimeTarget = 1000.0f / FrameRateTarget;
	float FrameStartTime = 0.0f;
	float CurrentTime = 0.0f;
	float LastReportTime = 0.0f;
	float FPS = 0.0f;

	float Ticktime = 0.0f;
	float TickRateTarget = 60.0f;
	float TickTimeTarget = 1000.0f / TickRateTarget;
	world.TickStrength = TickRateTarget / 1000.0f;
	float TickStartTime = 0.0f;
	float TPS = 0.0f;

	bool Pause = false;
	input.init(&mainInput);
	bool wasPaused = false;
	float PauseStartTime = 0;
	float PauseTime = 0;
	SDL_SetWindowRelativeMouseMode(simple.window, true);

	while (running) {
		CurrentTime = static_cast<float>(SDL_GetTicks());
		input.poll(&mainInput);
		input.pollMouse(&mainMouse);
		PauseTime = 0;
		if (mainInput[6].active) {
			if (!wasPaused) { // pause starts
				PauseStartTime = CurrentTime;
				SDL_SetWindowRelativeMouseMode(simple.window, false);
			}
			wasPaused = true;
		}
		else {
			if (wasPaused) { // pause ends
				PauseTime = CurrentTime - PauseStartTime; 
				SDL_SetWindowRelativeMouseMode(simple.window, true);
			}
			wasPaused = false;
		}

		if (!mainInput[6].active && CurrentTime > TickStartTime + TickTimeTarget - PauseTime) {
			Ticktime = CurrentTime - PauseTime - TickStartTime;
			if (Ticktime < 0.0f) Ticktime = 0.0f;
			world.TickStrength = abs(Ticktime / 1000.0f);
			world.tick();
			TickStartTime = static_cast<float>(SDL_GetTicks());
		}
		// Rendering and Showing a Plane
		if (CurrentTime > FrameStartTime + FrameTimeTarget) {
			Frametime = CurrentTime - FrameStartTime;
			if(simple.active) simple.render(&world.Lines, &world.Triangles, &world.Points);
			if (open.active) open.render();
			FrameStartTime = CurrentTime;
		}
		if (CurrentTime >= LastReportTime + TickRateTarget) {
			LastReportTime = CurrentTime;
			if (report == true) {
				FPS = 1000.0f / Frametime;
				Report.clear();
				int SDLVersion = SDL_GetVersion();
				int TTFVersion = TTF_Version();
				if (mainInput[6].active) Report.push_back("---GAME TICKING PAUSED---");
				Report.push_back("SDL Version: " + to_string(SDL_VERSIONNUM_MAJOR(SDLVersion)) + "." + to_string(SDL_VERSIONNUM_MINOR(SDLVersion)) + "." + to_string(SDL_VERSIONNUM_MICRO(SDLVersion)));
				Report.push_back("TTF Version: " + to_string(SDL_VERSIONNUM_MAJOR(TTFVersion)) + "." + to_string(SDL_VERSIONNUM_MINOR(TTFVersion)) + "." + to_string(SDL_VERSIONNUM_MICRO(TTFVersion)));
				Report.push_back("Camera Position x: " + format("{:.1f}",simple.Camera.pos.x) + " y: " + format("{:.1f}", simple.Camera.pos.y) + " z: " + format("{:.1f}", simple.Camera.pos.z));
				Report.push_back("Camera Rotation Yaw: " + format("{:.1f}", simple.CameraYaw) + " Pitch: " + format("{:.1f}", simple.CameraPitch));
				Report.push_back("Camera Velocity x: " + format("{:.3f}", simple.Camera.velocity.x) + " y: " + format("{:.3f}", simple.Camera.velocity.y) + " z: " + format("{:.3f}", simple.Camera.velocity.z));
				Report.push_back("Rendering Performance: " + format("{:.1f}", FPS) + "fps | " + format("{:.1f}", Frametime) + "ms");
				Report.push_back("Sin(Yaw): " + format("{:.2f}", sin(simple.CameraYaw*(3.14/180.0f))));
				Report.push_back("Cos(Yaw): " + format("{:.2f}", cos(simple.CameraYaw * (3.14 / 180.0f))));
				Report.push_back("Triangles Rendered: " + to_string(simple.PolyGonsRenderedTotal));
				Report.push_back("TriangleDrawingTime: " + to_string(simple.TriangleDrawingTime));
				Report.push_back("ThreadMergingTime: " + to_string(simple.ThreadMergingTime));
				Report.push_back("DepthBufferMergingTime: " + to_string(simple.DepthBufferMergingTime));
			}
		}
	}
}

static vector<float> convert(vector<Triangle>* &Triangles) {
	vector<float> Output;
	for (int i = 0; i < (*Triangles).size(); i++) {
		Output.push_back((*Triangles)[i].p1.position.x);
		Output.push_back((*Triangles)[i].p1.position.y);
		Output.push_back((*Triangles)[i].p1.position.z);
		Output.push_back((*Triangles)[i].p2.position.x);
		Output.push_back((*Triangles)[i].p2.position.y);
		Output.push_back((*Triangles)[i].p2.position.z);
		Output.push_back((*Triangles)[i].p3.position.x);
		Output.push_back((*Triangles)[i].p3.position.y);
		Output.push_back((*Triangles)[i].p3.position.z);
	}
	return Output;
}