#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include "class.h"
#include <vector>

vector<Point> Points;
vector<Line> Lines;
vector<Triangle> Triangles;
vector<Plane> Planes;
// bright RGB colors
RGBA_float bright_red(255.0f, 0.0f, 0.0f, 255.0f);
RGBA_float bright_blue(100.0f, 100.0f, 200.0f, 255.0f);
RGBA_float bright_green(0.0f, 255.0f, 0.0f, 255.0f);
// dark RGB colors
RGBA_float dark_red(128.0f, 0.0f, 0.0f, 255.0f);
RGBA_float dark_green(0.0f, 128.0f, 0.0f, 255.0f);
RGBA_float dark_blue(0.0f, 0.0f, 128.0f, 255.0f);
RGBA_float black(75.0f, 75.0f, 75.0f, 255.0f);

void WORLD::init() {
	if (debug == true) { cout << "[DEBUG] function world.init() from world.cpp" << endl; }
	// add Points to World
	Points.push_back(Point('O', Pos(0.0f, 0.0f, 0.0f), black));
	Points.push_back(Point('A', Pos(1.0f, 1.0f, 1.5f), bright_red));
	Points.push_back(Point('B', Pos(2.0f, 1.25f, 2.0f), bright_green));
	Points.push_back(Point('C', Pos(1.5f, 2.0f, 2.0f), bright_blue));
	Points.push_back(Point('D', Pos(4.0f, 4.0f, -3.0f), dark_red));
	Points.push_back(Point('E', Pos(0.0f, 0.05f, 0.0f), dark_green));

	// add Triangles to World
	Triangles.push_back(Triangle(Points[1], Points[2], Points[3], bright_red));
	Triangles.push_back(Triangle(Points[0], Points[1], Points[2], bright_blue));

	// add Planes to World
	Planes.push_back(Plane(Triangles[0], bright_blue));
}

void WORLD::tick() {
	if (debug == true) { cout << "[DEBUG] function game.tick() from game.cpp" << endl; }
	// Update Physics, later for all physics objects in the world
	simple.Camera.pos.x += simple.Camera.velocity.x * 0.1f * TickStrength;
	simple.Camera.pos.y += simple.Camera.velocity.y * 0.1f * TickStrength;
	simple.Camera.pos.z += simple.Camera.velocity.z * 0.1f * TickStrength;
	simple.Camera.velocity.x *= 0.9f * TickStrength;
	simple.Camera.velocity.y *= 0.9f * TickStrength;
	simple.Camera.velocity.z *= 0.9f * TickStrength;
}

WORLD world;