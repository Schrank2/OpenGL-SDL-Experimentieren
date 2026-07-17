#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "SimpleRenderer.h"
#include "world.h"
#include <vector>

vector<Point> Points;
vector<Line> Lines;
vector<Triangle> Triangles;
vector<Plane> Planes;
// bright RGB colors
RGBA_int bright_red(255, 0, 0, 255);
RGBA_int bright_blue(100, 100, 200, 255);
RGBA_int bright_green(0, 255, 0, 255);
// dark RGB colors
RGBA_int dark_red(128, 0, 0, 255);
RGBA_int dark_green(0, 128, 0, 255);
RGBA_int dark_blue(0, 0, 128, 255);
RGBA_int black(75, 75, 75, 255);

void WORLD::init(int* ScreenWidth, int* ScreenHeight) {
	if (debug == true) { cout << "[DEBUG] function world.init() from world.cpp" << endl; }
	world.ScreenWidth = *ScreenWidth;
	world.ScreenHeight = *ScreenHeight;
	world.ScreenWidthF = static_cast<float>(world.ScreenWidth);
	world.ScreenHeightF = static_cast<float>(world.ScreenHeight);
	// add Points to World
	Points.push_back(Point('O', { 0.0f, 0.0f, 0.0f }, black));
	Points.push_back(Point('A', { 2.0f, 2.0f, 2.0f }, bright_red));
	Points.push_back(Point('B', { 2.0f, 0.0f, 0.0f }, bright_green));
	Points.push_back(Point('C', { 0.0f, 0.0f, 2.0f }, bright_blue));
	Points.push_back(Point('D', { 4.0f, 4.0f, -3.0f }, bright_red));
	Points.push_back(Point('E', { 0.0f, 0.05f, 0.0f }, bright_green));
	Points.push_back(Point('X', { 1.0f, 0.0f, 0.0f }, dark_blue));
	Points.push_back(Point('Y', { 0.0f, 1.0f, 0.0f }, dark_red));
	Points.push_back(Point('Z', { 0.0f, 0.0f, 1.0f }, dark_green));

	// add Triangles to World
	Triangles.push_back(Triangle(Points[1], Points[2], Points[3], bright_red));
	Triangles.push_back(Triangle(Points[0], Points[2], Points[3], bright_blue));

	// add Planes to World
	Planes.push_back(Plane(Triangles[0], bright_blue));

	// add Lines to World
	//Lines.push_back(Line(Points[0], Points[6], 'x', dark_blue));
	//Lines.push_back(Line(Points[0], Points[7], 'y', dark_red));
	//Lines.push_back(Line(Points[0], Points[8], 'z', dark_green));
}

void WORLD::tick() {
	if (debug == true) { cout << "[DEBUG] function game.tick() from game.cpp" << endl; }
	// Update Camera Rotation
	simple.CameraYaw += 90.0f * (mainMouse.movex / world.ScreenWidthF) * mainMouse.sens;
	simple.CameraPitch += 90.0f * (mainMouse.movey / world.ScreenHeightF) * mainMouse.sens;
	mainMouse.movex = 0.0f;
	mainMouse.movey = 0.0f;
	
	// Keep Rotation within 0.0f to 360.0f
	if (simple.CameraYaw > 360.0f) simple.CameraYaw -= 360.0f;
	if (simple.CameraYaw < 0.0f) simple.CameraYaw += 360.0f;
	if (simple.CameraPitch > 360.0f) simple.CameraPitch -= 360.0f;
	if (simple.CameraPitch < 0.0f) simple.CameraPitch += 360.0f;
	if (simple.CameraYaw > 1000.0f or simple.CameraYaw <= -1000.0f) simple.CameraYaw = 0.0f;
	if (simple.CameraPitch > 1000.0f or simple.CameraPitch <= -1000.0f) simple.CameraPitch = 0.0f;
	// Update Movement
	float Yaw = simple.CameraYaw * (3.14 / 180.0f);
	float Pitch = simple.CameraPitch * (3.14 / 180.0f);
	float x = cos(Yaw) - sin(Yaw);
	float z = cos(Yaw) + sin(Yaw);
	// W
	if (mainInput[0].active == true) {
		simple.Camera.velocity.z += 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.x += 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.z += 10.0f * world.TickStrength;
	}
	// A
	if (mainInput[1].active == true) {
		simple.Camera.velocity.x -= 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.z += 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.x -= 10.0f * world.TickStrength;
	}
	// S
	if (mainInput[2].active == true) {
		simple.Camera.velocity.z -= 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.x -= 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.z -= 10.0f * world.TickStrength;
	}
	// D
	if (mainInput[3].active == true) {
		simple.Camera.velocity.x += 10.0f * world.TickStrength * cos(Yaw);
		simple.Camera.velocity.z -= 10.0f * world.TickStrength * sin(Yaw);
		//simple.Camera.velocity.x += 10.0f * world.TickStrength;
	}

	if (mainInput[4].active == true) simple.Camera.velocity.y += 10.0f * world.TickStrength;
	if (mainInput[5].active == true) simple.Camera.velocity.y -= 10.0f * world.TickStrength;
	// Update Physics, later for all physics objects in the world
	simple.Camera.pos.x += simple.Camera.velocity.x * TickStrength;
	simple.Camera.pos.y += simple.Camera.velocity.y * TickStrength;
	simple.Camera.pos.z += simple.Camera.velocity.z * TickStrength;
	simple.Camera.velocity.x *= 0.9f;
	simple.Camera.velocity.y *= 0.9f;
	simple.Camera.velocity.z *= 0.9f;
}

WORLD world;