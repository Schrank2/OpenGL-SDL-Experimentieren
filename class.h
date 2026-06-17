#ifndef CLASS_H // only defines if variable has not been defined yet
#define CLASS_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <string>
#include <vector>
#include "defs.h"

// declare R1 so rastSdlRender.cpp can use it
class SimpleRenderer {
public:
	void render();
	void open_window();
	void draw();
	SDL_Window* window;
	SDL_Renderer* renderer;
};
// declare r1 so the R1 functions can be called elsewhere
extern SimpleRenderer simple;

// declare game so game.cpp can use it
class WORLD {
public:
	vector<Point> Points;
	void init();
	void tick();
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;

#endif // CLASS_H