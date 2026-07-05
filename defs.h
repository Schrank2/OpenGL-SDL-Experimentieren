#ifndef DEFS_H // only defines if variable has not been defined yet
#define DEFS_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
/* Information about the current video settings. */
/* extern const SDL_VideoInfo; */
/* Dimensions of our window. */
extern int ScreenWidth;
extern int ScreenHeight;
extern float ScreenWidthF;
extern float ScreenHeightF;
/* Color depth in bits of our window. */
extern int bpp;
/* Flags we will pass into SDL_SetVideoMode. */
extern int flags;
extern bool debug;
// Text for the Report
extern string Report;
extern bool DoReport;

// declare the game objects
struct Pos {
	float x, y, z;
	Pos(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct ScreenPos {
	float x, y, z;
	ScreenPos(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector {
	float x, y, z;
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}
};

float CapColor_float(float color);

int CapColor_int(int color);

struct RGBA_float {
	float r, g, b, a;
	RGBA_float(float r, float g, float b, float a) : r(CapColor_float(r)), g(CapColor_float(g)), b(CapColor_float(b)), a(CapColor_float(a)) {}
};

struct RGBA_int {
	int r, g, b, a;
	RGBA_int(int r, int g, int b, int a) : r(CapColor_int(r)), g(CapColor_int(g)), b(CapColor_int(b)), a(CapColor_int(a)) {}
};

struct Point {
	char letter;
	Pos pos;
	RGBA_float color;
	Point(char letter, Pos pos, RGBA_float color) : letter(letter), pos(pos), color(color) {}
};

struct Line {
	char letter;
	Point p1, p2;
	RGBA_float color;
	Line(Point p1, Point p2, char letter, RGBA_float color) : p1(p1), p2(p2), letter(letter), color(color) {}
};

struct Triangle {
	Point p1, p2, p3;
	string name;
	RGBA_float color;
	const string triangle_getname() {
		string name = "Triangle ";
		name += p1.letter;
		name += p2.letter;
		name += p3.letter;
		return name;
	}
	Triangle(Point p1, Point p2, Point p3, RGBA_float color) : p1(p1), p2(p2), p3(p3), name(triangle_getname()), color(color) {}
};

struct Plane {
	Triangle triangle;
	string name;
	RGBA_float color;
	Vector SupportV;
	Vector Span1V;
	Vector Span2V;
	// get vectors of the plane
	Vector GetVector(Pos start, Pos end) {
		return Vector(end.x - start.x, end.y - start.y, end.z - start.z);
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
	Plane(Triangle triangle, RGBA_float color) : SupportV(GetVector(Pos(0,0,0), triangle.p1.pos)), Span1V(GetVector(triangle.p1.pos, triangle.p2.pos)), Span2V(GetVector(triangle.p1.pos, triangle.p3.pos)), triangle(triangle), name(plane_getname()), color(color) {}
};


#endif // DEFS_H