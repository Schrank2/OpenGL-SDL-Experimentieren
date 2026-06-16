#ifndef DEFS_H // only defines if variable has not been defined yet
#define DEFS_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>
/* Information about the current video settings. */
/* extern const SDL_VideoInfo; */
/* Dimensions of our window. */
extern int ScreenWidth;
extern int ScreenHeight;
/* Color depth in bits of our window. */
extern int bpp;
/* Flags we will pass into SDL_SetVideoMode. */
extern int flags;
extern bool debug;
extern SDL_Window* window;
extern SDL_Renderer* renderer;
extern SDL_GLContext OpenglContext;

// declare R1 so rastSdlRender.cpp can use it
class R1 {
public:
	void render();
};
// declare r1 so the R1 functions can be called elsewhere
extern R1 r1;

// declare game so game.cpp can use it
class WORLD {
public:
	void init();
	void tick();
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;

// declare the game objects
struct Pos {
	float x, y, z;
	Pos(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Vector {
	float x, y, z;
	Vector(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Color_RGB {
	float r, g, b;
	Color_RGB(float r, float g, float b) : r(r), g(g), b(b) {}
};

struct Point {
	char letter;
	Pos position;
	Color_RGB color;
	Point(char letter, Pos position, Color_RGB color) : letter(letter), position(position), color(color) {}
};

struct Line {
	char letter;
	Point p1, p2;
	Color_RGB color;
	Line(Point p1, Point p2, char letter, Color_RGB color) : p1(p1), p2(p2), letter(letter), color(color) {}
};

struct Triangle {
	Point p1, p2, p3;
	string name;
	Color_RGB color;
	string getname() {
		name = "triangle_" + to_string(p1.letter) + to_string(p2.letter) + to_string(p3.letter);
		return name;
	}
	Triangle(Point p1, Point p2, Point p3, Color_RGB color) : p1(p1), p2(p2), p3(p3), name(getname()), color(color) {}
};

struct Plane {
	Triangle triangle;
	string name;
	Color_RGB color;
	Vector SupportV;
	Vector Span1V;
	Vector Span2V;
	// get vectors of the plane
	Vector GetVector(Pos start, Pos end) {
		return Vector(end.x - start.x, end.y - start.y, end.z - start.z);
	}
	// get name of the plane
	string getname() {
		name = "plane_" + to_string(triangle.p1.letter) + to_string(triangle.p2.letter) + to_string(triangle.p3.letter);
		return name;
	}
	// constructor
	Plane(Triangle triangle, Color_RGB color) : SupportV(GetVector(Pos(0,0,0), triangle.p1.position)), Span1V(GetVector(triangle.p1.position, triangle.p2.position)), Span2V(GetVector(triangle.p1.position, triangle.p3.position)), triangle(triangle), name(getname()), color(color) {}
};


#endif // DEFS_H