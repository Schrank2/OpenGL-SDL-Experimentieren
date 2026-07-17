#ifndef DEFS_H // only defines if variable has not been defined yet
#define DEFS_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "input.h"
/* Information about the current video settings. */
/* extern const SDL_VideoInfo; */
/* Color depth in bits of our window. */
extern int bpp;
/* Flags we will pass into SDL_SetVideoMode. */
extern int flags;
extern bool debug;
// Text for the Report
extern vector<string> Report;
extern bool DoReport;
extern vector<Button> mainInput;
extern Mouse mainMouse;

// declare the game objects
struct Pos {
	float x, y, z;
	Pos(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct ScreenPos {
	float x, y, z;
	bool valid = true;
	ScreenPos(float x, float y, float z, bool valid) : x(x), y(y), z(z), valid(valid) {}
};

float CapColor_float(float color);

int CapColor_int(int color);

struct RGBA_float {
	float r, g, b, a;
	RGBA_float(float r, float g, float b, float a) : r(CapColor_float(r)), g(CapColor_float(g)), b(CapColor_float(b)), a(CapColor_float(a)) {}
};

struct RGBA_int {
	Uint8 r, g, b, a;
	RGBA_int(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(CapColor_int(r)), g(CapColor_int(g)), b(CapColor_int(b)), a(CapColor_int(a)) {}
};

struct Point {
	char letter;
	float position[3];
	RGBA_int color;
	Point(char letter, float position[3], RGBA_int color) : letter(letter), position(position[3]), color(color) {}
};

struct Line {
	char letter;
	Point p1, p2;
	RGBA_int color;
	Line(Point p1, Point p2, char letter, RGBA_int color) : p1(p1), p2(p2), letter(letter), color(color) {}
};

struct Triangle {
	Point p1, p2, p3;
	string name;
	RGBA_int color;
	const string triangle_getname() {
		string name = "Triangle ";
		name += p1.letter;
		name += p2.letter;
		name += p3.letter;
		return name;
	}
	Triangle(Point p1, Point p2, Point p3, RGBA_int color) : p1(p1), p2(p2), p3(p3), name(triangle_getname()), color(color) {}
};

struct Plane {
	Triangle triangle;
	string name;
	RGBA_int color;
	Pos SupportV;
	Pos Span1V;
	Pos Span2V;
	// get vectors of the plane
	Pos GetVector(Pos start, Pos end) {
		return Pos(end.x - start.x, end.y - start.y, end.z - start.z);
	}
	// get name of the plane
	const string plane_getname() {
		string name = "Plane ";
		name += triangle.p1.letter;
		name += triangle.p2.letter;
		name += triangle.p3.letter;
		return name;
	}
	// constructor
	Plane(Triangle triangle, RGBA_int color) : SupportV(GetVector(Pos(0,0,0), triangle.p1.pos)), Span1V(GetVector(triangle.p1.pos, triangle.p2.pos)), Span2V(GetVector(triangle.p1.pos, triangle.p3.pos)), triangle(triangle), name(plane_getname()), color(color) {}
};

struct PhysicsObject {
	Pos pos;
	Pos velocity;
	float mass;
	PhysicsObject(Pos pos, Pos velocity, float mass) : pos(pos), velocity(velocity), mass(mass) {}
};



#endif // DEFS_H