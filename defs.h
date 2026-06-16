#ifndef DEFS_H // only defines if variable has not been defined yet
#define DEFS_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
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
class GameLogic {
public:
	void init();
	void tick();
};
// declare r1 so the R1 functions can be called elsewhere
extern GameLogic game;

// declare the game objects
struct Pos {
	float x, y, z;
	Pos(float x, float y, float z) : x(x), y(y), z(z) {}
};

struct Color_RGB {
	float r, g, b;
	Color_RGB(float r, float g, float b) : r(r), g(g), b(b) {}
};

struct Point {
	char letter;
	Pos position;
	Color_RGB color;
};

struct line {
	char letter;
	Point p1, p2;
	Color_RGB color;
};

struct triangle {
	chars[3] letters;
	Point p1, p2, p3;
	Color_RGB color;
};




#endif // DEFS_H