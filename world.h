#ifndef WORLD_H // only defines if variable has not been defined yet
#define WORLD_H

using namespace std;
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <string>
#include <vector>
#include "defs.h"

// declare game so game.cpp can use it
class WORLD {
public:
	vector<Point> Points;
	vector<Line> Lines;
	vector<Triangle> Triangles;
	vector<Plane> Planes;
	void init();
	void tick();
	float TickStrength = 0.0f;
	bool DebugMenuShown = false;
};
// declare r1 so the R1 functions can be called elsewhere
extern WORLD world;

#endif // WORLD_H