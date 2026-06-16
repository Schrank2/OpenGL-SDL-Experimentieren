#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include "functions.h"
#include "defs.h"
#include <vector>

void WORLD::init() {
	if (debug == true) { cout << "[DEBUG] function game.init() from game.cpp" << endl; }
}

void WORLD::tick() {
	if (debug == true) { cout << "[DEBUG] function game.tick() from game.cpp" << endl; }
}

WORLD world;